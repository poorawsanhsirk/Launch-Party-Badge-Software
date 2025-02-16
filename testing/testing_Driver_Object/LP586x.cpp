#include "LP586x.h"
/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>
#include "LP586x_I2C.h"

LP586x::LP586x(uint8_t slave_address, uint8_t sdaPin, uint8_t sclPin, uint8_t enablePin, uint8_t syncPin)
{
    _slave_address = slave_address;
    LP586X_i2c_obj = LP586X_I2C(enablePin, syncPin);
    LP586X_i2c_obj.begin(sdaPin, sclPin);
    Chip_Normal();
    // delay(100);
    Device_Initial(Max_Line_11, Mode_3_1, PWM_Frequency_125k);
    Device_Configuration_2(Comp_Group3_2clock, Comp_Group2_Off, Comp_Group1_Off, LOD_Removal_Off, LSD_Removal_Off);
}

/***************************************************************************//**
 *Function Definition
 ******************************************************************************/
/* Set Chip_EN=0 to enter STANDBY mode */
void LP586x::Chip_Standby()
{
    LP586X_i2c_obj.setSlave (_slave_address,
                  Chip_Enable_Register);
    LP586X_i2c_obj.writeRegister (Chip_Enable_Register,
               Chip_Disable);
}

/* Set Chip_EN=1 to enter NORMAL mode */
void LP586x::Chip_Normal()
{
    // Resetting the Chip helped resolve lock-ups. Remove later if not needed
    Chip_Standby();

    // Setting Bit0 of Register Address 0x00 to 1                            
    LP586X_i2c_obj.setSlave (_slave_address,
                  Chip_Enable_Register);
    LP586X_i2c_obj.writeRegister (Chip_Enable_Register,
               Chip_Enable);
}

/* Device initialization */
void LP586x::Device_Initial(uint8_t Max_Line,
                    uint8_t Data_Ref_Mode,
                    uint8_t PWM_Fre)
{
    int Device_Initial_Value;
    Device_Initial_Value = (Max_Line<<3) + (Data_Ref_Mode<<1) + PWM_Fre;

    LP586X_i2c_obj.setSlave (_slave_address,
                  Device_Initial_Register);
    LP586X_i2c_obj.writeRegister (Device_Initial_Register,
               Device_Initial_Value);
}

/* Set up device configuration 1 */
void LP586x::Device_Configuration_1(uint8_t SW_BLK,
                            uint8_t PWM_Scale_mode,
                            uint8_t PWM_Phase_Shift,
                            uint8_t CS_ON_Shift)
{
    int Device_Configuration_1_Value;
    Device_Configuration_1_Value = (SW_BLK<<3) + (PWM_Scale_mode<<2) + (PWM_Phase_Shift<<1) + CS_ON_Shift;

    LP586X_i2c_obj.setSlave (_slave_address,
                  Device_Configuration_1_Register);
    LP586X_i2c_obj.writeRegister (Device_Configuration_1_Register,
               Device_Configuration_1_Value);
}

/* Set up device configuration 2 */
void LP586x::Device_Configuration_2(uint8_t Comp_Group3,
                            uint8_t Comp_Group2,
                            uint8_t Comp_Group1,
                            uint8_t LOD_Removal,
                            uint8_t LSD_Removal)
{
    int Device_Configuration_2_Value;
    Device_Configuration_2_Value = (Comp_Group3<<6) + (Comp_Group2<<4) + (Comp_Group1<<2) + (LOD_Removal<<1) + LSD_Removal;

    LP586X_i2c_obj.setSlave (_slave_address,
                  Device_Configuration_2_Register);
    LP586X_i2c_obj.writeRegister (Device_Configuration_2_Register,
               Device_Configuration_2_Value);
}

/* Set up device configuration 3 */
void LP586x::Device_Configuration_3(uint8_t Down_Deghost,
                            uint8_t Up_Deghost,
                            uint8_t Maximum_Current,
                            uint8_t Up_Deghost_Enable)
{
    int Device_Configuration_3_Value;
    Device_Configuration_3_Value = (Down_Deghost<<6) + (Up_Deghost<<4) + (Maximum_Current<<1) + Up_Deghost_Enable;

    LP586X_i2c_obj.setSlave (_slave_address,
                  Device_Configuration_3_Register);
    LP586X_i2c_obj.writeRegister (Device_Configuration_3_Register,
               Device_Configuration_3_Value);
}

/* Set LED group */
// void LP586x::Group_Control_Setting(uint8_t Dot, uint8_t Group_Sel)

/* Set LED dots onoff */
// void LP586x::LED_onoff(uint8_t Dot, uint8_t Dot_onoff)

/* Set group color */
void LP586x::ColorGroup_current (uint8_t CC_Group1,
                         uint8_t CC_Group2,
                         uint8_t CC_Group3)
{
    LP586X_i2c_obj.setSlave (_slave_address,
                  Color_Group_R_Current_Register);
    LP586X_i2c_obj.writeRegister (Color_Group_R_Current_Register,
               CC_Group1);

    LP586X_i2c_obj.setSlave (_slave_address,
                  Color_Group_G_Current_Register);
    LP586X_i2c_obj.writeRegister (Color_Group_G_Current_Register,
               CC_Group2);

    LP586X_i2c_obj.setSlave (_slave_address,
                  Color_Group_B_Current_Register);
    LP586X_i2c_obj.writeRegister (Color_Group_B_Current_Register,
               CC_Group3);
}

/* Set color for LED dot */
void LP586x::DC_Color(uint8_t Dot,
              uint8_t R_DC,
              uint8_t G_DC,
              uint8_t B_DC)
{
    LP586X_i2c_obj.setSlave (_slave_address,
                  LED_Dot_Current_Register_Start+3*Dot);
    LP586X_i2c_obj.writeRegister (LED_Dot_Current_Register_Start+3*Dot,
               R_DC);

    LP586X_i2c_obj.setSlave (_slave_address,
                  LED_Dot_Current_Register_Start+3*Dot+1);
    LP586X_i2c_obj.writeRegister (LED_Dot_Current_Register_Start+3*Dot+1,
               G_DC);

    LP586X_i2c_obj.setSlave (_slave_address,
                  LED_Dot_Current_Register_Start+3*Dot+2);
    LP586X_i2c_obj.writeRegister (LED_Dot_Current_Register_Start+3*Dot+2,
               B_DC);
}

/* Set color for all LEDs */
void LP586x::DC_Color_All(uint8_t R_DC,
                  uint8_t G_DC,
                  uint8_t B_DC)
{
    int Dot;
    for (Dot=0; Dot<66; Dot++)
        DC_Color(Dot,
                 R_DC,
                 G_DC,
                 B_DC);
}

/* Set global PWM value */
void LP586x::Master_Brightness(uint8_t PWM_Global)
{
    LP586X_i2c_obj.setSlave (_slave_address,
                  Master_Brightness_Register);
    LP586X_i2c_obj.writeRegister (Master_Brightness_Register,
               PWM_Global);
}

/* Set group1/2/3 PWM value */
void LP586x::Group_Brightness(uint8_t PWM_Group1,
                      uint8_t PWM_Group2,
                      uint8_t PWM_Group3)
{
    LP586X_i2c_obj.setSlave (_slave_address,
                  Group_1_Brightness_Register);
    LP586X_i2c_obj.writeRegister (Group_1_Brightness_Register,
               PWM_Group1);

    LP586X_i2c_obj.setSlave (_slave_address,
                  Group_2_Brightness_Register);
    LP586X_i2c_obj.writeRegister (Group_2_Brightness_Register,
               PWM_Group2);

    LP586X_i2c_obj.setSlave (_slave_address,
                  Group_3_Brightness_Register);
    LP586X_i2c_obj.writeRegister (Group_3_Brightness_Register,
               PWM_Group3);
}

/* Set 8-bit PWM value for LED_dot */
void LP586x::Device_LED_Dot_Brightness_8bit(uint8_t Dot,
                                    uint8_t R_PWM,
                                    uint8_t G_PWM,
                                    uint8_t B_PWM)
{
    LP586X_i2c_obj.setSlave (_slave_address,
                  LED_Dot_Brightness_Register_Start+3*Dot);
    LP586X_i2c_obj.writeRegister (LED_Dot_Brightness_Register_Start+3*Dot,
               R_PWM);

   LP586X_i2c_obj.setSlave (_slave_address,
                 LED_Dot_Brightness_Register_Start+3*Dot+1);
    LP586X_i2c_obj.writeRegister (LED_Dot_Brightness_Register_Start+3*Dot+1,
               G_PWM);

   LP586X_i2c_obj.setSlave (_slave_address,
                 LED_Dot_Brightness_Register_Start+3*Dot+2);
    LP586X_i2c_obj.writeRegister (LED_Dot_Brightness_Register_Start+3*Dot+2,
               B_PWM);
}

/* Set 16-bit PWM value for LED_dot */
void LP586x::Device_LED_Dot_Brightness_16bit(uint8_t Dot,
                                     uint16_t R_PWM,
                                     uint16_t G_PWM,
                                     uint16_t B_PWM)
{
    LP586X_i2c_obj.setSlave (_slave_address,
                  LED_Dot_Brightness_Register_Start+6*Dot);
    LP586X_i2c_obj.writeRegister (LED_Dot_Brightness_Register_Start+6*Dot,
               R_PWM);
    LP586X_i2c_obj.writeRegister (LED_Dot_Brightness_Register_Start+6*Dot+1,
               R_PWM>>8);

   LP586X_i2c_obj.setSlave (_slave_address,
                 LED_Dot_Brightness_Register_Start+6*Dot+2);
    LP586X_i2c_obj.writeRegister (LED_Dot_Brightness_Register_Start+6*Dot+2,
               G_PWM);
    LP586X_i2c_obj.writeRegister (LED_Dot_Brightness_Register_Start+6*Dot+3,
               G_PWM>>8);

   LP586X_i2c_obj.setSlave (_slave_address,
                 LED_Dot_Brightness_Register_Start+6*Dot+4);
    LP586X_i2c_obj.writeRegister (LED_Dot_Brightness_Register_Start+6*Dot+4,
               B_PWM);
    LP586X_i2c_obj.writeRegister (LED_Dot_Brightness_Register_Start+6*Dot+5,
               B_PWM>>8);
}

/* Reset all registers */
void LP586x::Reset()
{
    LP586X_i2c_obj.setSlave (_slave_address,
                  Reset_Register);
    LP586X_i2c_obj.writeRegister (Reset_Register,
               Reset_En);
}

/* Clear LOD flags */
void LP586x::LOD_Clear()
{
    LP586X_i2c_obj.setSlave (_slave_address,
                  LOD_Clear_Register);
    LP586X_i2c_obj.writeRegister (LOD_Clear_Register,
               LOD_Clear_En);
}

/* Clear LOD flags */
void LP586x::LSD_Clear()
{
    LP586X_i2c_obj.setSlave (_slave_address,
                  LSD_Clear_Register);
    LP586X_i2c_obj.writeRegister (LSD_Clear_Register,
               LSD_Clear_En);
}

// void LP586x::ReadRegister(uint)
