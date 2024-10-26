#ifndef ESPAC_FIRMWARE_THERMOSTAT_VIEW_H
#define ESPAC_FIRMWARE_THERMOSTAT_VIEW_H
#include "lvgl.h"
#include "structure/view_orchestrator.h"
#include "structure/view.h"
#include "enums.h"


class ThermostatView: public View {
    const int ARC_SIZE = 220;
    void create_arc();
public:
    lv_obj_t* view_obj;
    lv_obj_t* arc;
    lv_obj_t* label;
    lv_obj_t* icon;
    int value = 20;
    int previous_value = 20;
    Mode mode = OFF;

    static void update_ui(lv_timer_t* arg);

    void handle_rotary_event(RotaryEventEnum event) override;

    void register_events() override;
    void create() override;

    void handle_click_event(ButtonEventEnum event) override;

    void update_mode() const;
};


#endif //ESPAC_FIRMWARE_THERMOSTAT_VIEW_H
