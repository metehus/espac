#ifndef ESPAC_FIRMWARE_THERMOSTAT_OVERLAY_MENU_VIEW_H
#define ESPAC_FIRMWARE_THERMOSTAT_OVERLAY_MENU_VIEW_H
#include "structure/view.h"
#include "assets/images/generated/images.h"
#include "lvgl.h"
#include "enums.h"
#include "structure/menu_view_base.h"
#include <map>


enum MenuOptionEnum {
    MODE = 1,
    AIR,
    SETTINGS,
    BACK,
};



class ThermostatOverlayMenuView: public MenuViewBase {
    lv_obj_t* overlay_obj;
public:
    Mode mode = COOLING;

    void create() override;

    void register_events() override;

    std::map<uint16_t, MenuOption> populate_options() override;

    void handle_option_click(uint16_t option) override;
};


#endif //ESPAC_FIRMWARE_THERMOSTAT_OVERLAY_MENU_VIEW_H
