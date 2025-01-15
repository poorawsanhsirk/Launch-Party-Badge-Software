#include <WiFi.h>


#include "LP586x_I2C.h"
#include <time.h>
// #include "esp32-hal-time.h"

#include "Wire.h"
#include <stdint.h>


#define SLAVE_ADDRESS_1 0x10
#define SLAVE_ADDRESS_2 0x11
#define SLAVE_ADDRESS_3 0x12
#define SLAVE_ADDRESS_4 0x13

#define ENABLE_PIN 0x01
#define SYNC_PIN 0x00

#define sdaPin 0x05
#define sclPin 0x04

#define INTIMATION_TIME 0x3C

LP586X_I2C Drivers[] = {
  LP586X_I2C (SLAVE_ADDRESS_1, ENABLE_PIN, SYNC_PIN),
  LP586X_I2C (SLAVE_ADDRESS_2, ENABLE_PIN, SYNC_PIN),
  LP586X_I2C (SLAVE_ADDRESS_3, ENABLE_PIN, SYNC_PIN),
  LP586X_I2C (SLAVE_ADDRESS_4, ENABLE_PIN, SYNC_PIN)
};

// WiFi Credentials
const char* ssid = "Your SSID";
const char* password = "Your Password";

const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 19800;    // IST offset: (5 hours * 3600) + (30 minutes * 60) = 19800 seconds
const int daylightOffset_sec = 0;    // India doesn't use DST, so set to 0

const int NTP_UPDATE_INTERVAL = 3600000;  // Update NTP every hour (in milliseconds)
unsigned long lastNTPUpdate = 0;

// Target Launch Time (24-hour format)
const int LAUNCH_TARGET_HOUR = 4;    // 4:00 AM
const int LAUNCH_TARGET_MIN = 45;
const int LAUNCH_TARGET_SEC = 0;

// Global time structure
struct tm timeinfo;
long timeRemainingToLaunch = 0;             // Global Variable to store how much time is remaining to launch
long timeRemainingToLaunchSincePowerON = 0;
bool eventTransition = 0;


// Split the original bitmap into 4 sections
// const unsigned long matrix1[MATRIX_WIDTH * MATRIX_HEIGHT] PROGMEM = {0};
// const unsigned long matrix2[MATRIX_WIDTH * MATRIX_HEIGHT] PROGMEM = {0};
// const unsigned long matrix3[MATRIX_WIDTH * MATRIX_HEIGHT] PROGMEM = {0};
// const unsigned long matrix4[MATRIX_WIDTH * MATRIX_HEIGHT] PROGMEM = {0};

// Colors
const uint32_t COLOR_BLUE = 0x0099d9ea;
const uint32_t COLOR_GREEN = 0x0000FF00;

void connectToWifi();
void setupTime();
void LaunchTimer();


void setup() {
    Serial.begin(115200);   // UART for Debugging

    // Connect to Wifi
    connectToWifi();

    // Setup Time 
    setupTime();

    /**
     * Add Code to get data from the Server
     */


    

    /** ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
     *Setting up the LED Matrix ICs
    *//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    for (int i = 0 ; i < 4 ; i++) {
      Drivers[i].interfaceInit(sdaPin, sclPin);
    }
    // Resetting the IC
    for (int i = 0 ; i < 4 ; i++) {
      Drivers[i].disableDevice();
    }
    delay(10);
    for (int i = 0 ; i < 4 ; i++) {
      Drivers[i].enableDevice();
    }
    delay(10);
    // Software enabling the IC
    for (int i = 0 ; i < 4 ; i++) {
      Drivers[i].writeRegister(Chip_Enable_Register, 0x01);
    }
    for (int i = 0 ; i < 4 ; i++) {
      Drivers[i].setDeviceInitialRegister(Max_Line_11, Mode_2, PWM_Frequency_125k);
    }
    for (int i = 0 ; i < 4 ; i++) {
      Drivers[i].setDeviceConfigReg1( SW_BLK_1us,  PWM_Scale_Exponential, PWM_PhaseShift_on,  CS_Delay_on);
      Drivers[i].setDeviceConfigReg3( Down_Deghost_Strong,  Up_Deghost_GND,  Maximum_Current_15,  Up_Deghost_En);
    }
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

}

void loop() {
  unsigned long currentMillis = millis();

  // Check if it is time to update NTP
  if (currentMillis - lastNTPUpdate >= NTP_UPDATE_INTERVAL) {
      configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
      lastNTPUpdate = currentMillis;
  }
  
  // Update timeinfo Struct using local RTC
  if(!getLocalTime(&timeinfo)) {
      Serial.println("Failed to obtain time");
      return;
  }

  // Just one View for now
  LaunchTimer();

  /**
   * TODO: Add a State machine to switch between the various views
   * 1) Launch Timer
   * 2) Injection Timer
   * 3) Time to next Pass
   * 4) Time to Sunlit/Eclipse
   * 5) Any other parameter that can be represented as a bar graph and can be fetched from the local server
   */

}

void connectToWifi() {
    Serial.print("Connecting to ");
    Serial.println(ssid);
    
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    
    Serial.println("WiFi connected");
}

void setupTime() {
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
    if(!getLocalTime(&timeinfo)) {
        Serial.println("Failed to obtain time");
        return;
    }
    Serial.println("Time synchronized");

    // Calculate timeRemainingToXXXXX and set Global Variables
    int currentSeconds = timeinfo.tm_hour*3600 + timeinfo.tm_min*60 + timeinfo.tm_sec;
    int targetSecondsToLaunch = LAUNCH_TARGET_HOUR*3600 + LAUNCH_TARGET_MIN*60 + LAUNCH_TARGET_SEC;

    if(targetSecondsToLaunch <= currentSeconds){
      targetSecondsToLaunch += 24*3600;         // Target is tomorrow
    }

    timeRemainingToLaunch = targetSecondsToLaunch - currentSeconds;
    timeRemainingToLaunchSincePowerON = timeRemainingToLaunch;

    Serial.print(timeRemainingToLaunch);
}





void LaunchTimer() {
  int currentSeconds = timeinfo.tm_hour*3600 + timeinfo.tm_min*60 + timeinfo.tm_sec;
  int targetSecondsToLaunch = LAUNCH_TARGET_HOUR*3600 + LAUNCH_TARGET_MIN*60 + LAUNCH_TARGET_SEC;
  int LaunchDone = 0;

  if(targetSecondsToLaunch <= currentSeconds){
    targetSecondsToLaunch += 24*3600;         // Target is tomorrow
    if (eventTransition == 1){
      LaunchDone = 1;  
      eventTransition = 0;
    }
  }

  timeRemainingToLaunch = targetSecondsToLaunch - currentSeconds;


  /**
   * TODO: Need to figure out the Launch Transition Event Handling properly. Spaghetti Logic at the moment
   */
  if (timeRemainingToLaunch <= INTIMATION_TIME){
    flashGreen();
    return;
  } else if (timeRemainingToLaunch == 0) {
    eventTransition = 1;
  } else if (LaunchDone == 1){
    for (int i = 0 ; i < 4 ; i++) {
      Drivers[i].setBlockColour(0, 0, 255);
    }
    return;
  } else {
    float progress = (((float)timeRemainingToLaunchSincePowerON - (float)timeRemainingToLaunch)/(float)timeRemainingToLaunchSincePowerON);
    Serial.print(progress);
    if (progress < 0) {
      flashGreen();
      return;
    }
    int numberOfRowsLIT = (int)(progress*24) + 1;             // Show atleast one row to start
    if (numberOfRowsLIT <= 6) {                               // Addressing only 1 Driver
      for (int i = 0; i <=numberOfRowsLIT; i++) {
        Drivers[0].lightNumberOfRows(i , 0x0000ff00);
        delay(100);
      }
      delay(2000);                                            
      for (int i = 0; i <=numberOfRowsLIT; i++) {
        Drivers[0].lightNumberOfRows(i , 0x00000000);
        delay(10);
      }
      delay(1000);
      return;
      
    } else if (numberOfRowsLIT >6 && numberOfRowsLIT <=12) {  // Addressing 2nd Matrix. Fill all rows on previous Drivers
      for (int i = 0; i <=6; i++) {
        Drivers[0].lightNumberOfRows(i , 0x0000ff00);
        delay(100);
      }
      for (int i = 0; i <=numberOfRowsLIT-6; i++) {
        Drivers[1].lightNumberOfRows(i , 0x0000ff00);
        delay(100);
      }
      delay(2000);
      for (int i = 0; i <=6; i++) {
        Drivers[0].lightNumberOfRows(i , 0x00000000);
        // delay(100);
      }
      for (int i = 0; i <=numberOfRowsLIT-6; i++) {
        Drivers[1].lightNumberOfRows(i , 0x00000000);
        // delay(100);
      }
      delay(1000);
      return;
    } else if (numberOfRowsLIT >12 && numberOfRowsLIT <=18) {
      for (int i = 0; i <=6; i++) {
        Drivers[0].lightNumberOfRows(i , 0x0000ff00);
        delay(100);
      }
      for (int i = 0; i <=6; i++) {
        Drivers[1].lightNumberOfRows(i , 0x0000ff00);
        delay(100);
      }
      for (int i = 0; i <=numberOfRowsLIT-12; i++) {
        Drivers[2].lightNumberOfRows(i , 0x0000ff00);
        delay(100);
      }
      delay(2000);
      for (int i = 0; i <=6; i++) {
        Drivers[0].lightNumberOfRows(i , 0x00000000);
        Drivers[1].lightNumberOfRows(i , 0x00000000);
        // delay(100);
      }
      for (int i = 0; i <=numberOfRowsLIT-12; i++) {
        Drivers[2].lightNumberOfRows(i , 0x00000000);
        // delay(100);
      }
      delay(1000);
      return;
    } else if (numberOfRowsLIT >18 && numberOfRowsLIT <=23) {
      for (int i = 0; i <=6; i++) {
        Drivers[0].lightNumberOfRows(i , 0x0000ff00);
        delay(100);
      }
      for (int i = 0; i <=6; i++) {
        Drivers[1].lightNumberOfRows(i , 0x0000ff00);
        delay(100);
      }
      for (int i = 0; i <=6; i++) {
        Drivers[2].lightNumberOfRows(i , 0x0000ff00);
        delay(100);
      }
      for (int i = 0; i <=numberOfRowsLIT-18; i++) {
        Drivers[3].lightNumberOfRows(i , 0x0000ff00);
        delay(100);
      }
      delay(2000);
      for (int i = 0; i <=6; i++) {
        Drivers[0].lightNumberOfRows(i , 0x00000000);
        Drivers[1].lightNumberOfRows(i , 0x00000000);
        Drivers[2].lightNumberOfRows(i , 0x00000000);
        // delay(100);
      }
      for (int i = 0; i <=numberOfRowsLIT-12; i++) {
        Drivers[3].lightNumberOfRows(i , 0x00000000);
        // delay(100);
      }
      delay(1000);
      return;
    }
  }

}

void flashGreen() {
  for (int i = 0 ; i < 4 ; i++) {
    Drivers[i].setBlockColour(0, 0, 255);
  }
  delay(100);
  for (int i = 0 ; i < 4 ; i++) {
    Drivers[i].setBlockColour(0, 0, 0);
  }
  delay(100);
}