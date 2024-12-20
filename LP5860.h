/*
    @file LP58060.h
    @author Krishna Swaroop (krishna-swaroop)
    @version 0.1
    @date 2024-12-21
*/

#ifndef __LP5860_H
#define __LP5860_H

// Including Arduino Libraries
#include <Arduino.h>
#include <Wire.h>
#include <stdint.h>

class LP5860 {
private:
    TwoWire* wire;
    uint8_t deviceAddressbits;
    // Constants that might be needed
    T_CHIP_EN_US = 100;                 // Time to wait in microseconds after enabling the chip

    enum Registers: uint16_t {
        // Device Control Registers
        CHIP_EN = 0x000,                // Chip Enable Register. Default:0x00. 0x01 to Enable
        
        DEV_INITIAL = 0x001,            // Default = 0x5E; Reserved(7)|Max_Line_Num(6-3)|Data_Ref_Mode(2-1)|PWM_Fre(0)
        DEV_CONFIG1 = 0x002,            // Default = 0x00; Reserved(7-4)|SW_BLK(3)|PWM_Scale_Mode(2)|PWM_Phase_Shift(1)|CS_ON_Shift(0)
        DEV_CONFIG2 = 0x003,            // Default = 0x00; Comp_Group3(7-6)|Comp_Group2(5-4)|Comp_Group1(3-2)|LOD_removal(1):LSD_removal(0)
        DEV_CONFIG3 = 0x004,            // Default = 0x47; Down_Deghost(7-6)|Up_Deghost(5-4)|Maximum_Current(3-1)|Up_Deghost_enable(0)

        GLOBAL_BRIGHTNESS = 0x005,      // Default = 0xFF; PWM_Global(7-0)
        GROUP0_BRIGHTNESS = 0x006,      // Default = 0xFF; PWM_Group1(7-0)
        GROUP1_BRIGHTNESS = 0x007,      // Default = 0xFF; PWM_Group2(7-0)
        GROUP2_BRIGHTNESS = 0x008,      // Default = 0xFF; PWM_Group3(7-0)

        R_CURRENT = 0x009,              // Default = 0x40; Reserved(7)|CC_Group1(6-0)
        G_CURRENT = 0x00A,              // Default = 0x40; Reserved(7)|CC_Group2(6-0)
        B_CURRENT = 0x00B,              // Default = 0x40; Reserved(7)|CC_Group3(6-0)

        DOT_GROUP_SELECT_START = 0x00C,

        DOT_ONOFF_START = 0x043,

        FAULT_STATE = 0x064,            // Default = 0x00; Reserved(7-2)|Global_LOD(1)|Global_LSD(0)

        DOT_LOD_START = 0x065,

        DOT_LSD_START = 0x086,

        LOD_CLEAR = 0x0A7,

        LSD_CLEAR = 0x0A8,

        RESET = 0x0A9,

        DOT_CURRENT_START = 0x100,

        PWM_BRIGHTNESS_START = 0x200
    };
    // Static methods for accessing register values
    static constexpr uint16_t dot_group_select(uint16_t group) {                    // group:0-54
        return static_cast<uint16_t>(Registers::DOT_GROUP_SELECT_START) + group;
    }

    static constexpr uint16_t dot_onoff(uint16_t dot) {                             // dot:0-32
        return static_cast<uint16_t>(Registers::DOT_GROUP_SELECT_START) + dot;
    }

    static constexpr uint16_t dot_lod(uint16_t dot) {                               // dot:0-32
        return static_cast<uint16_t>(Registers::DOT_GROUP_SELECT_START) + dot;
    }

    static constexpr uint16_t dot_lsd(uint16_t dot) {                               // dot:0-32
        return static_cast<uint16_t>(Registers::DOT_GROUP_SELECT_START) + dot;
    }

    static constexpr uint16_t dot_current(uint16_t dot) {                           // dot:0-197 (SW0-CS0 to SW10-CS17)
        return static_cast<uint16_t>(Registers::DOT_GROUP_SELECT_START) + dot;
    }

    static constexpr uint16_t pwm_brightness(uint16_t dot) {                        // dot:0-395 (0-197: 8-bit PWM) (198-395: 16-bit PWM)
        return static_cast<uint16_t>(Registers::DOT_GROUP_SELECT_START) + dot;
    }

    // BitFlags Enum for register bitfields
    enum BitFlags : uint8_t {
        CHIP_EN_CHIP_EN = (1 << 0),

        DEV_INITIAL_PWM_FREQ = (1 << 0),
        DEV_INITIAL_DATA_REF_MODE_SHIFT = 1,
        DEV_INITIAL_DATA_REF_MODE_MASK = 0b11,
        DEV_INITIAL_MAX_LINE_NUM_SHIFT = 3,
        DEV_INITIAL_MAX_LINE_NUM_MASK = 0b1111,

        DEV_CONFIG1_CS_ON_SHIFT = (1 << 0),
        DEV_CONFIG1_PWM_PHASE_SHIFT = (1 << 1),
        DEV_CONFIG1_PWM_SCALE_MODE = (1 << 2),
        DEV_CONFIG1_SW_BLK = (1 << 3),

        DEV_CONFIG2_LSD_REMOVAL = (1 << 0),
        DEV_CONFIG2_LOD_REMOVAL = (1 << 1),
        DEV_CONFIG2_COMP_GROUP1_SHIFT = 2,
        DEV_CONFIG2_COMP_GROUP1_MASK = 0b11,
        DEV_CONFIG2_COMP_GROUP2_SHIFT = 4,
        DEV_CONFIG2_COMP_GROUP2_MASK = 0b11,
        DEV_CONFIG2_COMP_GROUP3_SHIFT = 6,
        DEV_CONFIG2_COMP_GROUP3_MASK = 0b11,

        DEV_CONFIG3_UP_DEGHOST_ENABLE = (1 << 0),
        DEV_CONFIG3_MAXIMUM_CURRENT_SHIFT = 1,
        DEV_CONFIG3_MAXUMUM_CURRENT_MASK = 0b111,
        DEV_CONFIG3_UP_DEGHOST_SHIFT = 4,
        DEV_CONFIG3_UP_DEGHOST_MASK = 0b11,
        DEV_CONFIG3_DOWN_DEGHOST_SHIFT = 6,
        DEV_CONFIG3_DOWN_DEGHOST_MASK = 0b11,

        FAULT_STATE_GLOBAL_LSD = (1 << 0),
        FAULT_STATE_GLOBAL_LOD = (1 << 1)
    };

    

    // Method to write a value to a register with custom addressing
    void writeRegister(Registers reg, uint8_t value) {
        uint8_t addrByte1 = 0x00;  // Initialize the first address byte
        uint8_t addrByte2 = static_cast<uint8_t>(reg);  // Register address is the lower byte of the register enum

        // Calculate the address bytes based on the custom addressing scheme
        addrByte1 = 0x80 | (deviceAddrBits << 3) | ((reg >> 8) & 0x03);  // Bit 7-5 = 100, Bit 4-3 = ADDR1-ADDR0, Bit 2-1 = Register[8:9]
        addrByte2 = reg & 0xFF;  // Bits 7-0 of the register address

        // Begin I2C communication for write (R/W = 0)
        wire->beginTransmission(addrByte1);  // Use addrByte1 as the device address
        wire->write(addrByte2);  // Send the second address byte (register address)
        wire->write(value);      // Send the value to write to the register
        wire->endTransmission();
    }

    // Method to read a value from a register with custom addressing
    uint8_t readRegister(Registers reg) {
        uint8_t addrByte1 = 0x00;  // Initialize the first address byte
        uint8_t addrByte2 = static_cast<uint8_t>(reg);  // Register address is the lower byte of the register enum

        // Calculate the address bytes based on the custom addressing scheme
        addrByte1 = 0x80 | (deviceAddrBits << 3) | ((reg >> 8) & 0x03);  // Bit 7-5 = 100 | Bit 4-3 = ADDR1-ADDR0 | Bit 2-1 = Register[8:9]
        addrByte2 = reg & 0xFF;  // Bits 7-0 of the register address

        // Begin I2C communication for read (R/W = 1)
        wire->beginTransmission(addrByte1);  // Use addrByte1 as the device address
        wire->write(addrByte2);  // Send the second address byte (register address)
        wire->endTransmission(false);  // Send repeated start to keep communication alive

        // Request 1 byte of data
        wire->requestFrom(addrByte1, (uint8_t)1);
        if (wire->available()) {
            return wire->read();  // Return the value read from the register
        }
        return 0;  // Return 0 if no data was available
    }
public:
    // Constructor to initialize the I2C communication
    LP5860(TwoWire* wire, uint8_t deviceAddressbits) : wire(wire), deviceAddressbits(deviceAddressbits) {}

    // Output PWM Frequency Setting
    enum PwmFrequency {
        Pwm125kHz,   // 125 kHz
        Pwm62_5kHz   // 62.5 kHz
    };

    // Line Switch Blanking Time Setting
    enum LineBlankingTime {
        Blank1us,               // 1us 
        Blanks0_5us             // 0.5us
    };

    // Dimming Scale Setting of Final PWM Generator
    enum PwmScaleMode {
        Linear,                 // Linear Scale Dimming Curve
        Exponential             // Exponential Scale Dimming Curve
    };

    // Downside deghosting level selection
    enum DownDegHost:uint8_t {
        None    = 0x00,
        Weak    = 0x01,
        Medium  = 0x02,
        Strong  = 0x03
    };
    // Scan line clamp voltage of upside deghosting
    enum UpDegHost:uint8_t {
        VledMinus2V     = 0x00,
        VledMinus2_5V   = 0x01,
        VledMinus3V     = 0x02,
        GND             = 0x03
    };

    // Data Refresh Mode Selection
    enum DataRefMode:uint8_t {
        Mode1 = 0x00,           // 
        Mode2 = 0x01,
        Mode3 = 0x02
    };

    // Maximum Current Setting
    enum CurrentSetting {
        Max3mA  = 0x00,
        Max5mA  = 0x01,
        Max10mA = 0x02,
        Max15mA = 0x03,
        Max20mA = 0x04,
        Max30mA = 0x05,
        Max40mA = 0x06,
        Max50mA = 0x07
    };

    /** 
    *  Initialization Method
    */


    /**
    *  Chip Enable Method
    */

    /**
    *  Set Device Configuration Method
    */

    /**
     * Reset Method
     */

    /**
     * Set Dot Groups Method
     */

    /**
     * Set Dot Current Method
     */

    /**
     * Set Global Brightness Method
     */

    /**
     * Set Group Brightness Method
     */

    /**
     * Get Global Fault Status Method
     */

    /**
     * Get LED Open Status Method
     */

    /**
     * Get LED Short Status Method
     */

    /**
     * Clear LED Open Fault Method
     */

    /**
     * Clear LED Short Fault Method
     */

    /**
     * Write Frame to 11 x 6 LED MAtrix
     */

    /**
     * Or do we want to make a class which internally instantiates 4 of the LP5860 sub-classes??????
     */

    
}


#endif // LP5860_H