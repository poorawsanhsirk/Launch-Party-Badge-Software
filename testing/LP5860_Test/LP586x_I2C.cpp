/*
 * LP586x_I2C.cpp - Library implementation for interfacing with LP586x devices using I2C
 * Created by [Your Name], [Current Date]
 * Released into the public domain.
 */

#include "LP586x_I2C.h"

#include <stdint.h>
#include <stdbool.h>


LP586X_I2C::LP586X_I2C(uint8_t slaveAddress, uint8_t enablePin, uint8_t syncPin) {
    _enablePin = enablePin;
    // _ifsPin = ifsPin;
    _syncPin = syncPin;
    _currentSlaveAddr = 0;
    _slaveAddr5bit = slaveAddress;
}

void LP586X_I2C::interfaceInit(uint8_t sdaPin, uint8_t sclPin) {
    // Initialize I2C communication
    // Wire.initPins(5,4);
    Wire.begin(sdaPin,sclPin);
    Wire.setClock(I2C_FREQUENCY);
    
    // Configure pins
    pinMode(_enablePin, OUTPUT);
    // pinMode(_ifsPin, OUTPUT);
    pinMode(_syncPin, OUTPUT);
    
    // Set initial pin states
    digitalWrite(_syncPin, LOW);
    digitalWrite(_enablePin, LOW);  // Start with device disabled
    delay(100);
    digitalWrite(_enablePin, HIGH);  // Start with device disabled
    // digitalWrite(_ifsPin, LOW);     // Default to I2C mode
    // delay(1);
}


void LP586X_I2C::writeRegister(uint16_t regAddr10bit, uint8_t data) {
    // Form Address Byte-1
    _currentSlaveAddr = (_slaveAddr5bit << 2) + (regAddr10bit >> 8);
    // Form Address Byte-2
    uint8_t regAddr8bit = regAddr10bit & 0xFF;
    
    // Start transmission to the current slave address
    Wire.beginTransmission(_currentSlaveAddr);
    
    // Send register address
    Wire.write(regAddr8bit);
    
    // Send data
    Wire.write(data);
    
    // End transmission
    Wire.endTransmission();
    
    // Small delay for device processing
    delayMicroseconds(5);
}

uint8_t LP586X_I2C::readRegister(uint16_t regAddr10bit) {
    uint8_t regAddr8bit = regAddr10bit & 0xFF;
    uint8_t value = 0;
    
    // Write the register address first
    Wire.beginTransmission(_currentSlaveAddr);
    Wire.write(regAddr8bit);
    Wire.endTransmission(false);
    
    // Request 2 bytes from the slave device
    Wire.requestFrom(_currentSlaveAddr, 1);
    
    // Check if 2 bytes are available
    if (Wire.available()) {
        // Read MSB first
        value = Wire.read();
    }
    
    return value;
}

void LP586X_I2C::disableDevice() {
    digitalWrite(_enablePin, LOW);
}

void LP586X_I2C::enableDevice() {
    digitalWrite(_enablePin, HIGH);
}

void LP586X_I2C::initializeSync() {
    digitalWrite(_syncPin, LOW);
}

void LP586X_I2C::sendSync() {
    digitalWrite(_syncPin, HIGH);
    delayMicroseconds(200);
    digitalWrite(_syncPin, LOW);
}

// Software Disable the LED Matrix Driver
void LP586X_I2C::chip_Standby() {

}

// Software Enable the LED Matrix Driver
void LP586X_I2C::chip_Normal() {

}

// Set Data Refresh Mode
/**
 * Mode-1: LED updates as soon as register is written to
 * Mode-2: LED Updates when VSYNC is strobed. 8-bit PWM
 * Mode-3: LED Updates when VSYNC is strobed. 16-bit PWM
 */
void LP586X_I2C::setDataRefreshMode(uint8_t Data_Ref_Mode) {

}


// void LP586X_I2C::setDeviceInitialRegister(uint8_t Max_Line, uint8_t Data_Ref_Mode, uint8_t PWM_Frequency) {

// }

// void LP586X_I2C::setDeviceConfigReg1() {

// }

// void LP586X_I2C::setDeviceConfigReg2() {

// }

// void LP586X_I2C::setDeviceConfigReg3() {

// }
