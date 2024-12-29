#include "LED_Animation.h"
/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>


LED_Animation::LED_Animation(uint8_t slave_address, uint8_t enablePin, uint8_t ifsPin, uint8_t syncPin)
{
    LP586x_obj = LP586x(slave_address, enablePin, ifsPin, syncPin);
}

void LED_Animation::Breathing()
{

    int i, j;

    while(1)
    {
        for (i=0; i<256; i++)
        {
            for (j=0; j<66; j++)
                LP586x_obj.Device_LED_Dot_Brightness_8bit(j, i, i, i);
            delay(5);
        }
        for (i=255; i>=0; i--)
        {
            for (j=0; j<66; j++)
                LP586x_obj.Device_LED_Dot_Brightness_8bit(j, i, i, i);
            delay(5);
        }
    }
}

void LED_Animation::Rainbow()
{
    int i, Dot;
    int GS_Rainbow_R[256] = {
                          255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
                          255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
                          255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
                          255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
                          255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
                          255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
                          250,238,228,217,207,198,188,179,170,162,154,145,137,130,123,116,
                          109,103, 97, 91, 85, 80, 75, 70, 65, 60, 56, 62, 48, 44, 40, 37,
                           34, 31, 28, 25, 23, 21, 19, 17, 15, 13, 11, 10,  9,  8,  7,  6,
                            5,  5,  4,  4,  4,  3,  3,  3,  2,  2,  2,  2,  1,  1,  1,  1,
                            0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
                            0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
                            1,  1,  1,  1,  2,  2,  2,  2,  3,  3,  3,  4,  4,  4,  5,  5,
                            6,  7,  8,  9, 10, 11, 13, 15, 17, 19, 21, 23, 25, 28, 31, 34,
                           37, 40, 44, 48, 62, 56, 60, 65, 70, 75, 80, 85, 91, 97,103,109,
                          116,123,130,137,145,154,162,170,179,188,198,207,217,228,238,250
                         };

    int GS_Rainbow_G[256] = {
                            0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
                            0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
                            1,  1,  1,  1,  2,  2,  2,  2,  3,  3,  3,  4,  4,  4,  5,  5,
                            6,  7,  8,  9, 10, 11, 13, 15, 17, 19, 21, 23, 25, 28, 31, 34,
                           37, 40, 44, 48, 52, 56, 60, 65, 70, 75, 80, 85, 91, 97,103,109,
                          116,123,130,137,145,154,162,170,179,188,198,207,217,228,238,250,
                          255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
                          255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
                          255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
                          255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
                          250,238,228,217,207,198,188,179,170,162,154,145,137,130,123,116,
                          109,103, 97, 91, 85, 80, 75, 70, 65, 60, 56, 52, 48, 44, 40, 37,
                           34, 31, 28, 25, 23, 21, 19, 17, 15, 13, 11, 10,  9,  8,  7,  6,
                            5,  5,  4,  4,  4,  3,  3,  3,  2,  2,  2,  2,  1,  1,  1,  1,
                            0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
                            0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
                         };

    int GS_Rainbow_B[256] = {
                          250,238,228,217,207,198,188,179,170,162,154,145,137,130,123,116,
                          109,103, 97, 91, 85, 80, 75, 70, 65, 60, 56, 52, 48, 44, 40, 37,
                           34, 31, 28, 25, 23, 21, 19, 17, 15, 13, 11, 10,  9,  8,  7,  6,
                            5,  5,  4,  4,  4,  3,  3,  3,  2,  2,  2,  2,  1,  1,  1,  1,
                            0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
                            0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
                            0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
                            0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
                            1,  1,  1,  1,  2,  2,  2,  2,  3,  3,  3,  4,  4,  4,  5,  5,
                            6,  7,  8,  9, 10, 11, 13, 15, 17, 19, 21, 23, 25, 28, 31, 34,
                           37, 40, 44, 48, 52, 56, 60, 65, 70, 75, 80, 85, 91, 97,103,109,
                          116,123,130,137,145,154,162,170,179,188,198,207,217,228,238,250,
                          255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
                          255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
                          255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
                          255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255
                         };
    int Rainbow_GS_length = (sizeof(GS_Rainbow_R)) / (sizeof(GS_Rainbow_R[0]));
    while (1)
    {
        int x=2;
        for (i=0; i<Rainbow_GS_length; i++)
        {
            for (Dot=0; Dot<66; Dot++)
                LP586x_obj.Device_LED_Dot_Brightness_8bit(Dot,
                                               GS_Rainbow_R[(i+x*Dot) % Rainbow_GS_length],
                                               GS_Rainbow_G[(i+x*Dot) % Rainbow_GS_length],
                                               GS_Rainbow_B[(i+x*Dot) % Rainbow_GS_length]);
            delay(5);
        }
    }
}

// void LED_Animation::Rainbow_Start()
// {
//     int i, Dot;
//     int GS_Rainbow_R[256] = {
//                           255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
//                           255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
//                           255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
//                           255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
//                           255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
//                           255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
//                           250,238,228,217,207,198,188,179,170,162,154,145,137,130,123,116,
//                           109,103, 97, 91, 85, 80, 75, 70, 65, 60, 56, 62, 48, 44, 40, 37,
//                            34, 31, 28, 25, 23, 21, 19, 17, 15, 13, 11, 10,  9,  8,  7,  6,
//                             5,  5,  4,  4,  4,  3,  3,  3,  2,  2,  2,  2,  1,  1,  1,  1,
//                             0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
//                             0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
//                             1,  1,  1,  1,  2,  2,  2,  2,  3,  3,  3,  4,  4,  4,  5,  5,
//                             6,  7,  8,  9, 10, 11, 13, 15, 17, 19, 21, 23, 25, 28, 31, 34,
//                            37, 40, 44, 48, 62, 56, 60, 65, 70, 75, 80, 85, 91, 97,103,109,
//                           116,123,130,137,145,154,162,170,179,188,198,207,217,228,238,250
//                          };

//     int GS_Rainbow_G[256] = {
//                             0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
//                             0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
//                             1,  1,  1,  1,  2,  2,  2,  2,  3,  3,  3,  4,  4,  4,  5,  5,
//                             6,  7,  8,  9, 10, 11, 13, 15, 17, 19, 21, 23, 25, 28, 31, 34,
//                            37, 40, 44, 48, 52, 56, 60, 65, 70, 75, 80, 85, 91, 97,103,109,
//                           116,123,130,137,145,154,162,170,179,188,198,207,217,228,238,250,
//                           255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
//                           255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
//                           255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
//                           255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
//                           250,238,228,217,207,198,188,179,170,162,154,145,137,130,123,116,
//                           109,103, 97, 91, 85, 80, 75, 70, 65, 60, 56, 52, 48, 44, 40, 37,
//                            34, 31, 28, 25, 23, 21, 19, 17, 15, 13, 11, 10,  9,  8,  7,  6,
//                             5,  5,  4,  4,  4,  3,  3,  3,  2,  2,  2,  2,  1,  1,  1,  1,
//                             0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
//                             0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
//                          };

//     int GS_Rainbow_B[256] = {
//                           250,238,228,217,207,198,188,179,170,162,154,145,137,130,123,116,
//                           109,103, 97, 91, 85, 80, 75, 70, 65, 60, 56, 52, 48, 44, 40, 37,
//                            34, 31, 28, 25, 23, 21, 19, 17, 15, 13, 11, 10,  9,  8,  7,  6,
//                             5,  5,  4,  4,  4,  3,  3,  3,  2,  2,  2,  2,  1,  1,  1,  1,
//                             0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
//                             0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
//                             0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
//                             0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
//                             1,  1,  1,  1,  2,  2,  2,  2,  3,  3,  3,  4,  4,  4,  5,  5,
//                             6,  7,  8,  9, 10, 11, 13, 15, 17, 19, 21, 23, 25, 28, 31, 34,
//                            37, 40, 44, 48, 52, 56, 60, 65, 70, 75, 80, 85, 91, 97,103,109,
//                           116,123,130,137,145,154,162,170,179,188,198,207,217,228,238,250,
//                           255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
//                           255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
//                           255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
//                           255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255
//                          };
//     int Rainbow_GS_length = (sizeof(GS_Rainbow_R)) / (sizeof(GS_Rainbow_R[0]));
//     while (1)
//     {
//         int x=2;
//         for (i=0; i<Rainbow_GS_length; i++)
//         {
//             for (Dot=0; Dot<66; Dot++)
//                 LP586x_obj.Device_LED_Dot_Brightness_8bit(Dot,
//                                                GS_Rainbow_R[(i+x*Dot) % Rainbow_GS_length],
//                                                GS_Rainbow_G[(i+x*Dot) % Rainbow_GS_length],
//                                                GS_Rainbow_B[(i+x*Dot) % Rainbow_GS_length]);
//             delay(5);
//         }
//     }
// }

void LED_Animation::Snake_Basic(int Bri)
{
    int Dot, i;

    while (1)
    {
        for (Dot=0; Dot<70; Dot++)
        {
            for (i=0; i<(Bri/8); i++)
            {
            LP586x_obj.Device_LED_Dot_Brightness_8bit(Dot,
                                           i*8,
                                           i*8,
                                           i*8);
            if ((Dot-1)>=0)
                LP586x_obj.Device_LED_Dot_Brightness_8bit(Dot-1,
                                               Bri-i*4,
                                               Bri-i*4,
                                               Bri-i*4);
            if ((Dot-2)>=0)
                LP586x_obj.Device_LED_Dot_Brightness_8bit(Dot-2,
                                               Bri/2-i*2,
                                               Bri/2-i*2,
                                               Bri/2-i*2);
            if ((Dot-3)>=0)
                LP586x_obj.Device_LED_Dot_Brightness_8bit(Dot-3,
                                               Bri/4-i,
                                               Bri/4-i,
                                               Bri/4-i);
            if ((Dot-4)>=0)
                LP586x_obj.Device_LED_Dot_Brightness_8bit(Dot-4,
                                               Bri/8-i,
                                               Bri/8-i,
                                               Bri/8-i);
            delay(5);
            }
        }
    }
}

void LED_Animation::Snake_Loop(int Bri)
{
    int Dot, i;

    int Snake_Loop[] = { 0,  1,  2,  3,  4,  5, 11, 10,  9,  8,  7,  6,
                        12, 13, 14, 15, 16, 17, 23, 22, 21, 20, 19, 18,
                        24, 25, 26, 27, 28, 29, 35, 34, 33, 32, 31, 30,
                        36, 37, 38, 39, 40, 41, 47, 46, 45, 44, 43, 42,
                        48, 49, 50, 51, 52, 53, 59, 58, 57, 56, 55, 54,
                        60, 61, 62, 63, 64, 65, 59, 58, 57, 56, 55, 54,
                        48, 49, 50, 51, 52, 53, 47, 46, 45, 44, 43, 42,
                        36, 37, 38, 39, 40, 41, 35, 34, 33, 32, 31, 30,
                        24, 25, 26, 27, 28, 29, 23, 22, 21, 20, 19, 18,
                        12, 13, 14, 15, 16, 17, 11, 10,  9,  8,  7,  6,
                         0,  1,  2,  3,  4
    };

    int Snake_Loop_length = (sizeof(Snake_Loop)) / (sizeof(Snake_Loop[0]));

    for (Dot=0; Dot<5; Dot++)
    {
        for (i=0; i<(Bri/8); i++)
        {
        LP586x_obj.Device_LED_Dot_Brightness_8bit(Snake_Loop[Dot],
                                       i*8,
                                       i*8,
                                       i*8);
        if ((Dot-1)>=0)
            LP586x_obj.Device_LED_Dot_Brightness_8bit(Snake_Loop[Dot-1],
                                           Bri-i*4,
                                           Bri-i*4,
                                           Bri-i*4);
        if ((Dot-2)>=0)
            LP586x_obj.Device_LED_Dot_Brightness_8bit(Snake_Loop[Dot-2],
                                           Bri/2-i*2,
                                           Bri/2-i*2,
                                           Bri/2-i*2);
        if ((Dot-3)>=0)
            LP586x_obj.Device_LED_Dot_Brightness_8bit(Snake_Loop[Dot-3],
                                           Bri/4-i,
                                           Bri/4-i,
                                           Bri/4-i);
        if ((Dot-4)>=0)
            LP586x_obj.Device_LED_Dot_Brightness_8bit(Snake_Loop[Dot-4],
                                           Bri/8-i,
                                           Bri/8-i,
                                           Bri/8-i);
        delay(3);
        }
    }

    while (1)
    {
        for (Dot=5; Dot<Snake_Loop_length; Dot++)
        {
            for (i=0; i<(Bri/8); i++)
            {
            LP586x_obj.Device_LED_Dot_Brightness_8bit(Snake_Loop[Dot],
                                           i*8,
                                           i*8,
                                           i*8);
            LP586x_obj.Device_LED_Dot_Brightness_8bit(Snake_Loop[Dot-1],
                                           Bri-i*4,
                                           Bri-i*4,
                                           Bri-i*4);
            LP586x_obj.Device_LED_Dot_Brightness_8bit(Snake_Loop[Dot-2],
                                           Bri/2-i*2,
                                           Bri/2-i*2,
                                           Bri/2-i*2);
            LP586x_obj.Device_LED_Dot_Brightness_8bit(Snake_Loop[Dot-3],
                                           Bri/4-i,
                                           Bri/4-i,
                                           Bri/4-i);
            LP586x_obj.Device_LED_Dot_Brightness_8bit(Snake_Loop[Dot-4],
                                           Bri/8-i,
                                           Bri/8-i,
                                           Bri/8-i);
            delay(5);
            }
        }
    }
}

void LED_Animation::Snake_Single(int Bri)
{
    int Dot, i;

    int Snake_Loop[] = { 0,  1,  2,  3,  4,  5, 11, 10,  9,  8,  7,  6,
                        12, 13, 14, 15, 16, 17, 23, 22, 21, 20, 19, 18,
                        24, 25, 26, 27, 28, 29, 35, 34, 33, 32, 31, 30,
                        36, 37, 38, 39, 40, 41, 47, 46, 45, 44, 43, 42,
                        48, 49, 50, 51, 52, 53, 59, 58, 57, 56, 55, 54,
                        60, 61, 62, 63, 64, 65, 59, 58, 57, 56, 55, 54,
                        48, 49, 50, 51, 52, 53, 47, 46, 45, 44, 43, 42,
                        36, 37, 38, 39, 40, 41, 35, 34, 33, 32, 31, 30,
                        24, 25, 26, 27, 28, 29, 23, 22, 21, 20, 19, 18,
                        12, 13, 14, 15, 16, 17, 11, 10,  9,  8,  7,  6,
                         0,  0,  0,  0,  0
    };

    int Snake_Loop_length = (sizeof(Snake_Loop)) / (sizeof(Snake_Loop[0]));

    for (Dot=0; Dot<Snake_Loop_length; Dot++)
    {
        for (i=0; i<(Bri/8); i++)
        {
            LP586x_obj.Device_LED_Dot_Brightness_8bit(Snake_Loop[Dot],
                                           i*8,
                                           i*8,
                                           i*8);
            if ((Dot-1)>=0)
                LP586x_obj.Device_LED_Dot_Brightness_8bit(Snake_Loop[Dot-1],
                                               Bri-i*4,
                                               Bri-i*4,
                                               Bri-i*4);
            if ((Dot-2)>=0)
                LP586x_obj.Device_LED_Dot_Brightness_8bit(Snake_Loop[Dot-2],
                                               Bri/2-i*2,
                                               Bri/2-i*2,
                                               Bri/2-i*2);
            if ((Dot-3)>=0)
                LP586x_obj.Device_LED_Dot_Brightness_8bit(Snake_Loop[Dot-3],
                                               Bri/4-i,
                                               Bri/4-i,
                                               Bri/4-i);
            if ((Dot-4)>=0)
                LP586x_obj.Device_LED_Dot_Brightness_8bit(Snake_Loop[Dot-4],
                                               Bri/8-i,
                                               Bri/8-i,
                                               Bri/8-i);
            delay(3);
        }
    }
}

void LED_Animation::Snake_Single_withLP5860(int Bri)
{
    int Dot, i, j=0;

    int Snake_Loop[] = { 0,  1,  2,  3,  4,  5, 11, 10,  9,  8,  7,  6,
                        12, 13, 14, 15, 16, 17, 23, 22, 21, 20, 19, 18,
                        24, 25, 26, 27, 28, 29, 35, 34, 33, 32, 31, 30,
                        36, 37, 38, 39, 40, 41, 47, 46, 45, 44, 43, 42,
                        48, 49, 50, 51, 52, 53, 59, 58, 57, 56, 55, 54,
                        60, 61, 62, 63, 64, 65, 65, 65, 65, 65,
    };

    int LP5860_Dotnumber_5[] = {  0,  1,  2,
                                  6,
                                 12, 13, 14,
                                         20,
                                 24, 25, 26, 66, 66
    };
    int LP5860_Dotnumber_8[] = {  3,  4,  5,
                                  9,     11,
                                 15, 16, 17,
                                 21,     23,
                                 27, 28, 29
    };
    int LP5860_Dotnumber_6[] = { 36, 37, 38,
                                 42,
                                 48, 49, 50,
                                 54,     56,
                                 60, 61, 62
    };
    int LP5860_Dotnumber_0[] = { 39, 40, 41,
                                 45,     47,
                                 51,     53,
                                 57,     59,
                                 63, 64, 65, 66
    };

    int Snake_Loop_length = (sizeof(Snake_Loop)) / (sizeof(Snake_Loop[0]));
    int LP5860_Dotnumber_length = (sizeof(LP5860_Dotnumber_8)) / (sizeof(LP5860_Dotnumber_8[0]));

    for (Dot=0; Dot<Snake_Loop_length; Dot++)
    {
        int decline_index[4] = {0, 0, 0, 0};

        for (j=0; j<LP5860_Dotnumber_length; j++)
        {
            if ((Dot-1)>=0)
            {
                if (Snake_Loop[Dot-1]==LP5860_Dotnumber_5[j])
                    decline_index[0] = 1; // decline to yellow
                if (Snake_Loop[Dot-1]==LP5860_Dotnumber_8[j])
                    decline_index[0] = 2;
                if (Snake_Loop[Dot-1]==LP5860_Dotnumber_6[j])
                    decline_index[0] = 3;
                if (Snake_Loop[Dot-1]==LP5860_Dotnumber_0[j])
                    decline_index[0] = 4;
            }
            if ((Dot-1)>=0)
            {
                if (Snake_Loop[Dot-2]==LP5860_Dotnumber_5[j])
                    decline_index[1] = 1;
                if (Snake_Loop[Dot-2]==LP5860_Dotnumber_8[j])
                    decline_index[1] = 2;
                if (Snake_Loop[Dot-2]==LP5860_Dotnumber_6[j])
                    decline_index[1] = 3;
                if (Snake_Loop[Dot-2]==LP5860_Dotnumber_0[j])
                    decline_index[1] = 4;
            }

            if ((Dot-1)>=0)
            {
                if (Snake_Loop[Dot-3]==LP5860_Dotnumber_5[j])
                    decline_index[2] = 1;
                if (Snake_Loop[Dot-3]==LP5860_Dotnumber_8[j])
                    decline_index[2] = 2;
                if (Snake_Loop[Dot-3]==LP5860_Dotnumber_6[j])
                    decline_index[2] = 3;
                if (Snake_Loop[Dot-3]==LP5860_Dotnumber_0[j])
                    decline_index[2] = 4;
            }

            if ((Dot-1)>=0)
            {
                if (Snake_Loop[Dot-4]==LP5860_Dotnumber_5[j])
                    decline_index[3] = 1;
                if (Snake_Loop[Dot-4]==LP5860_Dotnumber_8[j])
                    decline_index[3] = 2;
                if (Snake_Loop[Dot-4]==LP5860_Dotnumber_6[j])
                    decline_index[3] = 3;
                if (Snake_Loop[Dot-4]==LP5860_Dotnumber_0[j])
                    decline_index[3] = 4;
            }
        }

        for (i=0; i<(Bri/8); i++)
        {
            LP586x_obj.Device_LED_Dot_Brightness_8bit(Snake_Loop[Dot],
                                           i*8,
                                           i*8,
                                           i*8);
            if ((Dot-1)>=0)
            {
                if (decline_index[0]==0)
                        LP586x_obj.Device_LED_Dot_Brightness_8bit(Snake_Loop[Dot-1],
                                                       Bri-i*4,
                                                       Bri-i*4,
                                                       Bri-i*4);
                else if (decline_index[0]==1)
                    LP586x_obj.Device_LED_Dot_Brightness_8bit(Snake_Loop[Dot-1],
                                                   Bri-i*4,
                                                   Bri,
                                                   Bri);
                else if (decline_index[0]==2)
                    LP586x_obj.Device_LED_Dot_Brightness_8bit(Snake_Loop[Dot-1],
                                                   Bri,
                                                   Bri-i*4,
                                                   Bri);
                else if (decline_index[0]==3)
                    LP586x_obj.Device_LED_Dot_Brightness_8bit(Snake_Loop[Dot-1],
                                                   Bri,
                                                   Bri,
                                                   Bri-i*4);
                else if (decline_index[0]==4)
                    LP586x_obj.Device_LED_Dot_Brightness_8bit(Snake_Loop[Dot-1],
                                                   Bri-i*4,
                                                   Bri-i*4,
                                                   Bri);
            }

            if ((Dot-2)>=0)
            {
                if (decline_index[1]==0)
                    LP586x_obj.Device_LED_Dot_Brightness_8bit(Snake_Loop[Dot-2],
                                                   Bri/2-i*2,
                                                   Bri/2-i*2,
                                                   Bri/2-i*2);
                else if (decline_index[1]==1)
                    LP586x_obj.Device_LED_Dot_Brightness_8bit(Snake_Loop[Dot-2],
                                                   Bri/2-i*2,
                                                   Bri,
                                                   Bri);
                else if (decline_index[1]==2)
                    LP586x_obj.Device_LED_Dot_Brightness_8bit(Snake_Loop[Dot-2],
                                                   Bri,
                                                   Bri/2-i*2,
                                                   Bri);
                else if (decline_index[1]==3)
                    LP586x_obj.Device_LED_Dot_Brightness_8bit(Snake_Loop[Dot-2],
                                                   Bri,
                                                   Bri,
                                                   Bri/2-i*2);
                else if (decline_index[1]==4)
                    LP586x_obj.Device_LED_Dot_Brightness_8bit(Snake_Loop[Dot-2],
                                                   Bri/2-i*2,
                                                   Bri/2-i*2,
                                                   Bri);
            }

            if ((Dot-3)>=0)
            {
                if (decline_index[2]==0)
                    LP586x_obj.Device_LED_Dot_Brightness_8bit(Snake_Loop[Dot-3],
                                                   Bri/4-i,
                                                   Bri/4-i,
                                                   Bri/4-i);
                else if (decline_index[2]==1)
                    LP586x_obj.Device_LED_Dot_Brightness_8bit(Snake_Loop[Dot-3],
                                                   Bri/4-i,
                                                   Bri,
                                                   Bri);
                else if (decline_index[2]==2)
                    LP586x_obj.Device_LED_Dot_Brightness_8bit(Snake_Loop[Dot-3],
                                                   Bri,
                                                   Bri/4-i,
                                                   Bri);
                else if (decline_index[2]==3)
                    LP586x_obj.Device_LED_Dot_Brightness_8bit(Snake_Loop[Dot-3],
                                                   Bri,
                                                   Bri,
                                                   Bri/4-i);
                else if (decline_index[2]==4)
                    LP586x_obj.Device_LED_Dot_Brightness_8bit(Snake_Loop[Dot-3],
                                                   Bri/4-i,
                                                   Bri/4-i,
                                                   Bri);
            }

            if ((Dot-4)>=0)
            {
                if (decline_index[3]==0)
                    LP586x_obj.Device_LED_Dot_Brightness_8bit(Snake_Loop[Dot-4],
                                                   Bri/8-i,
                                                   Bri/8-i,
                                                   Bri/8-i);
                else if (decline_index[3]==1)
                    LP586x_obj.Device_LED_Dot_Brightness_8bit(Snake_Loop[Dot-4],
                                                   Bri/8-i,
                                                   Bri,
                                                   Bri);
                else if (decline_index[3]==2)
                    LP586x_obj.Device_LED_Dot_Brightness_8bit(Snake_Loop[Dot-4],
                                                   Bri,
                                                   Bri/8-i,
                                                   Bri);
                else if (decline_index[3]==3)
                    LP586x_obj.Device_LED_Dot_Brightness_8bit(Snake_Loop[Dot-4],
                                                   Bri,
                                                   Bri,
                                                   Bri/8-i);
                else if (decline_index[3]==4)
                    LP586x_obj.Device_LED_Dot_Brightness_8bit(Snake_Loop[Dot-4],
                                                   Bri/8-i,
                                                   Bri/8-i,
                                                   Bri);
            }

            delay(3);
        }

    }
}

void LED_Animation::Helix_Single(int Bri)
{
    int Dot, i;

    int Helix_Loop[] = { 0,  1,  2,  3,  4,  5,
                        11, 17, 23, 29, 35, 41, 47, 53, 59, 65,
                        64, 63, 62, 61, 60,
                        54, 48, 42, 36, 30, 24, 18, 12, 6,
                         7,  8,  9, 10,
                        16, 22, 28, 34, 40, 46, 52, 58,
                        57, 56, 55,
                        49, 43, 37, 31, 25, 19, 13,
                        14, 15,
                        21, 27, 33, 39, 45, 51,
                        50,
                        44, 38, 32, 26, 20
    };

    int Helix_Loop_length = (sizeof(Helix_Loop)) / (sizeof(Helix_Loop[0]));

    for (Dot=0; Dot<Helix_Loop_length; Dot++)
    {
        for (i=0; i<Bri/2; i++)
        {
        LP586x_obj.Device_LED_Dot_Brightness_8bit(Helix_Loop[Dot],
                                       2*i,
                                       2*i,
                                       2*i);
        delayMicroseconds(400);
        }
    }

    delay(500);

    for (Dot=Helix_Loop_length-1; Dot>=0; Dot--)
    {
        for (i=Bri/2; i>=0; i--)
        {
        LP586x_obj.Device_LED_Dot_Brightness_8bit(Helix_Loop[Dot],
                                       2*i,
                                       2*i,
                                       2*i);
        delayMicroseconds(200);
        }
    }
}

void LED_Animation::Helix_Night(int Bri)
{
    int Dot, i;

    int Helix_Loop[] = { 0,  1,  2,  3,  4,  5,
                        11, 17, 23, 29, 35, 41, 47, 53, 59, 65,
                        64, 63, 62, 61, 60,
                        54, 48, 42, 36, 30, 24, 18, 12, 6,
                         7,  8,  9, 10,
                        16, 22, 28, 34, 40, 46, 52, 58,
                        57, 56, 55,
                        49, 43, 37, 31, 25, 19, 13,
                        14, 15,
                        21, 27, 33, 39, 45, 51,
                        50,
                        44, 38, 32, 26, 20
    };

    int Helix_Loop_length = (sizeof(Helix_Loop)) / (sizeof(Helix_Loop[0]));

    for (Dot=0; Dot<Helix_Loop_length; Dot++)
    {
        for (i=0; i<Bri/2; i++)
        {
        LP586x_obj.Device_LED_Dot_Brightness_8bit(Helix_Loop[Dot],
                                       2*i,
                                       2*i,
                                       2*i);
        delayMicroseconds(300);
        }
    }

    delay(500);

    for (Dot=Helix_Loop_length-1; Dot>=0; Dot--)
    {
        for (i=Bri/2; i>=0; i--)
        {
        LP586x_obj.Device_LED_Dot_Brightness_8bit(Helix_Loop[Dot],
                                       2*i+2,
                                       2*i+2,
                                       2*i+30);
        delayMicroseconds(100);
        }
    }
}

void LED_Animation::Firework(int Bri)
{
    int Dot, i, j;

    int Firework[] = { 63, 57, 51, 45, 39, 33, 27, 21, 21, 21, 21
    };

    int Fire1[][5] = { {21, 22, 23, 23, 23},
                       {21, 27, 33, 33, 33},
                       {21, 20, 19, 19, 19},
                       {21, 15,  9,  9,  9}
    };

    int Fire2[][5] = { {21, 16, 11, 11, 11},
                       {21, 28, 35, 35, 35},
                       {21, 26, 31, 31, 31},
                       {21, 14,  7,  7,  7}
    };

    int Firework_length = (sizeof(Firework)) / (sizeof(Firework[0]));
    int Fire1_length = (sizeof(Fire1)) / (sizeof(Fire1[0][0])) / 4;
    int Fire2_length = (sizeof(Fire2)) / (sizeof(Fire2[0][0])) / 4;

/* #1 fire */
    for (Dot=0; Dot<Firework_length; Dot++)
    {
        for (i=0; i<(Bri/4); i++)
        {
        LP586x_obj.Device_LED_Dot_Brightness_8bit(Firework[Dot],
                                       i*4,
                                       i*4,
                                       i*4);
        if ((Dot-1)>=0)
            LP586x_obj.Device_LED_Dot_Brightness_8bit(Firework[Dot-1],
                                           Bri-i*2,
                                           Bri-i*2,
                                           Bri-i*2);
        if ((Dot-2)>=0)
            LP586x_obj.Device_LED_Dot_Brightness_8bit(Firework[Dot-2],
                                           Bri/2-i,
                                           Bri/2-i,
                                           Bri/2-i);
        if ((Dot-3)>=0)
            LP586x_obj.Device_LED_Dot_Brightness_8bit(Firework[Dot-3],
                                           Bri/4-i,
                                           Bri/4-i,
                                           Bri/4-i);
        delayMicroseconds(500);
        }
    }

    for (Dot=0; Dot<Fire1_length; Dot++)
    {
        for (i=0; i<(Bri/2); i++)
        {
            for (j=0; j<4; j++)
            {
                LP586x_obj.Device_LED_Dot_Brightness_8bit(Fire1[j][Dot],
                                               i*2,
                                               0,
                                               0);
                if ((Dot-1)>=0)
                    LP586x_obj.Device_LED_Dot_Brightness_8bit(Fire1[j][Dot-1],
                                                   Bri-i,
                                                   0,
                                                   0);
                if ((Dot-2)>=0)
                    LP586x_obj.Device_LED_Dot_Brightness_8bit(Fire1[j][Dot-2],
                                                   Bri/2-i,
                                                   0,
                                                   0);
            }
        delayMicroseconds(1000);
        }
    }

    for (Dot=0; Dot<Fire2_length; Dot++)
    {
        for (i=0; i<(Bri/2); i++)
        {
            for (j=0; j<4; j++)
            {
                LP586x_obj.Device_LED_Dot_Brightness_8bit(Fire2[j][Dot],
                                               i*2,
                                               0,
                                               i*2);
                if ((Dot-1)>=0)
                    LP586x_obj.Device_LED_Dot_Brightness_8bit(Fire2[j][Dot-1],
                                                   Bri-i,
                                                   0,
                                                   Bri-i);
                if ((Dot-2)>=0)
                    LP586x_obj.Device_LED_Dot_Brightness_8bit(Fire2[j][Dot-2],
                                                   Bri/2-i,
                                                   0,
                                                   Bri/2-i);
            }
        delayMicroseconds(1000);
        }
    }

/* #2 fire */
    for (Dot=0; Dot<Firework_length; Dot++)
    {
        for (i=0; i<(Bri/4); i++)
        {
        LP586x_obj.Device_LED_Dot_Brightness_8bit(Firework[Dot]+5,
                                       i*4,
                                       i*4,
                                       i*4);
        if ((Dot-1)>=0)
            LP586x_obj.Device_LED_Dot_Brightness_8bit(Firework[Dot-1]+5,
                                           Bri-i*2,
                                           Bri-i*2,
                                           Bri-i*2);
        if ((Dot-2)>=0)
            LP586x_obj.Device_LED_Dot_Brightness_8bit(Firework[Dot-2]+5,
                                           Bri/2-i,
                                           Bri/2-i,
                                           Bri/2-i);
        if ((Dot-3)>=0)
            LP586x_obj.Device_LED_Dot_Brightness_8bit(Firework[Dot-3]+5,
                                           Bri/4-i,
                                           Bri/4-i,
                                           Bri/4-i);
        delayMicroseconds(500);
        }
    }

    for (Dot=0; Dot<Fire1_length; Dot++)
    {
        for (i=0; i<(Bri/2); i++)
        {
            for (j=0; j<4; j++)
            {
                LP586x_obj.Device_LED_Dot_Brightness_8bit(Fire1[j][Dot]+5,
                                               0,
                                               i*2,
                                               0);
                if ((Dot-1)>=0)
                    LP586x_obj.Device_LED_Dot_Brightness_8bit(Fire1[j][Dot-1]+5,
                                                   0,
                                                   Bri-i,
                                                   0);
                if ((Dot-2)>=0)
                    LP586x_obj.Device_LED_Dot_Brightness_8bit(Fire1[j][Dot-2]+5,
                                                   0,
                                                   Bri/2-i,
                                                   0);
            }
        delayMicroseconds(1000);
        }
    }

    for (Dot=0; Dot<Fire2_length; Dot++)
    {
        for (i=0; i<(Bri/2); i++)
        {
            for (j=0; j<4; j++)
            {
                LP586x_obj.Device_LED_Dot_Brightness_8bit(Fire2[j][Dot]+5,
                                               i*2,
                                               i*2,
                                               0);
                if ((Dot-1)>=0)
                    LP586x_obj.Device_LED_Dot_Brightness_8bit(Fire2[j][Dot-1]+5,
                                                   Bri-i,
                                                   Bri-i,
                                                   0);
                if ((Dot-2)>=0)
                    LP586x_obj.Device_LED_Dot_Brightness_8bit(Fire2[j][Dot-2]+5,
                                                   Bri/2-i,
                                                   Bri/2-i,
                                                   0);
            }
        delayMicroseconds(1000);
        }
    }
}

void LED_Animation::Firework_Night(int Bri)
{
    int Dot, i, j;

    int Firework[] = { 63, 57, 51, 45, 39, 33, 27, 21, 21, 21, 21
    };

    int Fire1[][5] = { {21, 22, 23, 23, 23},
                       {21, 27, 33, 33, 33},
                       {21, 20, 19, 19, 19},
                       {21, 15,  9,  9,  9}
    };

    int Fire2[][5] = { {21, 16, 11, 11, 11},
                       {21, 28, 35, 35, 35},
                       {21, 26, 31, 31, 31},
                       {21, 14,  7,  7,  7}
    };

    int Firework_length = (sizeof(Firework)) / (sizeof(Firework[0]));
    int Fire1_length = (sizeof(Fire1)) / (sizeof(Fire1[0][0])) / 4;
    int Fire2_length = (sizeof(Fire2)) / (sizeof(Fire2[0][0])) / 4;

/* #1 fire */
    for (Dot=0; Dot<Firework_length; Dot++)
    {
        for (i=0; i<(Bri/4); i++)
        {
        LP586x_obj.Device_LED_Dot_Brightness_8bit(Firework[Dot],
                                       i*4+2,
                                       i*4+2,
                                       i*4+30);
        if ((Dot-1)>=0)
            LP586x_obj.Device_LED_Dot_Brightness_8bit(Firework[Dot-1],
                                           Bri-i*2+2,
                                           Bri-i*2+2,
                                           Bri-i*2+30);
        if ((Dot-2)>=0)
            LP586x_obj.Device_LED_Dot_Brightness_8bit(Firework[Dot-2],
                                           Bri/2-i+2,
                                           Bri/2-i+2,
                                           Bri/2-i+30);
        if ((Dot-3)>=0)
            LP586x_obj.Device_LED_Dot_Brightness_8bit(Firework[Dot-3],
                                           Bri/4-i+2,
                                           Bri/4-i+2,
                                           Bri/4-i+30);
        delayMicroseconds(500);
        }
    }

    for (Dot=0; Dot<Fire1_length; Dot++)
    {
        for (i=0; i<(Bri/2); i++)
        {
            for (j=0; j<4; j++)
            {
                LP586x_obj.Device_LED_Dot_Brightness_8bit(Fire1[j][Dot],
                                               i*2+2,
                                               0+2,
                                               0+30);
                if ((Dot-1)>=0)
                    LP586x_obj.Device_LED_Dot_Brightness_8bit(Fire1[j][Dot-1],
                                                   Bri-i+2,
                                                   0+2,
                                                   0+30);
                if ((Dot-2)>=0)
                    LP586x_obj.Device_LED_Dot_Brightness_8bit(Fire1[j][Dot-2],
                                                   Bri/2-i+2,
                                                   0+2,
                                                   0+30);
            }
        delayMicroseconds(1000);
        }
    }

    for (Dot=0; Dot<Fire2_length; Dot++)
    {
        for (i=0; i<(Bri/2); i++)
        {
            for (j=0; j<4; j++)
            {
                LP586x_obj.Device_LED_Dot_Brightness_8bit(Fire2[j][Dot],
                                               i*2,
                                               0,
                                               i*2);
                if ((Dot-1)>=0)
                    LP586x_obj.Device_LED_Dot_Brightness_8bit(Fire2[j][Dot-1],
                                                   Bri-i,
                                                   0,
                                                   Bri-i);
                if ((Dot-2)>=0)
                    LP586x_obj.Device_LED_Dot_Brightness_8bit(Fire2[j][Dot-2],
                                                   Bri/2-i+2,
                                                   0+2,
                                                   Bri/2-i+30);
            }
        delayMicroseconds(1000);
        }
    }

/* #2 fire */
    for (Dot=0; Dot<Firework_length; Dot++)
    {
        for (i=0; i<(Bri/4); i++)
        {
        LP586x_obj.Device_LED_Dot_Brightness_8bit(Firework[Dot]+5,
                                       i*4+2,
                                       i*4+2,
                                       i*4+30);
        if ((Dot-1)>=0)
            LP586x_obj.Device_LED_Dot_Brightness_8bit(Firework[Dot-1]+5,
                                           Bri-i*2+2,
                                           Bri-i*2+2,
                                           Bri-i*2+30);
        if ((Dot-2)>=0)
            LP586x_obj.Device_LED_Dot_Brightness_8bit(Firework[Dot-2]+5,
                                           Bri/2-i+2,
                                           Bri/2-i+2,
                                           Bri/2-i+30);
        if ((Dot-3)>=0)
            LP586x_obj.Device_LED_Dot_Brightness_8bit(Firework[Dot-3]+5,
                                           Bri/4-i+2,
                                           Bri/4-i+2,
                                           Bri/4-i+30);
        delayMicroseconds(500);
        }
    }

    for (Dot=0; Dot<Fire1_length; Dot++)
    {
        for (i=0; i<(Bri/2); i++)
        {
            for (j=0; j<4; j++)
            {
                LP586x_obj.Device_LED_Dot_Brightness_8bit(Fire1[j][Dot]+5,
                                               0+2,
                                               i*2+2,
                                               0+30);
                if ((Dot-1)>=0)
                    LP586x_obj.Device_LED_Dot_Brightness_8bit(Fire1[j][Dot-1]+5,
                                                   0+2,
                                                   Bri-i+2,
                                                   0+30);
                if ((Dot-2)>=0)
                    LP586x_obj.Device_LED_Dot_Brightness_8bit(Fire1[j][Dot-2]+5,
                                                   0+2,
                                                   Bri/2-i+2,
                                                   0+30);
            }
        delayMicroseconds(1000);
        }
    }

    for (Dot=0; Dot<Fire2_length; Dot++)
    {
        for (i=0; i<(Bri/2); i++)
        {
            for (j=0; j<4; j++)
            {
                LP586x_obj.Device_LED_Dot_Brightness_8bit(Fire2[j][Dot]+5,
                                               i*2+2,
                                               i*2+2,
                                               0+30);
                if ((Dot-1)>=0)
                    LP586x_obj.Device_LED_Dot_Brightness_8bit(Fire2[j][Dot-1]+5,
                                                   Bri-i+2,
                                                   Bri-i+2,
                                                   0+30);
                if ((Dot-2)>=0)
                    LP586x_obj.Device_LED_Dot_Brightness_8bit(Fire2[j][Dot-2]+5,
                                                   Bri/2-i+2,
                                                   Bri/2-i+2,
                                                   0+30);
            }
        delayMicroseconds(1000);
        }
    }
}

void LED_Animation::SmileFace()
{
    int i=20, Dot;

    int SmileFace_GS[11][6][3] = {
                                  { {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }, // line_0
                                  { {0,0,0}, {1,1,0}, {1,1,0}, {1,1,0}, {1,1,0}, {0,0,0} }, // line_1
                                  { {1,1,0}, {1,1,0}, {1,1,0}, {1,1,0}, {1,1,0}, {1,1,0} }, // line_2
                                  { {1,1,0}, {0,0,1}, {1,1,0}, {1,1,0}, {0,0,1}, {1,1,0} }, // line_3
                                  { {1,1,0}, {0,0,1}, {1,1,0}, {1,1,0}, {0,0,1}, {1,1,0} }, // line_4
                                  { {1,1,0}, {1,1,0}, {1,1,0}, {1,1,0}, {1,1,0}, {1,1,0} }, // line_5
                                  { {1,1,1}, {1,1,0}, {1,1,0}, {1,1,0}, {1,1,0}, {1,1,1} }, // line_6
                                  { {1,1,0}, {1,1,1}, {1,1,0}, {1,1,0}, {1,1,1}, {1,1,0} }, // line_7
                                  { {1,1,0}, {1,1,0}, {1,1,1}, {1,1,1}, {1,1,0}, {1,1,0} }, // line_8
                                  { {0,0,0}, {1,1,0}, {1,1,0}, {1,1,0}, {1,1,0}, {0,0,0} }, // line_9
                                  { {0,0,0}, {0,0,0}, {1,1,0}, {1,1,0}, {0,0,0}, {0,0,0} } // line_10
    };

    for (Dot=0; Dot<66; Dot++)
        LP586x_obj.Device_LED_Dot_Brightness_8bit(Dot,
                                       SmileFace_GS[Dot/6][Dot%6][0] * i,
                                       SmileFace_GS[Dot/6][Dot%6][1] * i,
                                       SmileFace_GS[Dot/6][Dot%6][2] * i);

}
