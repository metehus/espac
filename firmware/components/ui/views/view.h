#ifndef ESPAC_FIRMWARE_VIEW_H
#define ESPAC_FIRMWARE_VIEW_H
#include "../events.h"

class View {
public:
    virtual void handle_rotary_event(RotaryEventEnum event) {};
};


#endif //ESPAC_FIRMWARE_VIEW_H
