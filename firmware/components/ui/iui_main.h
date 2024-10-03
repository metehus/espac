#ifndef ESPAC_FIRMWARE_IUI_MAIN_H
#define ESPAC_FIRMWARE_IUI_MAIN_H

#include "views/view.h"

class IUiMain {
public:
    View* active_view;

    virtual void create() {};

    virtual void handle_rotary_event(RotaryEventEnum event) {};
};

#endif //ESPAC_FIRMWARE_IUI_MAIN_H
