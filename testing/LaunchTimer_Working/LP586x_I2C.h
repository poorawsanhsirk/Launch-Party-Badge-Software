/*
 * LP586x_I2C.h - Library for interfacing with LP586x devices using I2C
 * Created by Krishna Swaroop, 12/01/2025
 * Released into the public domain.
 */

#ifndef LP586X_I2C_H
#define LP586X_I2C_H

#include <Arduino.h>
#include <Wire.h>

#define I2C_FREQUENCY  400000

/***************************************************************************//**
 *Register Dev_initial Value
 ******************************************************************************/
#define Max_Line_11                        0xB //default
#define Max_Line_10                        0xA
#define Max_Line_9                         0x9
#define Max_Line_8                         0x8
#define Max_Line_7                         0x7
#define Max_Line_6                         0x6
#define Max_Line_5                         0x5
#define Max_Line_4                         0x4
#define Max_Line_3                         0x3
#define Max_Line_2                         0x2
#define Max_Line_1                         0x1

#define Mode_3_1                           0x3 //default
#define Mode_3                             0x2
#define Mode_2                             0x1
#define Mode_1                             0x0

#define PWM_Frequency_62_5k                0x1
#define PWM_Frequency_125k                 0x0 //default

/***************************************************************************//**
 *Register Dev_config1 Value
 ******************************************************************************/
#define SW_BLK_05us                        0x1
#define SW_BLK_1us                         0x0 //default

#define PWM_Scale_Exponential              0x1
#define PWM_Scale_Linear                   0x0 //default

#define PWM_PhaseShift_on                  0x1
#define PWM_PhaseShift_off                 0x0 //default

#define CS_Delay_on                        0x1
#define CS_Delay_off                       0x0 //default

/***************************************************************************//**
 *Register Dev_config2 Value
 ******************************************************************************/
#define Comp_Group3_3clock                 0x3
#define Comp_Group3_2clock                 0x2
#define Comp_Group3_1clock                 0x1
#define Comp_Group3_Off                    0x0 //default

#define Comp_Group2_3clock                 0x3
#define Comp_Group2_2clock                 0x2
#define Comp_Group2_1clock                 0x1
#define Comp_Group2_Off                    0x0 //default

#define Comp_Group1_3clock                 0x3
#define Comp_Group1_2clock                 0x2
#define Comp_Group1_1clock                 0x1
#define Comp_Group1_Off                    0x0 //default

#define LOD_Removal_En                     0x1
#define LOD_Removal_Off                    0x0 //default

#define LSD_Removal_En                     0x1
#define LSD_Removal_Off                    0x0 //default

/***************************************************************************//**
 *Register Dev_config3 Value
 ******************************************************************************/
#define Down_Deghost_Strong                0x3
#define Down_Deghost_Medium                0x2
#define Down_Deghost_Weak                  0x1 //default
#define Down_Deghost_off                   0x0

#define Up_Deghost_GND                     0x3
#define Up_Deghost_3                       0x2
#define Up_Deghost_2_5                     0x1
#define Up_Deghost_2                       0x0 //default

#define Maximum_Current_50                 0x7
#define Maximum_Current_40                 0x6
#define Maximum_Current_30                 0x5
#define Maximum_Current_20                 0x4
#define Maximum_Current_15                 0x3 //default
#define Maximum_Current_10                 0x2
#define Maximum_Current_5                  0x1
#define Maximum_Current_3                  0x0

#define Up_Deghost_En                      0x1 //default
#define Up_Deghost_Off                     0x0

/***************************************************************************//**
 *PWM Value
 ******************************************************************************/
#define Group3                             0x3
#define Group2                             0x2
#define Group1                             0x1
#define Group_None                         0x0 //default

/***************************************************************************//**
 *PWM Value
 ******************************************************************************/
#define PWM_Global_Max                     0xff
#define PWM_Group_Max                      0xff
#define PWM_Dot_Max                        0xff

/***************************************************************************//**
 *Slave Address
 ******************************************************************************/
/* Slave Address for I2C Slave */
#define SLAVE_ADDRESS  0x15 //slave address for Broadcast
#define SLAVE_ADDRESS1 0x10 //slave address for U1
#define SLAVE_ADDRESS2 0x11 //slave address for U2
#define SLAVE_ADDRESS3 0x12 //slave address for U3
#define SLAVE_ADDRESS4 0x13 //slave address for U4

/***************************************************************************//**
 *Register Address
 ******************************************************************************/
/* Device Configuration Register Address*/
#define Chip_Enable_Register               0x000
#define Device_Initial_Register            0x001
#define Device_Configuration_1_Register    0x002
#define Device_Configuration_2_Register    0x003
#define Device_Configuration_3_Register    0x004

#define LED_Group_Selection_Register_start 0x00C //Register address from 0x009 to 0x042
#define LED_Dot_Onoff_Register_start       0x043 //Register address from 0x043 to 0x063

/* Current Configuration Register Address */
#define Color_Group_R_Current_Register     0x009
#define Color_Group_G_Current_Register     0x00A
#define Color_Group_B_Current_Register     0x00B
#define LED_Dot_Current_Register_Start     0x100 //Register address from 0x100 to 0x1C5

/* Brightness Configuration Register Address */
#define Master_Brightness_Register         0x005
#define Group_1_Brightness_Register        0x006
#define Group_2_Brightness_Register        0x007
#define Group_3_Brightness_Register        0x008
#define LED_Dot_Brightness_Register_Start  0x200 //Register address from 0x200 to 0x2C5 (8-bit) Or to 0x38B (16-bit)

/* Fault Register Address*/
#define Fault_State_Register               0x064
#define LED_Dot_LOD_Register_start         0x065 //Register address from 0x065 to 0x085
#define LED_Dot_LSD_Register_start         0x086 //Register address from 0x086 to 0x0A6

/* Reset Register Address*/
#define LOD_Clear_Register                 0x0A7
#define LSD_Clear_Register                 0x0A8
#define Reset_Register                     0x0A9

/***************************************************************************//**
 *Register Value
 ******************************************************************************/
#define Chip_Disable                       0x00
#define Chip_Enable                        0x01

#define LOD_Clear_En                       0xff
#define LSD_Clear_En                       0xff
#define Reset_En                           0xff


class LP586X_I2C {
    private:
        uint8_t _enablePin;  // Pin to control device enable
        uint8_t _syncPin;    // Pin for sync signal
        uint8_t _currentSlaveAddr; // Store current slave address
        uint8_t _slaveAddr5bit;     // 5-bit Slave Address
        
    public:
        LP586X_I2C() {};
        /**
         * Constructor
         * @param enablePin Arduino pin connected to device enable
         * @param syncPin Arduino pin connected to sync signal
         */
        LP586X_I2C(uint8_t slaveAddress, uint8_t enablePin, uint8_t syncPin);
        
        /**
         * Initialize the I2C interface and pins
         */
        void interfaceInit(uint8_t sdaPin, uint8_t sclPin);
        
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



        void chip_Standby();



        void chip_Normal();



        void setDataRefreshMode(uint8_t Data_Ref_Mode);


        void setDeviceInitialRegister(uint8_t Max_Line, uint8_t Data_Ref_Mode, uint8_t PWM_Frequency);


        void setDeviceConfigReg1(uint8_t SW_BLK, uint8_t PWM_Scale_Mode, uint8_t PWM_Phase_Shift, uint8_t CS_ON_Shift); 

        void setDeviceConfigReg2(uint8_t Comp_Group3, uint8_t Comp_Group2, uint8_t Comp_Group1, uint8_t LOD_removal, uint8_t LSD_removal);

        void setDeviceConfigReg3(uint8_t Down_Deghost, uint8_t Up_Deghost, uint8_t Maximum_Current, uint8_t Up_Deghost_enable);

        void setGlobalBrightness(uint8_t PWM_Global); 

        /*
        Set Red LED current 
        @default - 0x40
        */
        void R_current_set(uint8_t CC_Group1);


        /*
        Set Green LED current
        @default - 0x40
        */
        void G_current_set(uint8_t CC_Group2);


        /*
        Set Blue LED current 
        @default - 0x40
        */
        void B_current_set(uint8_t CC_Group3);

        // turn on/off individual LED
        void setIndLED(uint8_t row, uint8_t col);

        void setFrame(const uint32_t* Byte_Array);
        void setBlockColour(uint8_t RED, uint8_t GREEN, uint8_t BLUE);
        void lightNumberOfRows(uint8_t numberOfRows,unsigned long colour);

};

#endif // LP586X_I2C_H