idf_component_register(SRCS "ui_main.cpp" "views/thermostat_view.cpp" "bridge.cpp"
        INCLUDE_DIRS "." "assets/fonts" "assets/images")

lvgl_port_create_c_image("assets/images/freeze.png" "assets/images/" "RGB565" "NONE")
lvgl_port_add_images(${COMPONENT_LIB} "assets/images/")