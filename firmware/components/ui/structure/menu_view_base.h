#ifndef ESPAC_FIRMWARE_MENU_VIEW_BASE_H
#define ESPAC_FIRMWARE_MENU_VIEW_BASE_H
#include "structure/view.h"
#include "assets/images/generated/images.h"
#include "lvgl.h"
#include <map>
#include <string>

typedef struct {
    lv_image_dsc_t icon_src;
    lv_obj_t* obj;
    double angle;
    std::string title;
} MenuOption;

class MenuViewBase: public View {
public:
    lv_obj_t* parent_obj;

    lv_obj_t* menu_option_label;
    lv_obj_t* menu_option_label_alternative;
    lv_obj_t* dot_indicator_obj;

    uint8_t menu_index = 0;
    uint8_t last_menu_index = 0;
    double dot_current_angle = 0;

    std::map<uint16_t, MenuOption> options;
    std::vector<uint16_t> options_vec;

    bool animating = false;

    void create_menu();

    virtual void handle_option_click(uint16_t option);

    virtual std::map<uint16_t, MenuOption> populate_options() = 0;


    void open();
    void close();

    void register_events() override;

    void handle_rotary_event(RotaryEventEnum event) override;

    void handle_click_event(ButtonEventEnum event) override;

};






#endif //ESPAC_FIRMWARE_MENU_VIEW_BASE_H
