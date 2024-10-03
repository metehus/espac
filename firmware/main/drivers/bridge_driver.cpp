#include <driver/i2c.h>
#include "esp_log.h"
#include "bridge_driver.h"
#include "iot_knob.h"

#define TAG "BridgeDriver"


SensorDRV2605 drv;

void cb(void *arg, void *data) {
    auto event =  iot_knob_get_event(arg);
    auto bridge = static_cast<UiBridge*>(data);

    RotaryEventEnum bridge_enum;
    if (event == KNOB_LEFT) {
        bridge_enum = COUNTER_CLOCKWISE;
    } else if (event == KNOB_RIGHT) {
        bridge_enum = CLOCKWISE;
    } else {
        ESP_LOGW(TAG, "ERROR: INVALID ROTARY EVENT: %i\n", event);
        return;
    }

    bridge->trigger_rotary_change(bridge_enum);

    drv.setWaveform(0, 62);  // play effect
    drv.setWaveform(1, 0);       // end waveform

    // play the effect!
    drv.run();
}

void setup_rotary_encoder(UiBridge* bridge) {
    knob_config_t encoder_a_b_config = {
            .default_direction = 1,
            .gpio_encoder_a = GPIO_ENCODER_A,
            .gpio_encoder_b = GPIO_ENCODER_B,

    };

    auto knob = iot_knob_create(&encoder_a_b_config);

    ESP_ERROR_CHECK(iot_knob_register_cb(knob, KNOB_LEFT, cb, static_cast<void*>(bridge)));
    ESP_ERROR_CHECK(iot_knob_register_cb(knob, KNOB_RIGHT, cb, static_cast<void*>(bridge)));
}

void setup_haptic(UiBridge* bridge) {
    i2c_master_bus_handle_t i2c_handle;
    i2c_port_t i2c_port = I2C_NUM_0;

//    i2c_config_t conf;
//    conf.mode = I2C_MODE_MASTER;
//    conf.sda_io_num = HAPTIC_SDA;
//    conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
//    conf.scl_io_num = HAPTIC_SCL;
//    conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
//    conf.master.clk_speed = 100000;

    i2c_master_bus_config_t i2c_bus_config = {
            .i2c_port = i2c_port,
            .sda_io_num = HAPTIC_SDA,
            .scl_io_num = HAPTIC_SCL,
            .clk_source = I2C_CLK_SRC_DEFAULT,
            .glitch_ignore_cnt = 7,
            .flags = {
                    .enable_internal_pullup = true
                    },
    };


    ESP_ERROR_CHECK(i2c_new_master_bus(&i2c_bus_config, &i2c_handle));

    // Retrieve the I2C bus handle
//    ret = i2c_get_bus_handle(i2c_port, &i2c_handle);
//    if (ret != ESP_OK) {
//        ESP_LOGE(TAG, "Failed to get I2C bus handle: %s", esp_err_to_name(ret));
//        return;
//    }

    if (!drv.begin(i2c_handle, 0x5A)) {  // 0x5A is the default address for DRV2605
        ESP_LOGE(TAG, "Failed to initialize DRV2605");
        return;
    }

    drv.selectLibrary(1);
    drv.setMode(DRV2605_MODE_INTTRIG);



}

void setup_bridge_driver(UiMain* ui) {
    setup_rotary_encoder(&ui->bridge);
    setup_haptic(&ui->bridge);

}