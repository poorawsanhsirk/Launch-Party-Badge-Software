/*
 * LP586x_I2C.cpp - Library implementation for interfacing with LP586x devices using I2C
 * Created by [Your Name], [Current Date]
 * Released into the public domain.
 */

#include "LP586x_I2C.h"

#include <stdint.h>
#include <stdbool.h>


LP586X_I2C::LP586X_I2C(uint8_t enablePin, uint8_t syncPin) {
    _enablePin = enablePin;
    // _ifsPin = ifsPin;
    _syncPin = syncPin;
    _currentSlaveAddr = 0;
}

void LP586X_I2C::begin(uint8_t sdaPin, uint8_t sclPin) {
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

void LP586X_I2C::setSlave(uint8_t slaveAddr5bit, uint16_t regAddr10bit) {
    // Convert 5-bit slave address to 8-bit I2C format
    // Upper bits of regAddr10bit are included in slave address
    _currentSlaveAddr = (slaveAddr5bit << 2) + (regAddr10bit >> 8);
}

void LP586X_I2C::writeRegister(uint16_t regAddr10bit, uint8_t data) {
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

// void LP586X_I2C::setInterfaceI2C() {
//     digitalWrite(_ifsPin, LOW);
// }

// void LP586X_I2C::setInterfaceSPI() {
//     digitalWrite(_ifsPin, HIGH);
// }

void LP586X_I2C::initializeSync() {
    digitalWrite(_syncPin, LOW);
}

void LP586X_I2C::sendSync() {
    digitalWrite(_syncPin, HIGH);
    delayMicroseconds(200);
    digitalWrite(_syncPin, LOW);
}