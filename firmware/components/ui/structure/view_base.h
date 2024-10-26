#ifndef ESPAC_FIRMWARE_VIEWBASE_H
#define ESPAC_FIRMWARE_VIEWBASE_H
#include "../events.h"


class ViewBase {
public:
    virtual void handle_rotary_event(RotaryEventEnum event) {};
    virtual void handle_click_event(ButtonEventEnum event) {};

    virtual void create() {};
    virtual void register_events() {};
};


#endif //ESPAC_FIRMWARE_VIEWBASE_H
