#include "thermostat_view.h"
#include "fonts.h"
#include "images.h"
#include <cstdio>
#include <string>

void arc_animation_cb(void * arc_obj, int32_t value) {
    lv_arc_set_value((lv_obj_t *)arc_obj, value);
}


ThermostatView::ThermostatView() {
    view_obj = lv_obj_create(lv_screen_active());
    lv_obj_set_style_bg_color(view_obj, lv_color_black(), 0);
    lv_obj_set_size(view_obj, LV_HOR_RES, LV_VER_RES);
    lv_obj_set_style_border_width(view_obj, 0, 0);
    lv_obj_set_scrollbar_mode(view_obj, LV_SCROLLBAR_MODE_OFF);

    label = lv_label_create(view_obj);
    lv_label_set_text(label, "20º");
    lv_obj_set_style_text_color(label, lv_color_hex(0xffffff), LV_PART_MAIN);
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_text_font(view_obj, &poppins_semibold_68, 0);

    create_arc();


    lv_obj_t * icon = lv_image_create(view_obj);
    lv_obj_align(icon, LV_ALIGN_CENTER, 0, 50);
    lv_obj_set_size(icon, 80, 80);
    lv_image_set_src(icon, &freeze);


    lv_timer_create(update_ui, 10, this);
}

void ThermostatView::create_arc() {
    lv_style_t arc_style;

    arc = lv_arc_create(view_obj);
    lv_obj_set_size(arc, ARC_SIZE, ARC_SIZE);
    lv_obj_align(arc, LV_ALIGN_CENTER, 0, 0);
    lv_arc_set_range(arc, 10, 300);

    lv_arc_set_value(arc, 50);
//    lv_obj_add_style(arc, &arc_style, LV_PART_KNOB);
    lv_obj_set_style_arc_color(arc, lv_color_hex(0x303030), LV_PART_MAIN);
    lv_obj_set_style_arc_color(arc, lv_color_hex(0x303030), LV_PART_KNOB);
    lv_obj_set_style_arc_color(arc, lv_color_hex(0x55CCFF), LV_PART_INDICATOR);
    lv_obj_set_style_opa(arc, LV_OPA_TRANSP, LV_PART_KNOB);
    lv_obj_set_style_arc_width(arc, 0, LV_PART_KNOB);


//    lv_anim_t a;
//    lv_anim_init(&a);
//    lv_anim_set_var(&a, arc);  // Set the arc object as the target of the animation
//    lv_anim_set_values(&a, 10, 300);  // Animate from 10 to 300
//    lv_anim_set_time(&a, 2000);  // Set animation time (2 seconds to reach the end)
//    lv_anim_set_playback_time(&a, 2000);  // Set playback time (2 seconds to go back to 10)
//    lv_anim_set_repeat_delay(&a, 500);  // Optional: Add delay between repetitions
//    lv_anim_set_playback_delay(&a, 500);  // Optional: Add delay when reversing
//    lv_anim_set_exec_cb(&a, arc_animation_cb);  // Set the animation callback
//    lv_anim_set_repeat_count(&a, LV_ANIM_REPEAT_INFINITE);  // Make it loop infinitely
//    lv_anim_start(&a);  // Start the animation
}

void ThermostatView::update_ui(lv_timer_t *arg) {
    auto user_data = lv_timer_get_user_data(arg);
    auto view = static_cast<ThermostatView*>(user_data);

    if (view->value == view->previous_value) {
        return;
    }

    lv_label_set_text(view->label, (std::to_string(view->value) + "º").c_str());

    int val = view->value * 10;
    if (val < 10) {
        val = 10;
    } else if (val > 300) {
        val = 300;
    }
    lv_arc_set_value(view->arc, val);
    view->previous_value = view->value;
}

void ThermostatView::handle_rotary_event(RotaryEventEnum event) {
    if (event == CLOCKWISE) {
        value++;
    } else {
        value--;
    }
//    printf("rotary event: %i\n", event);

//    lv_async_call(update_value, this);
}