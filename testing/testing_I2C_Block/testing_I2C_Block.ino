#include "LP586x_I2C.h"

#include "Wire.h"
#include <stdint.h>

#define SLAVE_ADDRESS 0x10

#define ENABLE_PIN 0x01
#define SYNC_PIN 0x00

#define sdaPin 0x05
#define sclPin 0x04

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

LP586X_I2C test_driver(ENABLE_PIN, SYNC_PIN);

void setup() {
    Serial.begin(115200);
    test_driver.begin(sdaPin, sclPin);

    // Resetting the IC
    test_driver.disableDevice();
    delay(10);
    test_driver.enableDevice();
    delay(10);

    // Writing to a register and software enabling the IC
    test_driver.setSlave(SLAVE_ADDRESS, Chip_Enable_Register);
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

}