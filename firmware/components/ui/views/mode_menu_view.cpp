#include "mode_menu_view.h"
#include "enums.h"
#include "views.h"

using namespace ModeMenu;

void ModeMenuView::register_events() {
    event_bus->subscribe("open-mode", [this](void*) { open(); });
    event_bus->subscribe("close-mode", [this](void*) { close(); });

    MenuViewBase::register_events();
}

void ModeMenuView::create() {
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
}

std::map<uint16_t, MenuOption> ModeMenuView::populate_options() {
    return {
            {OFF, MenuOption  {
                    .icon_src = power_icon,
                    .obj = nullptr,
                    .title = "Desligado",
            }},
            {COOLING, MenuOption  {
                    .icon_src = freeze_icon,
                    .obj = nullptr,
                    .title = "Resfriar",
            }},
            {HEATING, MenuOption  {
                    .icon_src = heat_icon,
                    .obj = nullptr,
                    .title = "Aquecer",
            }},
            {FAN, MenuOption  {
                    .icon_src = fan_icon,
                    .obj = nullptr,
                    .title = "Ventilar",
            }},

    };
}

void ModeMenuView::handle_option_click(uint16_t option) {
    printf("selected option: %d\n", option);
    event_bus->trigger("mode-changed", (void*)(&option));

    printf("mode changed: %d\n", *(&option));

    close();
    auto view = THERMOSTAT_OVERLAY_MENU;
    event_bus->trigger("set-active-view", (void*)(&view));
}
