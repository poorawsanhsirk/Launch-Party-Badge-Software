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


LP586X_I2C Drivers[] = {
  LP586X_I2C (SLAVE_ADDRESS_1, ENABLE_PIN, SYNC_PIN),
  LP586X_I2C (SLAVE_ADDRESS_2, ENABLE_PIN, SYNC_PIN),
  LP586X_I2C (SLAVE_ADDRESS_3, ENABLE_PIN, SYNC_PIN),
  LP586X_I2C (SLAVE_ADDRESS_4, ENABLE_PIN, SYNC_PIN)
};

/**
 * State Manager Global Variables
 */
int menuState;
// int previousMenuState, previousMenuState_1;


// Colors
const uint32_t COLOR_BLUE = 0x0099d9ea;
const uint32_t COLOR_GREEN = 0x0000FF00;


void setup() {
    // Serial.begin(115200);   // UART for Debugging
    pinMode(9, INPUT);
    pinMode(3, INPUT);
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

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /**
     * Entering Launch Timer State by Default
     */
    // menuState = LAUNCH_TIMER;
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

}
int animationNumber = 0;
const int maxAnimations = 5; // Total number of animations

// Debouncing variables
bool buttonState = HIGH; // Current button state
bool lastButtonState = HIGH; // Previous button state
unsigned long lastDebounceTime = 0; // Last time the button state changed
unsigned long debounceDelay = 50; // Debounce delay (in milliseconds)

void flashGreen();
void flashBlue();

void loop() {
  int reading = digitalRead(3); // Read the button state

  // Check for state change and debounce
  if (reading != lastButtonState) {
    lastDebounceTime = millis(); // Reset debounce timer
  }

  // If the debounce delay has passed and the state has changed
  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading == LOW && buttonState == HIGH) { // Button pressed
      animationNumber = (animationNumber + 1) % maxAnimations; // Increment and loop back
    }
    buttonState = reading; // Update the current button state
  }

  lastButtonState = reading; // Save the last button state
  switch (animationNumber)
  {
  case 0:
    // Launch Timer Animation
    for (int j = 0; j <=6; j++) {
      Drivers[0].lightNumberOfRows(j , 0x0000ff00);
      delay(300);
    }
    for (int j = 0; j <=6; j++) {
      Drivers[1].lightNumberOfRows(j , 0x0000ff00);
      delay(300);
    }
    for (int j = 0; j <=6; j++) {
      Drivers[2].lightNumberOfRows(j , 0x0000ff00);
      delay(300);
    }
    for (int j = 0; j <=6; j++) {
      Drivers[3].lightNumberOfRows(j , 0x0000ff00);
      delay(300);
    }
    for (int j = 0; j <=6; j++) {
      Drivers[0].lightNumberOfRows(j , 0x00000000);
      // delay(100);
    }
    for (int j = 0; j <=6; j++) {
      Drivers[1].lightNumberOfRows(j , 0x00000000);
      // delay(100);
    }
    for (int j = 0; j <=6; j++) {
      Drivers[2].lightNumberOfRows(j , 0x00000000);
      // delay(100);
    }
    for (int j = 0; j <=6; j++) {
      Drivers[3].lightNumberOfRows(j , 0x00000000);
      // delay(100);
    }
    break;
  case 1:
    // Launch Intimation Animation
    flashGreen();
    break;
  case 2:
    // Transition Animation;
    flashBlue();
    break;
  case 3:
    // Injection Timer Animation
    for (int j = 0; j <=6; j++) {
      Drivers[1].lightNumberOfRows(j , 0x0000ff00); // FF3
      Drivers[2].lightNumberOfRows(j , 0x0000ff00); // FF2
      Drivers[3].lightNumberOfRows(j , 0x0000ff00); // FF1
      delay(300);
    }
    for (int j = 0; j <=6; j++) {
      Drivers[1].lightNumberOfRows(j , 0x00000000); // FF3
      Drivers[2].lightNumberOfRows(j , 0x00000000); // FF2
      Drivers[3].lightNumberOfRows(j , 0x00000000); // FF1
      // delay(300);
    }
    break;
  case 4:
    // Injection Intimation
    for (int j = 0; j <=6; j++) {
      Drivers[3].setBlockColour(0,0,255);
      delay(100);
      Drivers[3].setBlockColour(0,0,0);
      delay(100);
    }
    for (int j = 0; j <=6; j++) {
      Drivers[2].setBlockColour(0,0,255);
      delay(100);
      Drivers[2].setBlockColour(0,0,0);
      delay(100);
    }
    for (int j = 0; j <=6; j++) {
      Drivers[1].setBlockColour(0,0,255);
      delay(100);
      Drivers[1].setBlockColour(0,0,0);
      delay(100);
    }
    break;
  default:
    break;
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

void flashBlue() {
  for (int i = 0; i < 4; i++) {
    Drivers[i].setBlockColour(0,255,0);         // Change to red after figuring out the setting with Yawar
  }
  delay(100);
  for (int i = 0; i < 4; i++) {
    Drivers[i].setBlockColour(0,0,0);         // Change to red after figuring out the setting with Yawar
  }
  delay(100);
}


