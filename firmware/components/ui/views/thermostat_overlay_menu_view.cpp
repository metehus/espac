#include <esp_log.h>
#include <cmath>
#include "thermostat_overlay_menu_view.h"
#include "assets/images/generated/images.h"
#include "views.h"

void ThermostatOverlayMenuView::register_events() {
    event_bus->subscribe("open-overlay", [this](void*) { open(); });
    event_bus->subscribe("close-overlay", [this](void*) { close(); });

    event_bus->subscribe("set-active-view", [this](void* arg) -> void {
        auto* new_view = (ViewKeyEnum*)arg;
        if (*new_view == ViewKeyEnum::THERMOSTAT_OVERLAY_MENU) {
            lv_obj_remove_flag(parent_obj, LV_OBJ_FLAG_HIDDEN);
        }
    });

    MenuViewBase::register_events();
}

void ThermostatOverlayMenuView::create() {
    overlay_obj = lv_obj_create(lv_screen_active());
    parent_obj = overlay_obj;

    lv_obj_set_size(overlay_obj, LV_HOR_RES, LV_VER_RES);
    lv_obj_set_scrollbar_mode(overlay_obj, LV_SCROLLBAR_MODE_OFF);

    lv_obj_set_style_bg_color(overlay_obj, lv_color_make(0, 0, 0), 0);
    lv_obj_set_style_bg_opa(overlay_obj, LV_OPA_COVER, 0);
    lv_obj_set_style_border_width(overlay_obj, 0, 0);
    lv_obj_set_style_opa(overlay_obj, 0, 0);

    lv_obj_set_style_radius(overlay_obj, LV_RADIUS_CIRCLE, 0);


    MenuViewBase::create_menu();

    lv_obj_add_flag(parent_obj, LV_OBJ_FLAG_HIDDEN);
}



std::map<uint16_t, MenuOption> ThermostatOverlayMenuView::populate_options() {
    return {
            {BACK, MenuOption  {
                .icon_src = arrow_back_icon,
                .obj = nullptr,
                .title = "Voltar",
            }},
            {MODE, MenuOption  {
                .icon_src = freeze_icon,
                .obj = nullptr,
                .title = "Modo",
            }},
            {AIR, MenuOption  {
                .icon_src = air_icon,
                .obj = nullptr,
                .title = "Fluxo do ar",
            }},
            {SETTINGS, MenuOption  {
                .icon_src = settings_icon,
                .obj = nullptr,
                .title = "Configurações",
            }},
    };
}

void ThermostatOverlayMenuView::handle_option_click(uint16_t option) {
    printf("click! menu index: %d\n", option);
    if (option == BACK) {
        event_bus->trigger("close-overlay", nullptr);
        auto view = THERMOSTAT_VIEW;
        event_bus->trigger("set-active-view", (void*)(&view));

        lv_obj_add_flag(parent_obj, LV_OBJ_FLAG_HIDDEN);
    } else if (option == MODE) {
        auto view = MODE_MENU_VIEW;
        event_bus->trigger("set-active-view", (void*)(&view));
        event_bus->trigger("open-mode", nullptr);

        lv_obj_add_flag(parent_obj, LV_OBJ_FLAG_HIDDEN);

//        event_bus->trigger("mode-changed", (void*)(&mode));
    } else if (option == SETTINGS) {
        auto view = SETTINGS_VIEW;
        event_bus->trigger("set-active-view", (void*)(&view));
        event_bus->trigger("open-settings", nullptr);

        lv_obj_add_flag(parent_obj, LV_OBJ_FLAG_HIDDEN);
    }
}