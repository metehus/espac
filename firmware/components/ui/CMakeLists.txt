idf_component_register(SRCS
        "ui_main.cpp"
        "bridge.cpp"
        "utils.cpp"
        "structure/view.cpp"
        "structure/view_base.cpp"
        "structure/view_orchestrator.cpp"
        "structure/main_event_bus.cpp"
        "views/thermostat_view.cpp"
        "views/thermostat_overlay_menu_view.cpp"
        "views/settings_menu_view.cpp"
        "views/mode_menu_view.cpp"
        "structure/menu_view_base.cpp"
        "assets/images/generated"

        "assets/images/generated/air_icon.c"
        "assets/images/generated/arrow_back_icon.c"
        "assets/images/generated/fan_icon.c"
        "assets/images/generated/freeze_icon.c"
        "assets/images/generated/heat_icon.c"
        "assets/images/generated/power_icon.c"
        "assets/images/generated/settings_icon.c"

        "assets/fonts/"
        "."
        INCLUDE_DIRS "." "assets/fonts" "assets/images"
)


lvgl_port_create_c_image("assets/images/air_icon.png" "assets/images/generated" "A8" "NONE")
lvgl_port_create_c_image("assets/images/arrow_back_icon.png" "assets/images/generated" "A8" "NONE")
lvgl_port_create_c_image("assets/images/fan_icon.png" "assets/images/generated" "A8" "NONE")
lvgl_port_create_c_image("assets/images/freeze_icon.png" "assets/images/generated" "A8" "NONE")
lvgl_port_create_c_image("assets/images/heat_icon.png" "assets/images/generated" "A8" "NONE")
lvgl_port_create_c_image("assets/images/power_icon.png" "assets/images/generated" "A8" "NONE")
lvgl_port_create_c_image("assets/images/settings_icon.png" "assets/images/generated" "A8" "NONE")

lvgl_port_add_images(${COMPONENT_LIB} "assets/images/")