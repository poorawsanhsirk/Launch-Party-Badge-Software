#include "LED_Animation.h"

#define SLAVE_ADDRESS_LP_1 0x10
#define ENABLE_PIN_LP_1 0x10
#define IFS_PIN_LP_1 0x10
#define SYNC_PIN_LP_1 0x10

#define SLAVE_ADDRESS_LP_2 0x11
#define ENABLE_PIN_LP_2 0x11
#define IFS_PIN_LP_2 0x11
#define SYNC_PIN_LP_2 0x11

#define SLAVE_ADDRESS_LP_3 0x12
#define ENABLE_PIN_LP_3 0x12
#define IFS_PIN_LP_3 0x12
#define SYNC_PIN_LP_3 0x12

#define SLAVE_ADDRESS_LP_4 0x13
#define ENABLE_PIN_LP_4 0x13
#define IFS_PIN_LP_4 0x13
#define SYNC_PIN_LP_4 0x13


LED_Animation led_obj_1 = LED_Animation(SLAVE_ADDRESS_LP_1, ENABLE_PIN_LP_1, IFS_PIN_LP_1, SYNC_PIN_LP_1);
LED_Animation led_obj_2 = LED_Animation(SLAVE_ADDRESS_LP_2, ENABLE_PIN_LP_2, IFS_PIN_LP_2, SYNC_PIN_LP_2);
LED_Animation led_obj_3 = LED_Animation(SLAVE_ADDRESS_LP_3, ENABLE_PIN_LP_3, IFS_PIN_LP_3, SYNC_PIN_LP_3);
LED_Animation led_obj_4 = LED_Animation(SLAVE_ADDRESS_LP_4, ENABLE_PIN_LP_4, IFS_PIN_LP_4, SYNC_PIN_LP_4);

void setup() {
    
}

void loop() {
    led_obj_1.SmileFace();
    led_obj_2.SmileFace();
    led_obj_3.SmileFace();
    led_obj_4.SmileFace();    
    delay(500);
}
