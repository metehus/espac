#include <cstdio>
#include "esp_log.h"
#include "drivers/display/driver.h"
#include "drivers/bridge_driver.h"

extern "C" void app_main(void){
    printf("OII\n");
    ESP_LOGI("Main", "Starting espac firware!");

    auto ui = new UiMain();

    auto dsp_driver = DisplayDriver();

    setup_bridge_driver(ui);

    dsp_driver.init(ui);


//    i2c_master_bus_handle_t i2c_handle;
//    i2c_port_t i2c_port = I2C_NUM_0;
//
//    ESP_LOGI("Main", "A!");
//    i2c_master_bus_config_t i2c_bus_config = {
//            .i2c_port = i2c_port,
//            .sda_io_num = HAPTIC_SDA,
//            .scl_io_num = HAPTIC_SCL,
//            .clk_source = I2C_CLK_SRC_DEFAULT,
//            .glitch_ignore_cnt = 7,
//            .flags = {
//                    .enable_internal_pullup = true
//            },
//    };
//
//
//    ESP_LOGI("Main", "B!");
//    SensorDRV2605 drv;
//
//    ESP_LOGI("Main", "C!");
//    ESP_ERROR_CHECK(i2c_new_master_bus(&i2c_bus_config, &i2c_handle));
//    ESP_LOGI("Main", "D!");
//    if (!drv.begin(i2c_handle, 0x5A)) {  // 0x5A is the default address for DRV2605
//        ESP_LOGE("A", "Failed to initialize DRV2605");
//        return;
//    }
//
//    ESP_LOGI("Main", "E!");
//    drv.selectLibrary(1);
//    ESP_LOGI("Main", "F!");
//    drv.setMode(DRV2605_MODE_INTTRIG);
//
//    ESP_LOGI("Main", "G!");
//    drv.setWaveform(0, 62);  // play effect
//    ESP_LOGI("Main", "H!");
//    drv.setWaveform(1, 0);       // end waveform
//    ESP_LOGI("Main", "I!");
//
//    // play the effect!
////    drv.run();
//
//    while (true) {
//        ESP_LOGI("Main", "vibrate!");
//        vTaskDelay(1000 / portTICK_PERIOD_MS);
//
//        drv.run();
//    }
}
