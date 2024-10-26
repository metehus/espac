#include <stdio.h>

#include <utility>
#include <memory>
#include "ui_main.h"
#include "lvgl.h"
#include "views/thermostat_view.h"
#include "bridge.h"
#include "views/views.h"
#include "views/thermostat_overlay_menu_view.h"
#include "views/settings_menu_view.h"
#include "views/mode_menu_view.h"


static void anim_x_cb(void * var, int32_t v)
{
//    lv_obj_set_x((lv_obj_t*)var, v);
}

static void anim_size_cb(void * var, int32_t v)
{
    lv_obj_set_size((lv_obj_t*)var, v, v);
}

void UiMain::declare_fonts() {
    LV_FONT_DECLARE(poppins_semibold_68);
}

void UiMain::declare_images() {
//    LV_IMAGE_DECLARE(freeze);
}


void UiMain::create() {
    UiMain::declare_fonts();
    UiMain::declare_images();

    lv_obj_set_style_bg_color(lv_screen_active(), lv_color_black(), 0);

    lv_obj_set_scrollbar_mode(lv_screen_active(), LV_SCROLLBAR_MODE_OFF);

    event_bus = new MainEventBus();

    event_bus->subscribe("led-ring", [this](void* arg) -> void {
        auto* pixels = static_cast<std::vector<LedPixelColor>*>(arg);
        // Wrap the pointer back to shared_ptr for proper memory management

//        printf("a: %d", ring[0].red);
//        bridge.update_led_ring(ring);

        if (pixels != nullptr) {
            bridge.update_led_ring(*pixels); // Pass by reference
        } else {
            printf("pixels is null\n");
        }
    });

    auto thermostat_view = new ThermostatView();
    auto thermostat_overlay_menu_view = new ThermostatOverlayMenuView();
    auto settings_menu_view = new SettingsMenu::SettingsMenuView();
    auto mode_menu_view = new ModeMenu::ModeMenuView();

    views[THERMOSTAT_VIEW] = thermostat_view;
    views[THERMOSTAT_OVERLAY_MENU] = thermostat_overlay_menu_view;
    views[SETTINGS_VIEW] = settings_menu_view;
    views[MODE_MENU_VIEW] = mode_menu_view;

    thermostat_view->event_bus = event_bus;
    thermostat_overlay_menu_view->event_bus = event_bus;
    settings_menu_view->event_bus = event_bus;
    mode_menu_view->event_bus = event_bus;

    thermostat_view->register_events();
    thermostat_overlay_menu_view->register_events();
    settings_menu_view->register_events();
    mode_menu_view->register_events();

    thermostat_view->create();
    thermostat_overlay_menu_view->create();
    settings_menu_view->create();
    mode_menu_view->create();

    active_view = THERMOSTAT_VIEW;
    event_bus->subscribe("set-active-view", [this](void* arg) -> void {
        auto* new_view = (ViewKeyEnum*)arg;
        printf("set active view: %d\n", *new_view);

        this->active_view = *new_view;
    });


}

void UiMain::handle_rotary_event(RotaryEventEnum event) {
    auto ac_view = get_active_view();
    if (ac_view == nullptr) {
        printf("invalid active_view\n");
        return;
    }

    event_bus->trigger("rotary-change", (void*)&event);
    ac_view->handle_rotary_event(event);

}

struct ClickEventData {
    UiMain* ui_main;          // Pointer to the UiMain instance
    ButtonEventEnum event;    // The event to be passed
};

static void handle_click_event_async(void* data_ptr) {
    auto* data = (ClickEventData*)data_ptr;

    auto ac_view = data->ui_main->get_active_view();
    if (ac_view == nullptr) {
        printf("invalid active_view\n");
        return;
    }

    ac_view->handle_click_event(data->event);

    delete data;
}

void UiMain::handle_click_event(ButtonEventEnum event) {
    auto* data = new ClickEventData{ this, event };

    lv_async_call(handle_click_event_async, data);
}

View *UiMain::get_view(ViewKeyEnum key) {
    return views[key];
}

View *UiMain::get_active_view() {
    return get_view(active_view);
}
