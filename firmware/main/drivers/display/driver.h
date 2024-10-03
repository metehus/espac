#ifndef ESPAC_FIRMWARE_DRIVER_H
#define ESPAC_FIRMWARE_DRIVER_H
#include "../../pins.h"

#include "ui_main.h"

#define DISP_WIDTH              240
#define DISP_HEIGHT              240

#define LCD_HOST  SPI2_HOST


class DisplayDriver {
public:
    DisplayDriver();

    void init(UiMain* ui);
};


#endif //ESPAC_FIRMWARE_DRIVER_H
