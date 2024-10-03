#ifndef ESPAC_FIRMWARE_THERMOSTAT_VIEW_H
#define ESPAC_FIRMWARE_THERMOSTAT_VIEW_H
#include "lvgl.h"
#include "view.h"


class ThermostatView: public View {
    const int ARC_SIZE = 220;
    void create_arc();
public:
    lv_obj_t* view_obj;
    lv_obj_t* arc;
    lv_obj_t* label;
    int value = 20;
    int previous_value = 20;

    ThermostatView();

    static void update_ui(lv_timer_t* arg);

    void handle_rotary_event(RotaryEventEnum event) override;
};


#endif //ESPAC_FIRMWARE_THERMOSTAT_VIEW_H
