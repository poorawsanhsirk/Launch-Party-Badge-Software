#include "LP586x_I2C.h"

#include "Wire.h"
#include <stdint.h>

#define SLAVE_ADDRESS 0x10

#define ENABLE_PIN 0x01
#define SYNC_PIN 0x00

#define sdaPin 0x05
#define sclPin 0x04



LP586X_I2C test_driver(SLAVE_ADDRESS, ENABLE_PIN, SYNC_PIN);

void setup() {
    Serial.begin(115200);
    test_driver.interfaceInit(sdaPin, sclPin);

    // Resetting the IC
    test_driver.disableDevice();
    delay(10);
    test_driver.enableDevice();
    delay(10);

    // Writing to a register and software enabling the IC
    test_driver.writeRegister(Chip_Enable_Register, 0x01);

    uint8_t data = test_driver.readRegister(Chip_Enable_Register);
    Serial.print("Register Value: ");
    Serial.print(data);

    // Reading Default Value of Device_Initial_Register
    data =test_driver.readRegister(Device_Initial_Register);
    Serial.print("Register Value: ");
    Serial.print(data);
}

void loop() {
    // Reading Default Value of Device_Initial_Register
    int data =test_driver.readRegister(Device_Initial_Register);
    Serial.print("Register Value: ");
    Serial.print(data);
    delay(1000);
}