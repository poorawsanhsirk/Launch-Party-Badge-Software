/*
 * LP586x_I2C.h - Library for interfacing with LP586x devices using I2C
 * Created by [Your Name], [Current Date]
 * Released into the public domain.
 */

#ifndef LP586X_I2C_H
#define LP586X_I2C_H

#include <Arduino.h>
#include <Wire.h>

#define I2C_FREQUENCY  400000

class LP586X_I2C {
    private:
        uint8_t _enablePin;  // Pin to control device enable
        // uint8_t _ifsPin;     // Pin to select between I2C and SPI
        uint8_t _syncPin;    // Pin for sync signal
        uint8_t _currentSlaveAddr; // Store current slave address
        
    public:
        LP586X_I2C() {};
        /**
         * Constructor
         * @param enablePin Arduino pin connected to device enable
         * @param syncPin Arduino pin connected to sync signal
         */
        LP586X_I2C(uint8_t enablePin, uint8_t syncPin);
        
        /**
         * Initialize the I2C interface and pins
         */
        void begin(uint8_t sdaPin, uint8_t sclPin);
        
        /**
         * Set the slave device address
         * @param slaveAddr5bit 5-bit slave address
         * @param regAddr10bit 10-bit register address
         */
        void setSlave(uint8_t slaveAddr5bit, uint16_t regAddr10bit);
        
        /**
         * Write data to a register
         * @param regAddr10bit 10-bit register address
         * @param data Byte to write
         */
        void writeRegister(uint16_t regAddr10bit, uint8_t data);
        
        /**
         * Read 16-bit data from a register
         * @param regAddr10bit 10-bit register address
         * @return 16-bit value read from register
         */
        uint8_t readRegister(uint16_t regAddr10bit);
        
        /**
         * Disable the device (pull enable pin low)
         */
        void disableDevice();
        
        /**
         * Enable the device (pull enable pin high)
         */
        void enableDevice();
        
        // /**
        //  * Set interface to I2C mode
        //  */
        // void setInterfaceI2C();
        
        // /**
        //  * Set interface to SPI mode
        //  */
        // void setInterfaceSPI();
        
        /**
         * Initialize sync pin
         */
        void initializeSync();
        
        /**
         * Send sync pulse
         */
        void sendSync();
};

#endif // LP586X_I2C_H