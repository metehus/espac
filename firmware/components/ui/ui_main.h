#ifndef ESPAC_FIRMWARE_UI_MAIN_H
#define ESPAC_FIRMWARE_UI_MAIN_H


#include "bridge.h"
#include "structure/view.h"
#include "iui_main.h"
#include "structure/view_orchestrator.h"
#include "views/views.h"

class UiMain: private IUiMain {
    static void declare_fonts();
    static void declare_images();
public:
    UiMain(): bridge(*this) {};

    UiBridge bridge;
    ViewKeyEnum active_view{};
    ViewOrchestrator orchestrator;
    MainEventBus* event_bus;

    void create() override;

    void handle_rotary_event(RotaryEventEnum event) override;
    void handle_click_event(ButtonEventEnum event) override;

    View* get_view(ViewKeyEnum key) override;
    View* get_active_view() override;


};

#endif //ESPAC_FIRMWARE_UI_MAIN_H
