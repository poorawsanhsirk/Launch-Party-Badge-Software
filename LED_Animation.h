#ifndef LED_ANIMATION_H_
#define LED_ANIMATION_H_

#include "LP586x.h"
//*****************************************************************************
//
// If building with a C++ compiler, make all of the definitions in this header
// have a C binding.
//
//*****************************************************************************
#ifdef __cplusplus
extern "C"
{
#endif

class LED_Animation
{
    private:
        LP586x LP586x_obj;
    public:
        LED_Animation(uint8_t slave_address, uint8_t enablePin, uint8_t ifsPin, uint8_t syncPin);
        void Breathing();
        void Rainbow();
        void SmileFace();
        void Snake_Basic(int Bri);
        void Snake_Loop(int Bri);
        void Snake_Single(int Bri);
        void Snake_Single_withLP5860(int Bri);
        void Helix_Single(int Bri);
        void Helix_Night(int Bri);
        void Firework(int Bri);
        void Firework_Night(int Bri);
};

//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************
#endif /* LED_ANIMATION_H_ */
