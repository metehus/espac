#include "settings_menu_view.h"
#include "views.h"
using namespace SettingsMenu;

void SettingsMenuView::register_events() {
    event_bus->subscribe("open-settings", [this](void*) { open(); });
    event_bus->subscribe("close-settings", [this](void*) { close(); });

    MenuViewBase::register_events();
}

void SettingsMenuView::create() {
    menu_obj = lv_obj_create(lv_screen_active());
    parent_obj = menu_obj;

    lv_obj_set_size(menu_obj, LV_HOR_RES, LV_VER_RES);
    lv_obj_set_scrollbar_mode(menu_obj, LV_SCROLLBAR_MODE_OFF);

    lv_obj_set_style_bg_color(menu_obj, lv_color_make(0, 0, 0), 0);
    lv_obj_set_style_bg_opa(menu_obj, LV_OPA_COVER, 0);
    lv_obj_set_style_border_width(menu_obj, 0, 0);
    lv_obj_set_style_opa(menu_obj, 0, 0);

    lv_obj_set_style_radius(menu_obj, LV_RADIUS_CIRCLE, 0);

    MenuViewBase::create_menu();

    lv_obj_add_flag(parent_obj, LV_OBJ_FLAG_HIDDEN);
    printf("settings invisible\n");

}

std::map<uint16_t, MenuOption> SettingsMenuView::populate_options() {
    return {
            {SettingsMenu::MenuOptionEnum::BACK, MenuOption  {
                    .icon_src = arrow_back_icon,
                    .obj = nullptr,
                    .title = "Voltar",
            }},
            {SettingsMenu::MenuOptionEnum::LED_CHECKUP, MenuOption  {
                    .icon_src = settings_icon,
                    .obj = nullptr,
                    .title = "Config",
            }},

    };
}

void SettingsMenuView::handle_option_click(uint16_t option) {
//    printf("click! menu index: %d\n", option);
    if (option == SettingsMenu::MenuOptionEnum::BACK) {
        close();
        auto view = THERMOSTAT_OVERLAY_MENU;
        event_bus->trigger("set-active-view", (void*)(&view));
    }
}