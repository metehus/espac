#ifndef ESPAC_FIRMWARE_PINS_H
#define ESPAC_FIRMWARE_PINS_H
#include "driver/gpio.h"

#define TFT_SCLK 4
#define TFT_MOSI 5
#define TFT_CS 7
#define TFT_DC 6
#define TFT_RST -1
#define TFT_MISO 7


#define GPIO_ENCODER_A 15
#define GPIO_ENCODER_B 16

#define BUTTON_PIN GPIO_NUM_47

#define LED_PIN GPIO_NUM_48

#define HAPTIC_SDA GPIO_NUM_12
#define HAPTIC_SCL GPIO_NUM_13

#endif //ESPAC_FIRMWARE_PINS_H
