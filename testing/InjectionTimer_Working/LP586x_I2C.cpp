/*
 * LP586x_I2C.cpp - Library implementation for interfacing with LP586x devices using I2C
 * Created by Krishna Swaroop, 12/01/2025
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
    // uint8_t addrByte1 = _slaveAddr5bit << 3 && 0x00;
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
  writeRegister(0x000, 0x00);
}

// Software Enable the LED Matrix Driver
void LP586X_I2C::chip_Normal() {
    writeRegister(0x000, 0x01);
}

// Set Data Refresh Mode
/**
 * Mode-1: LED updates as soon as register is written to 
 * Mode-2: LED Updates when VSYNC is strobed. 8-bit PWM
 * Mode-3: LED Updates when VSYNC is strobed. 16-bit PWM
 // defaults to 62.5khz PWM (PWM_Freq = 1)
 */
void LP586X_I2C::setDataRefreshMode(uint8_t Data_Ref_Mode) {

  //D6-D3 for line selected, D2-D1 for data-ref-mode, D0 for PWM_freq (defaulted to 1)

  uint8_t data = 0xB << 3 + (Data_Ref_Mode << 1) + 0x0 ;

    writeRegister(Device_Initial_Register, data);
}

// Analog Current Dimming Functions







// PWM Based Dimming Functions

// Set Global PWM Based Brightness
void LP586X_I2C::setGlobalBrightness(uint8_t PWM_Global) {
  writeRegister(Master_Brightness_Register, PWM_Global);
}


// turn on/off individual LED
void LP586X_I2C::setIndLED(uint8_t row, uint8_t col){
  //D6-D3 for line selected, D2-D1 for data-ref-mode, D0 for PWM_freq (defaulted to 1)
  
  writeRegister(LED_Dot_Onoff_Register_start, 0xFF);
}

// Set Entire 66 LED Frame

void LP586X_I2C::setFrame(const uint32_t* Byte_Array) {
    uint8_t R_PWM_Value = 0;
    uint8_t G_PWM_Value = 0;
    uint8_t B_PWM_Value = 0;

    for (uint8_t i = 0; i < 66; i++) {
        // Extract RGB values from the 32-bit packed value
        uint32_t colour = Byte_Array[i];  // Assuming Byte_Array[i] holds the full RGB value in uint32_t
        R_PWM_Value = (colour >> 16) & 0xFF;  // Extract Red (8 most significant bits)
        G_PWM_Value = (colour >> 8) & 0xFF;   // Extract Green (8 middle bits)
        B_PWM_Value = colour & 0xFF;          // Extract Blue (8 least significant bits)

        // Write the color values to the corresponding registers
        writeRegister((LED_Dot_Brightness_Register_Start + (3 * i)), R_PWM_Value);
        writeRegister((LED_Dot_Brightness_Register_Start + (3 * i) + 1), G_PWM_Value);
        writeRegister((LED_Dot_Brightness_Register_Start + (3 * i) + 2), B_PWM_Value);

        // Optional: Add a delay between writes
        delayMicroseconds(100);
    }
}

void LP586X_I2C::setBlockColour(uint8_t RED, uint8_t GREEN, uint8_t BLUE) {
    // uint8_t R_PWM_Value = 0;
    // uint8_t G_PWM_Value = 0;
    // uint8_t B_PWM_Value = 100;

    for (uint8_t i = 0; i <66; i++) {
      // R_PWM_Value = Byte_Array[i][0];
      // G_PWM_Value = Byte_Array[i][1];
      // B_PWM_Value = Byte_Array[i][2];
      writeRegister((LED_Dot_Brightness_Register_Start+ (3*i)),     RED);
      writeRegister((LED_Dot_Brightness_Register_Start+ (3*i) + 1), GREEN);
      writeRegister((LED_Dot_Brightness_Register_Start+ (3*i) + 2), BLUE);
      delayMicroseconds(100);
    }
};

void LP586X_I2C::lightNumberOfRows(uint8_t numberOfRows,unsigned long colour) {
  uint8_t R_PWM_Value = (colour >> 16) & 0xFF;
  uint8_t G_PWM_Value = (colour >> 8) & 0xFF;
  uint8_t B_PWM_Value = colour & 0xFF;

  for (uint8_t i = 0; i < numberOfRows; i++) {
      // Write the color values to the corresponding registers
      writeRegister((LED_Dot_Brightness_Register_Start + (3 * i)), R_PWM_Value);
      writeRegister((LED_Dot_Brightness_Register_Start + (3 * i) + 1), G_PWM_Value);
      writeRegister((LED_Dot_Brightness_Register_Start + (3 * i) + 2), B_PWM_Value);

      // Optional: Add a delay between writes
      delayMicroseconds(100);
  }
}



void LP586X_I2C::setDeviceInitialRegister(uint8_t Max_Line, uint8_t Data_Ref_Mode, uint8_t PWM_Frequency) {
  //D6-D3 for line selected, D2-D1 for data-ref-mode, D0 for PWM_freq (defaulted to 1)
  uint8_t data = Max_Line << 3 + (Data_Ref_Mode << 1) + PWM_Frequency ;
  writeRegister(Device_Initial_Register, data);
}

void LP586X_I2C::setDeviceConfigReg1(uint8_t SW_BLK, uint8_t PWM_Scale_Mode, uint8_t PWM_Phase_Shift, uint8_t CS_ON_Shift) {
  uint8_t data = SW_BLK << 3 + (PWM_Scale_Mode << 2) + PWM_Phase_Shift << 1 + CS_ON_Shift;
  writeRegister(Device_Configuration_1_Register, data);
}

void LP586X_I2C::setDeviceConfigReg2(uint8_t Comp_Group3, uint8_t Comp_Group2, uint8_t Comp_Group1, uint8_t LOD_removal, uint8_t LSD_removal) {
  uint8_t data = Comp_Group3 << 6 + (Comp_Group2 << 4) + Comp_Group1 << 2 + LOD_removal << 1 + LSD_removal;
  writeRegister(Device_Configuration_2_Register, data);
}

void LP586X_I2C::setDeviceConfigReg3(uint8_t Down_Deghost, uint8_t Up_Deghost, uint8_t Maximum_Current, uint8_t Up_Deghost_enable) {
  uint8_t data = Down_Deghost << 6 + (Up_Deghost << 4) + Maximum_Current << 1 + Up_Deghost_enable;
  writeRegister(Device_Configuration_2_Register, data);
}


        /*
        Set Red LED current 
        @default - 0x40
        */
void LP586X_I2C::R_current_set(uint8_t CC_Group1){
  writeRegister(Color_Group_R_Current_Register, CC_Group1);
}


        /*
        Set Green LED current
        @default - 0x40
        */
void LP586X_I2C::G_current_set(uint8_t CC_Group2){
  writeRegister(Color_Group_G_Current_Register, CC_Group2);
}


        /*
        Set Blue LED current 
        @default - 0x40
        */
void LP586X_I2C::B_current_set(uint8_t CC_Group3){
  writeRegister(Color_Group_B_Current_Register, CC_Group3);
}



