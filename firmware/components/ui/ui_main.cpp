#include <stdio.h>

#include <utility>
#include "ui_main.h"
#include "lvgl.h"
#include "lv_examples.h"
#include "fonts.h"
#include "views/thermostat_view.h"
#include "bridge.h"


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

    lv_obj_set_scrollbar_mode(lv_screen_active(), LV_SCROLLBAR_MODE_OFF);

    auto thermostat_view = new ThermostatView();

    active_view = thermostat_view;
}

void UiMain::handle_rotary_event(RotaryEventEnum event) {
    if (active_view == nullptr) {
        printf("invalid active_view\n");
        return;
    }

    active_view->handle_rotary_event(event);
}
