idf_component_register(SRCS "main.cpp" "drivers/display/driver.cpp" "drivers/bridge_driver.cpp" "pins.cpp"
                       INCLUDE_DIRS "drivers/display"
                       REQUIRES "ui"
)