#include "thermostat_view.h"
#include "fonts.h"
#include "assets/images/generated/images.h"
#include "thermostat_overlay_menu_view.h"
#include "views.h"
#include "bridge.h"
#include "utils.h"
#include <cstdio>
#include <string>
#include <esp_log.h>
#include <memory>

#define FREEZE_COLOR 0x55CCFF
#define HEAT_COLOR 0xFF6C40
#define FAN_COLOR 0x82FF15
#define OFF_COLOR 0x969696

void arc_animation_cb(void * arc_obj, int32_t value) {
    lv_arc_set_value((lv_obj_t *)arc_obj, value);
}

LedPixelColor hex_to_led(uint32_t hexColor, float brightnessFactor) {
    // Extract RGB components from the hex color
    uint8_t r = (hexColor >> 16) & 0xFF;  // Extract the red component
    uint8_t g = (hexColor >> 8) & 0xFF;   // Extract the green component
    uint8_t b = hexColor & 0xFF;          // Extract the blue component

    // Apply brightness factor (0.0 - 1.0) to dim the color
    r = static_cast<uint8_t>(r * brightnessFactor);
    g = static_cast<uint8_t>(g * brightnessFactor);
    b = static_cast<uint8_t>(b * brightnessFactor);

    return {r, g, b};
}

struct LedTaskParams {
    uint32_t color_hex;
    float brightness;
    ThermostatView* view_instance;  // Pointer to access event_bus from the task
};


void ThermostatView::create() {
    view_obj = lv_obj_create(lv_screen_active());
    lv_obj_set_style_bg_color(view_obj, lv_color_hex(0x0D1F26), 0);
    lv_obj_set_size(view_obj, LV_HOR_RES, LV_VER_RES);
    lv_obj_set_style_border_width(view_obj, 0, 0);
    lv_obj_set_scrollbar_mode(view_obj, LV_SCROLLBAR_MODE_OFF);

    label = lv_label_create(view_obj);
    lv_label_set_text(label, "20º");
    lv_obj_set_style_text_color(label, lv_color_hex(0xffffff), LV_PART_MAIN);
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_text_font(view_obj, &poppins_semibold_68, 0);

    create_arc();


    icon = lv_image_create(view_obj);
    lv_obj_align(icon, LV_ALIGN_CENTER, 0, 50);
    lv_obj_set_size(icon, 80, 80);
    lv_image_set_src(icon, &freeze_icon);


//    lv_style_t img_style;
//    lv_style_init(&img_style);
    lv_obj_set_style_image_recolor(icon, lv_color_hex(FREEZE_COLOR), 0);  // Set to red or any other color
    lv_obj_set_style_image_recolor_opa(icon, LV_OPA_COVER, 0);  // Set to red or any other color
//    lv_style_set_image_recolor_opa(&img_style, LV_OPA_COVER);        // Full opacity for the recolor
//    lv_style_set_image_opa(&img_style, LV_OPA_COVER);
//// Apply the style to the image object
//    lv_obj_add_style(icon, &img_style, 0);

    event_bus->subscribe("set-active-view", [this](void* arg) -> void {
        auto* new_view = (ViewKeyEnum*)arg;
        if (*new_view == ViewKeyEnum::THERMOSTAT_VIEW) {
            lv_obj_remove_flag(view_obj, LV_OBJ_FLAG_HIDDEN);
        }
    });
//
//    auto* ledParams = new LedTaskParams;
//    ledParams->color_hex = 0x55CCFF;     // Example color
//    ledParams->brightness = 0.04;        // Example brightness
//    ledParams->view_instance = this;     // Pass the ThermostatView instance

    // Create a task for handling the LED ring, passing ledParams as the argument
//    xTaskCreate(led_ring_task, "LedRingTask", 2048, (void*)ledParams, 1, &ledRingTaskHandle);

    update_mode();



    lv_timer_create(update_ui, 30, this);
}

void ThermostatView::create_arc() {
    lv_style_t arc_style;

    arc = lv_arc_create(view_obj);
    lv_obj_set_size(arc, ARC_SIZE, ARC_SIZE);
    lv_obj_align(arc, LV_ALIGN_CENTER, 0, 0);
    lv_arc_set_range(arc, 10, 300);

    lv_arc_set_value(arc, 50);
//    lv_obj_add_style(arc, &arc_style, LV_PART_KNOB);
    lv_obj_set_style_arc_color(arc, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    lv_obj_set_style_arc_color(arc, lv_color_hex(0xFFFFFF), LV_PART_KNOB);
    lv_obj_set_style_arc_color(arc, lv_color_hex(0x55CCFF), LV_PART_INDICATOR);
    lv_obj_set_style_arc_opa(arc, 25, LV_PART_MAIN);
    lv_obj_set_style_arc_opa(arc, 25, LV_PART_KNOB);
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

void ThermostatView::handle_click_event(ButtonEventEnum event) {
    ESP_LOGW("Click", "Thermostat event");
    esp_log_timestamp();
    event_bus->trigger("open-overlay", nullptr);
    auto view = THERMOSTAT_OVERLAY_MENU;
    event_bus->trigger("set-active-view", (void*)(&view));

    lv_obj_add_flag(view_obj, LV_OBJ_FLAG_HIDDEN);
}

void ThermostatView::register_events() {
    event_bus->subscribe("mode-changed", [this](void* arg) {
        auto* new_mode = (Mode*)arg;
        printf("mode event: %d", *new_mode);
        mode = *new_mode;

        update_mode();
    });
}

void ThermostatView::update_mode() const {
    printf("selected mode: %d", mode);
    uint32_t color_hex;
    const lv_image_dsc_t* icon_src;

    if (mode == COOLING) {
        color_hex = FREEZE_COLOR;
        icon_src = &freeze_icon;
    } else if (mode == HEATING) {
        color_hex = HEAT_COLOR;
        icon_src = &heat_icon;
    } else if (mode == FAN) {
        color_hex = FAN_COLOR;
        icon_src = &fan_icon;
    } else {
        color_hex = OFF_COLOR;
        icon_src = &power_icon;
    }


    auto bg_color = dim_color(color_hex, 0.18);
    lv_obj_set_style_bg_color(view_obj, lv_color_hex(bg_color), 0);

    auto lv_color = lv_color_hex(color_hex);

    lv_obj_set_style_arc_color(arc, lv_color, LV_PART_INDICATOR);
    lv_image_set_src(icon, icon_src);
    lv_obj_set_style_image_recolor(icon, lv_color, 0);  // Set to red or any other color

    auto ring = std::make_shared<std::vector<LedPixelColor>>(12); // Use shared_ptr
    auto pixel = hex_to_led(color_hex, 0.02);

    for (int i = 0; i < 12; ++i) {
        (*ring)[i] = pixel;
    }

    event_bus->trigger("led-ring", ring.get());
}
