#ifndef ESPAC_FIRMWARE_SETTINGS_MENU_VIEW_H
#define ESPAC_FIRMWARE_SETTINGS_MENU_VIEW_H

#include "structure/menu_view_base.h"

namespace SettingsMenu {
    enum MenuOptionEnum {
        LED_CHECKUP,
        BACK,
    };

    class SettingsMenuView: public MenuViewBase {
        lv_obj_t* menu_obj;
    public:
        void create() override;

        void register_events() override;

        std::map<uint16_t, MenuOption> populate_options() override;

        void handle_option_click(uint16_t option) override;
    };

}

#endif //ESPAC_FIRMWARE_SETTINGS_MENU_VIEW_H
