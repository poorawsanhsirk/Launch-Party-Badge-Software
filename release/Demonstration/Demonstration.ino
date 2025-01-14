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

// void LaunchTimer() {
  

//   volatile float currentSeconds = timeinfo.tm_hour*3600 + timeinfo.tm_min*60 + timeinfo.tm_sec;
//   volatile float targetSecondsToLaunch = LAUNCH_TARGET_HOUR*3600 + LAUNCH_TARGET_MIN*60 + LAUNCH_TARGET_SEC;
//   // int LaunchDone = 0;

//   if(targetSecondsToLaunch <= currentSeconds){
//     targetSecondsToLaunch += 24*3600;         // Target is tomorrow
//   }

//   volatile float timeRemainingToLaunch = targetSecondsToLaunch - currentSeconds;

//   /**
//    * State Transition Logic
//    */
//   int buttonPress = digitalRead(9);
//   if (buttonPress == 0) {
//     delay(50);
//     buttonPress = digitalRead(9);
//     if (buttonPress == 0) {
//       menuState = 2;
//     }
//   }


  
//   /**
//    * TODO: Need to figure out the Launch Transition Event Handling properly. Spaghetti Logic at the moment
//    */
//   if (timeRemainingToLaunch <= INTIMATION_TIME && timeRemainingToLaunch > 1) {
//     flashGreen();
//     Launch_Done = 1;
//     return;
//   } else if (timeRemainingToLaunch <= 0) {
//     Launch_Done = 1;
//   } else if (Launch_Done == 1){
//     for (int i = 0 ; i < 4 ; i++) {
//       Drivers[i].setBlockColour(0, 0, 255);
//     }
//     return;
//   } else {
//     // volatile float progress = (((float)timeRemainingToLaunchSincePowerON - (float)timeRemainingToLaunch)/(float)timeRemainingToLaunchSincePowerON);
//     volatile float progress = (timeRemainingToLaunchSincePowerON - timeRemainingToLaunch)/timeRemainingToLaunchSincePowerON;
//     Serial.print(progress);
//     if (progress < 0) {
//       flashGreen();
//       return;
//     }
//     int numberOfRowsLIT = (int)(progress*24) + 1;             // Show atleast one row to start
//     if (numberOfRowsLIT <= 6) {                               // Addressing only 1 Driver
//       for (int i = 0; i <=numberOfRowsLIT; i++) {
//         Drivers[0].lightNumberOfRows(i , 0x0000ff00);
//         delay(100);
//       }
//       delay(2000);                                            
//       for (int i = 0; i <=numberOfRowsLIT; i++) {
//         Drivers[0].lightNumberOfRows(i , 0x00000000);
//         delay(10);
//       }
//       delay(1000);
//       return;
      
//     } else if (numberOfRowsLIT >6 && numberOfRowsLIT <=12) {  // Addressing 2nd Matrix. Fill all rows on previous Drivers
//       for (int i = 0; i <=6; i++) {
//         Drivers[0].lightNumberOfRows(i , 0x0000ff00);
//         delay(100);
//       }
//       for (int i = 0; i <=numberOfRowsLIT-6; i++) {
//         Drivers[1].lightNumberOfRows(i , 0x0000ff00);
//         delay(100);
//       }
//       delay(2000);
//       for (int i = 0; i <=6; i++) {
//         Drivers[0].lightNumberOfRows(i , 0x00000000);
//         // delay(100);
//       }
//       for (int i = 0; i <=numberOfRowsLIT-6; i++) {
//         Drivers[1].lightNumberOfRows(i , 0x00000000);
//         // delay(100);
//       }
//       delay(1000);
//       return;
//     } else if (numberOfRowsLIT >12 && numberOfRowsLIT <=18) {
//       for (int i = 0; i <=6; i++) {
//         Drivers[0].lightNumberOfRows(i , 0x0000ff00);
//         delay(100);
//       }
//       for (int i = 0; i <=6; i++) {
//         Drivers[1].lightNumberOfRows(i , 0x0000ff00);
//         delay(100);
//       }
//       for (int i = 0; i <=numberOfRowsLIT-12; i++) {
//         Drivers[2].lightNumberOfRows(i , 0x0000ff00);
//         delay(100);
//       }
//       delay(2000);
//       for (int i = 0; i <=6; i++) {
//         Drivers[0].lightNumberOfRows(i , 0x00000000);
//         Drivers[1].lightNumberOfRows(i , 0x00000000);
//         // delay(100);
//       }
//       for (int i = 0; i <=numberOfRowsLIT-12; i++) {
//         Drivers[2].lightNumberOfRows(i , 0x00000000);
//         // delay(100);
//       }
//       delay(1000);
//       return;
//     } else if (numberOfRowsLIT >18 && numberOfRowsLIT <=24) {
//       for (int i = 0; i <=6; i++) {
//         Drivers[0].lightNumberOfRows(i , 0x0000ff00);
//         delay(100);
//       }
//       for (int i = 0; i <=6; i++) {
//         Drivers[1].lightNumberOfRows(i , 0x0000ff00);
//         delay(100);
//       }
//       for (int i = 0; i <=6; i++) {
//         Drivers[2].lightNumberOfRows(i , 0x0000ff00);
//         delay(100);
//       }
//       for (int i = 0; i <=numberOfRowsLIT-18; i++) {
//         Drivers[3].lightNumberOfRows(i , 0x0000ff00);
//         delay(100);
//       }
//       delay(2000);
//       for (int i = 0; i <=6; i++) {
//         Drivers[0].lightNumberOfRows(i , 0x00000000);
//         Drivers[1].lightNumberOfRows(i , 0x00000000);
//         Drivers[2].lightNumberOfRows(i , 0x00000000);
//         // delay(100);
//       }
//       for (int i = 0; i <=numberOfRowsLIT-12; i++) {
//         Drivers[3].lightNumberOfRows(i , 0x00000000);
//         // delay(100);
//       }
//       delay(1000);
//       return;
//     }
//   }

// }

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

// void transitionAnimation() {
//   while(digitalRead(9) == 0) {
//     for (int i = 0; i < 4; i++) {
//       Drivers[i].setBlockColour(0,255,0);         // Change to red after figuring out the setting with Yawar
//     }
//     delay(100);
//     for (int i = 0; i < 4; i++) {
//       Drivers[i].setBlockColour(0,0,0);         // Change to red after figuring out the setting with Yawar
//     }
//     delay(100);
//   }

//   // One more flash after button is released
//   for (int i = 0; i < 4; i++) {
//     Drivers[i].setBlockColour(0,255,0);         // Change to red after figuring out the setting with Yawar
//   }
//   delay(200);
//   for (int i = 0; i < 4; i++) {
//     Drivers[i].setBlockColour(0,0,0);         // Change to red after figuring out the setting with Yawar
//   }
//   delay(200);

//   switch (previousMenuState)
//   {
//   case LAUNCH_TIMER:
//     menuState = INJECTION_TIMERS;
//     previousMenuState = menuState;
//     break;
  
//   case INJECTION_TIMERS:
//     menuState = LAUNCH_TIMER;
//     previousMenuState = menuState;
//     break;
  
//   default:
//     break;
//   }
// }


// static bool FF1InjectionDone = 0;
// static bool FF2InjectionDone = 0;
// static bool FF3InjectionDone = 0;


// void InjectionTimers() {

//   /** /////////////////////////////////////////////////////////////////////////////
//    * State Transition Logic
//    */ /////////////////////////////////////////////////////////////////////////////
//   int buttonPress = digitalRead(9);
//   if (buttonPress == 0) {
//     delay(50);
//     buttonPress = digitalRead(9);
//     if (buttonPress == 0) {
//       menuState = 2;
//     }
//   }
//   // Very crude Debouncing. Change later if this code triggers anyone


//   volatile float currentSeconds = timeinfo.tm_hour*3600 + timeinfo.tm_min*60 + timeinfo.tm_sec;
//   static float LaunchTimeInSeconds = LAUNCH_TARGET_HOUR*3600 + LAUNCH_TARGET_MIN*60 + LAUNCH_TARGET_SEC;

  

//   // FF1 Injection Parameters
//   volatile float FF1TargetSecondsToSeparate = FF1_INJECTION_TARGET_HOUR*3600 + FF1_INJECTION_TARGET_MIN*60 + FF1_INJECTION_TARGET_SEC;
//   if(FF1TargetSecondsToSeparate <= currentSeconds){
//     FF1TargetSecondsToSeparate += 24*3600;         // Target is tomorrow
//   }

//   // FF2 Injection Parameters
//   volatile float FF2TargetSecondsToSeparate = FF2_INJECTION_TARGET_HOUR*3600 + FF2_INJECTION_TARGET_MIN*60 + FF2_INJECTION_TARGET_SEC;
//   if(FF2TargetSecondsToSeparate <= currentSeconds){
//     FF2TargetSecondsToSeparate += 24*3600;         // Target is tomorrow
//   }

//   // FF3 Injection Parameters
//   volatile float FF3TargetSecondsToSeparate = FF3_INJECTION_TARGET_HOUR*3600 + FF3_INJECTION_TARGET_MIN*60 + FF3_INJECTION_TARGET_SEC;
//   if(FF3TargetSecondsToSeparate <= currentSeconds){
//     FF3TargetSecondsToSeparate += 24*3600;         // Target is tomorrow
//   }

  

//   // Check if Launch is done
//   if (Launch_Done == 0) {
//     // Display some other colour on all Drivers till Launch is done
//     for (int i = 0 ; i < 4 ; i++) {
//       Drivers[i].setBlockColour(0, 100, 80);
//     }
//     delay(200);
//     for (int i = 0 ; i < 4 ; i++) {
//       Drivers[i].setBlockColour(0, 0, 0);
//     }
//   } else if (Launch_Done == 1) {
//     // Start Injection Progress View after Launch_Done is set by LaunchTimer() or some logic in loop()

//     // FF1 Display
//     volatile float timeRemainingToInjectFF1 = FF1TargetSecondsToSeparate - currentSeconds;
//     if (timeRemainingToInjectFF1 <= INJECTION_INTIMATION_TIME && timeRemainingToInjectFF1 > 1) {    // Indicator for the FF1 Injection Alert
//       // Flash on Driver[3]
//       Drivers[3].setBlockColour(0,0,255);
//       delay(100);
//       Drivers[3].setBlockColour(0,0,0);
//       delay(100);
//     } else if (timeRemainingToInjectFF1 == 0) {
//       FF1InjectionDone = 1;
//     } else if (FF1InjectionDone == 1) {
//       Drivers[3].setBlockColour(0,100,80);
//     } else {
//       volatile float progressFF1 = ((FF1TargetSecondsToSeparate - LaunchTimeInSeconds) - timeRemainingToInjectFF1)/ (FF1TargetSecondsToSeparate - LaunchTimeInSeconds);
//       if (progressFF1 < 0) {
//         Drivers[3].setBlockColour(0, 0, 30);                                                       // Change to Red later
//         return;
//       }
//       uint8_t numberOfRowsToLightFF1 = (uint8_t)(progressFF1 * 6) + 1;
//       Drivers[3].lightNumberOfRows(numberOfRowsToLightFF1, 0x0000ff00);
//     }

//     // FF2 Display
//     volatile float timeRemainingToInjectFF2 = FF2TargetSecondsToSeparate - currentSeconds;
//     if (timeRemainingToInjectFF2 <= INJECTION_INTIMATION_TIME && timeRemainingToInjectFF2 > 1) {    // Indicator for the FF1 Injection Alert
//       // Flash on Driver[2]
//       Drivers[2].setBlockColour(0,0,255);
//       delay(100);
//       Drivers[2].setBlockColour(0,0,0);
//       delay(100);
//     } else if (timeRemainingToInjectFF2 == 0) {
//       FF2InjectionDone = 1;
//     } else if (FF2InjectionDone == 1) {
//       Drivers[2].setBlockColour(0,100,80);
//     } else {
//       volatile float progressFF2 = ((FF2TargetSecondsToSeparate - LaunchTimeInSeconds) - timeRemainingToInjectFF2)/ (FF2TargetSecondsToSeparate - LaunchTimeInSeconds);
//       if (progressFF2 < 0) {
//         Drivers[2].setBlockColour(0, 0, 30);                                                       // Change to Red later
//         return;
//       }
//       uint8_t numberOfRowsToLightFF2 = (uint8_t)(progressFF2 * 6) + 1;
//       Drivers[2].lightNumberOfRows(numberOfRowsToLightFF2, 0x0000ff00);
//     }

//     // FF3 Display
//     volatile float timeRemainingToInjectFF3 = FF3TargetSecondsToSeparate - currentSeconds;
//     if (timeRemainingToInjectFF3 <= INJECTION_INTIMATION_TIME && timeRemainingToInjectFF3 > 1) {    // Indicator for the FF1 Injection Alert
//       // Flash on Driver[1]
//       Drivers[1].setBlockColour(0,0,255);
//       delay(100);
//       Drivers[1].setBlockColour(0,0,0);
//       delay(100);
//     } else if (timeRemainingToInjectFF3 == 0) {
//       FF3InjectionDone = 1;
//     } else if (FF3InjectionDone == 1) {
//       Drivers[1].setBlockColour(0,100,80);
//     } else {
//       volatile float progressFF3 = ((FF3TargetSecondsToSeparate - LaunchTimeInSeconds) - timeRemainingToInjectFF3)/ (FF3TargetSecondsToSeparate - LaunchTimeInSeconds);
//       if (progressFF3 < 0) {
//         Drivers[1].setBlockColour(0, 0, 30);                                                       // Change to Red later
//         return;
//       }
//       uint8_t numberOfRowsToLightFF3 = (uint8_t)(progressFF3 * 6) + 1;
//       Drivers[1].lightNumberOfRows(numberOfRowsToLightFF3, 0x0000ff00);
//     }
//   }
// }
