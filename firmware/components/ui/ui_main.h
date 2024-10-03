#ifndef ESPAC_FIRMWARE_UI_MAIN_H
#define ESPAC_FIRMWARE_UI_MAIN_H


#include "bridge.h"
#include "views/view.h"
#include "iui_main.h"

class UiMain: private IUiMain {
    static void declare_fonts();
    static void declare_images();
public:
    UiMain(): bridge(*this) {};

    UiBridge bridge;
    View* active_view{};

    void create() override;

    void handle_rotary_event(RotaryEventEnum event) override;

};

#endif //ESPAC_FIRMWARE_UI_MAIN_H
