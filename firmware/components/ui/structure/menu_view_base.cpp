#include <cmath>
#include "menu_view_base.h"


void MenuViewBase::register_events() {
    event_bus->subscribe("rotary-change", [this](void* arg) {
        auto* event = (RotaryEventEnum*)arg;
        this->handle_rotary_event(*event);
    });
}

int dot_indicator_radius = 50;

static void update_circle_position(void* var, int32_t angle) {
    auto obj = (lv_obj_t*)var;

    int x_dist = dot_indicator_radius * cos(angle * M_PI / 180);
    int y_dist = dot_indicator_radius * sin(angle * M_PI / 180);

    lv_obj_align(obj, LV_ALIGN_CENTER, x_dist, y_dist);
//    int x = CENTER_X + dot_indicator_radius * cos(angle / 180);
//    int y = CENTER_Y + dot_indicator_radius * sin(angle / 180);
//    lv_obj_set_pos(obj, x - lv_obj_get_width(obj) / 2, y - lv_obj_get_height(obj) / 2);
}

void MenuViewBase::create_menu() {
    options = populate_options();
    auto size = options.size();

    uint8_t radius = 90;
    uint8_t diff_angle = 180 / size;
    uint8_t index = 0;

    for (auto option = options.begin(); option != options.end(); ++option) {
        options_vec.push_back(option->first);

        double angledeg = 180 + (diff_angle * index) + diff_angle * 0.5;
        double angle = angledeg * M_PI / 180;
        option->second.angle = angledeg;

        int x_dist = radius * cos(angle);
        int y_dist = radius * sin(angle);

        lv_obj_t * icon = lv_image_create(parent_obj);
        lv_obj_align(icon, LV_ALIGN_CENTER, x_dist, y_dist);
//        lv_obj_set_pos(icon, x, y);
//        lv_obj_set_size(icon, iconSize, iconSize);
        lv_image_set_scale(icon, 255 * 0.7);
        lv_image_set_src(icon, &option->second.icon_src);


        lv_obj_set_style_image_recolor(icon, lv_color_hex(0xffffff), 0);
        lv_obj_set_style_image_recolor_opa(icon, LV_OPA_COVER, 0);
        if (index == 0) {
            lv_obj_set_style_opa(icon, LV_OPA_COVER, 0);
        } else {
            lv_obj_set_style_opa(icon, LV_OPA_30, 0);
        }

        option->second.obj = icon;

        index++;
    }


    dot_indicator_obj = lv_obj_create(parent_obj);
    lv_obj_set_size(dot_indicator_obj, 10, 10);
    lv_obj_set_style_radius(dot_indicator_obj, LV_RADIUS_CIRCLE, 0);
    lv_obj_set_style_bg_color(dot_indicator_obj, lv_color_hex(0xFFFFFF), 0); // Red circle

    auto first_option = options[options_vec[0]];
    update_circle_position(dot_indicator_obj, first_option.angle);


    menu_option_label = lv_label_create(parent_obj);
    lv_label_set_text(menu_option_label, first_option.title.c_str());
    lv_obj_set_style_text_color(menu_option_label, lv_color_hex(0xffffff), LV_PART_MAIN);
    lv_obj_align(menu_option_label, LV_ALIGN_CENTER, 0, 5);


    menu_option_label_alternative = lv_label_create(parent_obj);
    lv_label_set_text(menu_option_label_alternative, first_option.title.c_str());
    lv_obj_set_style_text_color(menu_option_label_alternative, lv_color_hex(0xffffff), LV_PART_MAIN);
    lv_obj_align(menu_option_label_alternative, LV_ALIGN_CENTER, 0, 5);
    lv_obj_set_style_opa(menu_option_label_alternative, LV_OPA_0, 0);
}



void MenuViewBase::handle_option_click(uint16_t option) {
    printf("WARN: handle_option_click NOT IMPLEMENTED");
}

std::map<uint16_t, MenuOption> MenuViewBase::populate_options() {
    printf("ERR: populate_options NOT IMPLEMENTED");

    return {};
}






void animate_option(void * var, int32_t value) {
    auto *obj = static_cast<lv_obj_t *>(var);
    float factor = value / 100.0f;
    float inc_op = (255-75)*factor;
    lv_obj_set_style_opa(obj, 75 + inc_op, 0);
}

void animate_option_label(void * var, int32_t value) {
    auto *obj = static_cast<lv_obj_t *>(var);
    float factor = value / 100.0f;
    lv_obj_set_x(obj, factor * 90);

    uint8_t opacity = 255 * (1 - fabs(factor));
    lv_obj_set_style_opa(obj, opacity, 0);
}

void update_menu_async(void* arg) {
    auto* instance = (MenuViewBase*)arg;

    uint8_t index = 0;
    MenuOption* last_option = nullptr;
    MenuOption* new_option = nullptr;
    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_exec_cb(&a, animate_option);
    lv_anim_set_time(&a, 120);
    for (auto option = instance->options.begin(); option != instance->options.end(); ++option) {
        if (option->second.obj == nullptr) {
            printf("Option obj is null\n");
            return;
        }
        lv_anim_set_var(&a, option->second.obj);
        if (index == instance->menu_index) {
            new_option = &option->second;
//
            lv_anim_set_values(&a, 0, 100);
            lv_anim_start(&a);
//            lv_obj_set_style_opa(option->second.obj, LV_OPA_COVER, 0);
        } else if (index == instance->last_menu_index) {
            last_option = &option->second;
            lv_anim_set_values(&a, 100, 0);
            lv_anim_start(&a);
        } else {
            lv_anim_set_values(&a, 0, 0);
            lv_anim_start(&a);
//            lv_obj_set_style_opa(option->second.obj, 75, 0);
//            lv_obj_set_style_opa(option->second.obj, LV_OPA_30, 0);
        }

        index++;
    }

    if (new_option != nullptr && new_option->obj != nullptr) {
        lv_anim_t anim;
        lv_anim_init(&anim);
        lv_anim_set_var(&anim, instance->dot_indicator_obj);
        lv_anim_set_exec_cb(&anim, update_circle_position);  // Callback function to update position
        lv_anim_set_time(&anim, 150); // Duration of the animation (2 seconds)
        lv_anim_set_values(&anim, instance->dot_current_angle, new_option->angle); // Animate from 0 to 360 degrees
//    lv_anim_set_repeat_count(LV_ANIM_REPEAT_INFINITE); // Repeat indefinitely
        lv_anim_set_path_cb(&anim, lv_anim_path_ease_in_out);
        lv_anim_set_user_data(&anim, instance);
        lv_anim_set_completed_cb(&anim, [](lv_anim_t *anim) -> void {
            auto* inst = (MenuViewBase*)lv_anim_get_user_data(anim);
            inst->animating = false;
        });

        // Start the animation
        lv_anim_start(&anim);

        instance->dot_current_angle = new_option->angle;


    }

    if (new_option != nullptr && last_option != nullptr) {
        lv_label_set_text(instance->menu_option_label_alternative, last_option->title.c_str());
        lv_label_set_text(instance->menu_option_label, new_option->title.c_str());

        lv_anim_t anim;
        lv_anim_init(&anim);
        lv_anim_set_time(&anim, 120); // Duration of the animation (2 seconds)
        lv_anim_path_ease_in_out(&anim);

        int to_val = 100;
        if (instance->menu_index > instance->last_menu_index) {
            to_val = -100;
        }

        lv_anim_set_var(&anim, instance->menu_option_label_alternative);
        lv_anim_set_exec_cb(&anim, animate_option_label);
        lv_anim_set_values(&anim, 0, to_val);
        lv_anim_start(&anim);

        lv_anim_set_var(&anim, instance->menu_option_label);
        lv_anim_set_exec_cb(&anim, animate_option_label);
        lv_anim_set_values(&anim, to_val * -1, 0);
        lv_anim_start(&anim);
    }
}

void MenuViewBase::handle_rotary_event(RotaryEventEnum event) {
    if (animating) return;
    animating = true;
    last_menu_index = menu_index;
    auto size = options.size();
    if (event == CLOCKWISE) {
        if (menu_index + 1 >= size) {
            menu_index = 0;
        } else {
            menu_index++;
        }
    } else {
        if (menu_index == 0) {
            menu_index = size - 1;
        } else {
            menu_index--;
        }
    }

    lv_async_call(&update_menu_async, static_cast<void*>(this));
}


void exec_cb(void * var, int32_t value) {
    auto *obj = static_cast<lv_obj_t *>(var);
    lv_obj_set_style_opa(obj, (lv_opa_t)value, 0);
//    ESP_LOGW("Click", "Animation tick");
//    printf("v: %lo\n", value);
}

typedef struct {
    lv_obj_t* obj;
    bool to_open;
} menu_anim_ctx;

void anim_overlay(void* args) {
    auto ctx = (menu_anim_ctx*)args;
    auto overlay_obj = ctx->obj;
    auto to_open = ctx->to_open;

    lv_anim_t anim;

    lv_anim_init(&anim);
    lv_anim_set_var(&anim, overlay_obj);
    lv_anim_set_time(&anim, 150);  // Duration of the animation in milliseconds
    lv_anim_set_delay(&anim, 0);

    uint8_t from = to_open ? 0 : 255;
    uint8_t to = to_open ? 255 : 0;
    lv_anim_set_values(&anim, from, to);  // Animate from 0 to 255
    lv_anim_set_exec_cb(&anim, &exec_cb);  // Set the exec callback
    lv_anim_set_path_cb(&anim, lv_anim_path_ease_in_out);

    // Add an animation ready callback to free the memory after it's done
    lv_anim_set_user_data(&anim, ctx);
    lv_anim_set_ready_cb(&anim, [](lv_anim_t* anim){
        auto ctx = (menu_anim_ctx*)anim->user_data;
        if (ctx->to_open) {
            printf("menu visible\n");
            lv_obj_remove_flag(ctx->obj, LV_OBJ_FLAG_HIDDEN);
        } else {
            printf("menu invisible\n");
            lv_obj_add_flag(ctx->obj, LV_OBJ_FLAG_HIDDEN);
        }
        free(ctx);
    });

    lv_anim_start(&anim);  // Start the animation
}

void MenuViewBase::handle_click_event(ButtonEventEnum event) {
    printf("click! menu index: %d\n", menu_index);
    auto option_enum = options_vec[menu_index];
    handle_option_click(option_enum);
}

void MenuViewBase::open() {
    printf("menu visible\n");
    lv_obj_remove_flag(parent_obj, LV_OBJ_FLAG_HIDDEN);
    // Allocate memory for the animation context
    auto* ctx = (menu_anim_ctx*)malloc(sizeof(menu_anim_ctx));
    if (ctx == NULL) {
        printf("Failed to allocate memory for menu_anim_ctx");
        return;
    }

    // Set up context for opening animation
    ctx->obj = parent_obj;
    ctx->to_open = true;

    // Pass ctx, not &ctx
    lv_async_call(&anim_overlay, static_cast<void*>(ctx));
}

void MenuViewBase::close() {

    // Allocate memory for the animation context
    auto* ctx = (menu_anim_ctx*)malloc(sizeof(menu_anim_ctx));
    if (ctx == NULL) {
        printf("Failed to allocate memory for menu_anim_ctx");
        return;
    }

    // Set up context for closing animation
    ctx->obj = parent_obj;
    ctx->to_open = false;

    // Pass ctx, not &ctx
    lv_async_call(&anim_overlay, static_cast<void*>(ctx));
}

