#ifndef ESPAC_FIRMWARE_IUI_MAIN_H
#define ESPAC_FIRMWARE_IUI_MAIN_H

#include "structure/view.h"
#include "views/views.h"

class IUiMain {
public:
    std::string active_view;
    std::map<ViewKeyEnum, View*> views;

    virtual void create() {};

    virtual void handle_rotary_event(RotaryEventEnum event) {};
    virtual void handle_click_event(ButtonEventEnum event) {};
    virtual View* get_view(ViewKeyEnum key) { return nullptr; };
    virtual View* get_active_view() { return nullptr; };
};


#endif //ESPAC_FIRMWARE_IUI_MAIN_H
