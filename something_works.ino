#include "LED_Animation.h"
#include "LP586x.h"
#include "Wire.h"
#include <stdint.h>

#define SLAVE_ADDRESS_LP_1 0x20
// #define ENABLE_PIN_LP_1 0x01
// #define IFS_PIN_LP_1 0x03
// #define SYNC_PIN_LP_1 0x00

#define SLAVE_ADDRESS_LP_2 0x11
// #define ENABLE_PIN_LP_2 0x11
// #define IFS_PIN_LP_2 0x11
// #define SYNC_PIN_LP_2 0x11

#define SLAVE_ADDRESS_LP_3 0x12
// #define ENABLE_PIN_LP_3 0x12
// #define IFS_PIN_LP_3 0x12
// #define SYNC_PIN_LP_3 0x12

#define SLAVE_ADDRESS_LP_4 0x13
// #define ENABLE_PIN_LP_4 0x13
// #define IFS_PIN_LP_4 0x13
// #define SYNC_PIN_LP_4 0x13

#define ENABLE_PIN 0x01
#define SYNC_PIN 0x00



void setup() {
  
  Serial.begin(115200);
  Wire.begin(5, 4); // Initialize I²C with SDA on GPIO 5, SCL on GPIO 4

  pinMode(1, OUTPUT); // VIO_EN
  pinMode(0, OUTPUT); // VSYNC
  digitalWrite(1, LOW);
  delay(100);
  digitalWrite(1, HIGH);

  Wire.beginTransmission(0x40); // Start transmission to slave with address 0x40
  Wire.write(0x00);             // Write the register address (0x00)
  Wire.write(0x00);             // Write data to the register
  Wire.endTransmission();       // End transmission

  // Write to the device to Enable Chip
  Wire.beginTransmission(0x40); // Start transmission to slave with address 0x40
  Wire.write(0x00);             // Write the register address (0x00)
  Wire.write(0x01);             // Write data to the register
  Wire.endTransmission();       // End transmission

  delay(100);

  // Read from the device
  Wire.beginTransmission(0x40); // Start transmission to slave with address 0x40
  Wire.write(0x00);             // Point to the register address (0x01) to read

  int err = Wire.endTransmission(false);
  Serial.print("Error Value: ");
  Serial.println(err);

  Wire.requestFrom(0x40, 1);    // Request 1 byte of data from the slave
  if (Wire.available()) {       // Check if data is available
    int registerValue = Wire.read(); // Read the data
    Serial.print("Register Value: ");
    Serial.println(registerValue);
  }

  
  // Light up one LED (L0-CS0)
  // Enter Mode-1
  Wire.beginTransmission(0x40); // Start transmission to slave with address 0x40
  Wire.write(0x01);             // Write the register address (0x01)
  uint8_t data = (0xB<<3) + (0x0<<1) + 0x0; // Setting Mode to Mode 1
  Wire.write(data);             // Write data to the register
  Wire.endTransmission();       // End transmission

  // Read from the device to validate if Mode has changed
  Wire.beginTransmission(0x40); // Start transmission to slave with address 0x40
  Wire.write(0x01);             // Point to the register address (0x01) to read

  Wire.endTransmission(false);
  Wire.requestFrom(0x40, 1);    // Request 1 byte of data from the slave
  if (Wire.available()) {       // Check if data is available
    int registerValue = Wire.read(); // Read the data
    Serial.print("Register Value: ");
    Serial.println(registerValue);
  }

  // Set Individual 8-bit Dot Current Setting
  Wire.beginTransmission(0x41); // Start transmission to slave with address 0x40
  Wire.write(0x02);             // Write the register address (0x00)
  data = 0xFF; // Setting Mode to Mode 1
  Wire.write(data);             // Write data to the register
  Wire.endTransmission();       // End transmission

  // Set PWM Based Brightness
  // for(int i = 0x0; i <= 0xFF; i=i+5){
    Wire.beginTransmission(0x42);
    Wire.write(0x02);
    // data = i;
    // Wire.write(data);
    Wire.write(0xFF);
    Wire.endTransmission();
  // }
}

void loop() {
  // Set PWM Based Brightness
  // for(int i = 0x0; i <= 0xFF; i=i+5){
  //   Wire.beginTransmission(0x42);
  //   Wire.write(0x02);
  //   uint8_t data = i;
  //   Wire.write(data);
  //   Wire.endTransmission();
  //   delay(10);
  // }
  digitalWrite(0, HIGH);
  delay(100);
  digitalWrite(0, LOW);
  delay(100);
}
