#include <stdint.h>
#include "LP586x.h"

#define SLAVE_ADDRESS1 0x10 //slave address for U1

/***************************************************************************//**
 *  I2C Pins
 ******************************************************************************/
#define sdaPin 0x05
#define sclPin 0x04

/***************************************************************************//**
 *  Aux Pins
 ******************************************************************************/
#define ENABLE_PIN 0x01
#define SYNC_PIN 0x00

LP586x Driver_1(SLAVE_ADDRESS1, sdaPin, sclPin, ENABLE_PIN, SYNC_PIN);

void setup() {
    Serial.begin(115200);

    // Checking Register Value after initialisation
    uint8_t data = Driver_1.LP586X_i2c_obj.readRegister(Chip_Enable_Register);
    Serial.print("Register Value: ");
    Serial.print(data); // Data needs to be 0x01

    // Disabling Chip
    Driver_1.Chip_Standby();
    // Checking Register Value after disabling
    data = Driver_1.LP586X_i2c_obj.readRegister(Chip_Enable_Register);
    Serial.print("Register Value: ");
    Serial.print(data); // Data needs to be 0x00

    // Enabling Chip again
    Driver_1.Chip_Normal();

    // Checking Register Value before changing
    data = Driver_1.LP586X_i2c_obj.readRegister(Device_Initial_Register);
    Serial.print("Register Value: ");
    Serial.print(data); // Data needs to be 0x5E

    // Changing Mode
    Driver_1.Device_Initial(Max_Line_11, Mode_1, PWM_Frequency_125k);
    // Checking Register Value after changing mode
    data = Driver_1.LP586X_i2c_obj.readRegister(Device_Initial_Register);
    Serial.print("Register Value: ");
    Serial.print(data); // Data needs to be 0x5E

}

void loop() {
    for (uint8_t i = 0; i < 255; i++)
    {
        /* code */
        // Lighting up all LEDs
        Driver_1.DC_Color_All(i, i, i);
        delay(10);
    }
    
}