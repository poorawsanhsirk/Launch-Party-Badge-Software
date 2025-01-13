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
#define INJECTION_INTIMATION_TIME 0x20

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
const int LAUNCH_TARGET_HOUR = 16;    // 4:00 AM
const int LAUNCH_TARGET_MIN = 26;
const int LAUNCH_TARGET_SEC = 0;

// Target Inception Time for FF-1 (24-Hour Format)
const int FF1_INJECTION_TARGET_HOUR = 16;
const int FF1_INJECTION_TARGET_MIN = 27;
const int FF1_INJECTION_TARGET_SEC = 30;

// Target Inception Time for FF-1 (24-Hour Format)
const int FF2_INJECTION_TARGET_HOUR = 16;
const int FF2_INJECTION_TARGET_MIN = 28;
const int FF2_INJECTION_TARGET_SEC = 0;

// Target Inception Time for FF-1 (24-Hour Format)
const int FF3_INJECTION_TARGET_HOUR = 16;
const int FF3_INJECTION_TARGET_MIN = 28;
const int FF3_INJECTION_TARGET_SEC = 30;


// Global time structure
struct tm timeinfo;
// long timeRemainingToLaunch = 0;             
long timeRemainingToLaunchSincePowerON = 0;    // Global Variable to store how much time is remaining to launch
bool eventTransition = 0;
bool Launch_Done = 0;

/**
 * State Manager Global Variables
 */
int menuState;
int previousMenuState;
#define LAUNCH_TIMER 0
#define INJECTION_TIMERS 1
#define TRANSITION 2
#define LOGO 3




// Colors
const uint32_t COLOR_BLUE = 0x0099d9ea;
const uint32_t COLOR_GREEN = 0x0000FF00;

void connectToWifi();
void setupTime();
void LaunchTimer();
void flashGreen();
void transitionAnimation();


void setup() {
    Serial.begin(115200);   // UART for Debugging

    pinMode(9, INPUT);

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
    /**
     * Entering Launch Timer State by Default
     */
    menuState = LAUNCH_TIMER;
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

  // // Just one View for now
  // LaunchTimer();

  /**
   * TODO: Add a State machine to switch between the various views
   * 1) Launch Timer
   * 2) Injection Timer
   * 3) Time to next Pass
   * 4) Time to Sunlit/Eclipse
   * 5) Any other parameter that can be represented as a bar graph and can be fetched from the local server
   */
  // switch (menuState)
  // {
  // case LAUNCH_TIMER:
  //   /* code */
  //   LaunchTimer();
  //   break;
  // case INJECTION_TIMERS:
  //   InjectionTimers();
  //   break;
  // case TRANSITION:
  //   transitionAnimation();
  //   break;
  // default:
  //   break;
  // }
  if ((timeinfo.tm_hour*3600 + timeinfo.tm_min*60 + timeinfo.tm_sec) > LAUNCH_TARGET_HOUR*3600 + LAUNCH_TARGET_MIN*60 + LAUNCH_TARGET_SEC) {
    Launch_Done = 1;
  }
  InjectionTimers();

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

    // timeRemainingToLaunch = targetSecondsToLaunch - currentSeconds;
    timeRemainingToLaunchSincePowerON = targetSecondsToLaunch - currentSeconds;

    // Serial.print(timeRemainingToLaunch);
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

  long timeRemainingToLaunch = targetSecondsToLaunch - currentSeconds;

  /**
   * State Transition Logic
   */
  int buttonPress = digitalRead(9);
  if (buttonPress == 0) {
    delay(50);
    buttonPress = digitalRead(9);
    if (buttonPress == 0) {
      menuState = 2;
    }
  }
  // static int buttonState = HIGH;
  // static int lastButtonState = HIGH;
  // static unsigned long lastDebounceTime = 0;
  // const unsigned long debounceDelay = 50;

  // // int buttonPress = digitalRead(9);
  // if(buttonPress != lastButtonState){ // Check if button state has changed
  //   lastDebounceTime = millis();      // Reset the debounce timer
  // }
  // if((millis() - lastDebounceTime) > debounceDelay) {
  //   if(buttonPress != buttonState) {
  //     buttonState = buttonPress;

  //     // If the button state is HIGH, it means button is pressed
  //     if (buttonState == LOW) {
  //       menuState = 2;
  //     }
  //   }
  // }

  /**
   * TODO: Need to figure out the Launch Transition Event Handling properly. Spaghetti Logic at the moment
   */
  if (timeRemainingToLaunch <= INTIMATION_TIME){
    flashGreen();
    Launch_Done = 1;
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

void transitionAnimation() {
  while(digitalRead(9) == 0) {
    for (int i = 0; i < 4; i++) {
      Drivers[i].setBlockColour(0,255,0);         // Change to red after figuring out the setting with Yawar
    }
    delay(100);
    for (int i = 0; i < 4; i++) {
      Drivers[i].setBlockColour(0,0,0);         // Change to red after figuring out the setting with Yawar
    }
    delay(100);
  }

  // One more flash after button is released
  for (int i = 0; i < 4; i++) {
    Drivers[i].setBlockColour(0,255,0);         // Change to red after figuring out the setting with Yawar
  }
  delay(200);
  for (int i = 0; i < 4; i++) {
    Drivers[i].setBlockColour(0,0,0);         // Change to red after figuring out the setting with Yawar
  }
  delay(200);

  switch (previousMenuState)
  {
  case LAUNCH_TIMER:
    menuState = INJECTION_TIMERS;
    previousMenuState = menuState;
    break;
  
  case INJECTION_TIMERS:
    menuState = LAUNCH_TIMER;
    previousMenuState = menuState;
    break;
  
  default:
    break;
  }
}


static bool FF1InjectionDone = 0;
static bool FF2InjectionDone = 0;
static bool FF3InjectionDone = 0;


void InjectionTimers() {

  /** /////////////////////////////////////////////////////////////////////////////
   * State Transition Logic
   */ /////////////////////////////////////////////////////////////////////////////
  int buttonPress = digitalRead(9);
  if (buttonPress == 0) {
    delay(50);
    buttonPress = digitalRead(9);
    if (buttonPress == 0) {
      menuState = 2;
    }
  }
  // Very crude Debouncing. Change later if this code triggers anyone


  volatile float currentSeconds = timeinfo.tm_hour*3600 + timeinfo.tm_min*60 + timeinfo.tm_sec;
  static float LaunchTimeInSeconds = LAUNCH_TARGET_HOUR*3600 + LAUNCH_TARGET_MIN*60 + LAUNCH_TARGET_SEC;

  

  // FF1 Injection Parameters
  volatile float FF1TargetSecondsToSeparate = FF1_INJECTION_TARGET_HOUR*3600 + FF1_INJECTION_TARGET_MIN*60 + FF1_INJECTION_TARGET_SEC;
  if(FF1TargetSecondsToSeparate <= currentSeconds){
    FF1TargetSecondsToSeparate += 24*3600;         // Target is tomorrow
  }

  // FF2 Injection Parameters
  volatile float FF2TargetSecondsToSeparate = FF2_INJECTION_TARGET_HOUR*3600 + FF2_INJECTION_TARGET_MIN*60 + FF2_INJECTION_TARGET_SEC;
  if(FF2TargetSecondsToSeparate <= currentSeconds){
    FF2TargetSecondsToSeparate += 24*3600;         // Target is tomorrow
  }

  // FF3 Injection Parameters
  volatile float FF3TargetSecondsToSeparate = FF3_INJECTION_TARGET_HOUR*3600 + FF3_INJECTION_TARGET_MIN*60 + FF3_INJECTION_TARGET_SEC;
  if(FF3TargetSecondsToSeparate <= currentSeconds){
    FF3TargetSecondsToSeparate += 24*3600;         // Target is tomorrow
  }

  

  // Check if Launch is done
  if (Launch_Done == 0) {
    // Display some other colour on all Drivers till Launch is done
    for (int i = 0 ; i < 4 ; i++) {
      Drivers[i].setBlockColour(0, 100, 80);
    }
    delay(200);
    for (int i = 0 ; i < 4 ; i++) {
      Drivers[i].setBlockColour(0, 0, 0);
    }
  } else if (Launch_Done == 1) {
    // Start Injection Progress View after Launch_Done is set by LaunchTimer() or some logic in loop()

    // FF1 Display
    volatile float timeRemainingToInjectFF1 = FF1TargetSecondsToSeparate - currentSeconds;
    if (timeRemainingToInjectFF1 <= INJECTION_INTIMATION_TIME && timeRemainingToInjectFF1 > 1) {    // Indicator for the FF1 Injection Alert
      // Flash on Driver[3]
      Drivers[3].setBlockColour(0,0,255);
      delay(100);
      Drivers[3].setBlockColour(0,0,0);
      delay(100);
    } else if (timeRemainingToInjectFF1 == 0) {
      FF1InjectionDone = 1;
    } else if (FF1InjectionDone == 1) {
      Drivers[3].setBlockColour(0,100,80);
    } else {
      volatile float progressFF1 = ((FF1TargetSecondsToSeparate - LaunchTimeInSeconds) - timeRemainingToInjectFF1)/ (FF1TargetSecondsToSeparate - LaunchTimeInSeconds);
      if (progressFF1 < 0) {
        Drivers[3].setBlockColour(0, 0, 30);                                                       // Change to Red later
        return;
      }
      uint8_t numberOfRowsToLightFF1 = (uint8_t)(progressFF1 * 6) + 1;
      Drivers[3].lightNumberOfRows(numberOfRowsToLightFF1, 0x0000ff00);
    }

    // FF2 Display
    volatile float timeRemainingToInjectFF2 = FF2TargetSecondsToSeparate - currentSeconds;
    if (timeRemainingToInjectFF2 <= INJECTION_INTIMATION_TIME && timeRemainingToInjectFF2 > 1) {    // Indicator for the FF1 Injection Alert
      // Flash on Driver[2]
      Drivers[2].setBlockColour(0,0,255);
      delay(100);
      Drivers[2].setBlockColour(0,0,0);
      delay(100);
    } else if (timeRemainingToInjectFF2 == 0) {
      FF2InjectionDone = 1;
    } else if (FF2InjectionDone == 1) {
      Drivers[2].setBlockColour(0,100,80);
    } else {
      volatile float progressFF2 = ((FF2TargetSecondsToSeparate - LaunchTimeInSeconds) - timeRemainingToInjectFF2)/ (FF2TargetSecondsToSeparate - LaunchTimeInSeconds);
      if (progressFF2 < 0) {
        Drivers[2].setBlockColour(0, 0, 30);                                                       // Change to Red later
        return;
      }
      uint8_t numberOfRowsToLightFF2 = (uint8_t)(progressFF2 * 6) + 1;
      Drivers[2].lightNumberOfRows(numberOfRowsToLightFF2, 0x0000ff00);
    }

    // FF3 Display
    volatile float timeRemainingToInjectFF3 = FF3TargetSecondsToSeparate - currentSeconds;
    if (timeRemainingToInjectFF3 <= INJECTION_INTIMATION_TIME && timeRemainingToInjectFF3 > 1) {    // Indicator for the FF1 Injection Alert
      // Flash on Driver[1]
      Drivers[1].setBlockColour(0,0,255);
      delay(100);
      Drivers[1].setBlockColour(0,0,0);
      delay(100);
    } else if (timeRemainingToInjectFF3 == 0) {
      FF3InjectionDone = 1;
    } else if (FF3InjectionDone == 1) {
      Drivers[1].setBlockColour(0,100,80);
    } else {
      volatile float progressFF3 = ((FF3TargetSecondsToSeparate - LaunchTimeInSeconds) - timeRemainingToInjectFF3)/ (FF3TargetSecondsToSeparate - LaunchTimeInSeconds);
      if (progressFF3 < 0) {
        Drivers[1].setBlockColour(0, 0, 30);                                                       // Change to Red later
        return;
      }
      uint8_t numberOfRowsToLightFF3 = (uint8_t)(progressFF3 * 6) + 1;
      Drivers[1].lightNumberOfRows(numberOfRowsToLightFF3, 0x0000ff00);
    }
  }
}
