#include <driver/i2c.h>
#include "esp_log.h"
#include "bridge_driver.h"
#include "iot_knob.h"
#include "iot_button.h"
#include "led_strip_types.h"
#include "led_strip.h"

#define TAG "BridgeDriver"

led_strip_handle_t led_strip;


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

    drv.setWaveform(0, 4);  // play effect
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

void led_ring_task(void *pvParameters) {
    auto *pixels = (std::vector<LedPixelColor> *)pvParameters;

    // Ensure the vector is valid
    if (!pixels || pixels->empty()) {
        vTaskDelete(NULL);  // Delete task if there's nothing to process
        return;
    }

    // Iterate over the pixel data and set the LED colors
    for (int i = 0; i < pixels->size(); ++i) {
        auto pixel = (*pixels)[i];
        printf("rgb: %d; %d; %d\n", pixel.red, pixel.green, pixel.blue);
        ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, i, pixel.red, pixel.green, pixel.blue));
    }

    ESP_ERROR_CHECK(led_strip_refresh(led_strip));

    // Task cleanup after execution
    vTaskDelete(NULL);  // Delete task after the work is done
}


void led_ring_callback(const std::vector<LedPixelColor>& pixels) {
    if (pixels.empty()) return;  // Check if the vector is empty

    // Allocate the pixels data on the heap to pass it to the task
    auto *task_pixels = new std::vector<LedPixelColor>(pixels);

    // Create a task to run the LED update asynchronously
    BaseType_t result = xTaskCreate(
            led_ring_task,       // Task function
            "LedRingTask",       // Name of the task (for debugging)
            4048,                // Stack size in words (adjust as needed)
            task_pixels,         // Task parameters (pixels to update)
            1,                   // Task priority (adjust as needed)
            NULL                 // Task handle (not used here)
    );

    // Handle task creation failure
    if (result != pdPASS) {
        printf("Failed to create LED ring task\n");
        delete task_pixels;  // Clean up memory if the task fails
    }
}

void click_cb(void *arg, void *data) {
    auto event =  iot_button_get_event(arg);
    auto bridge = static_cast<UiBridge*>(data);
    printf("clcik1\n");
    ESP_LOGW("Click", "click callback");
    esp_log_timestamp();

    ButtonEventEnum bridge_enum;
    if (event == BUTTON_PRESS_END) {
        bridge_enum = SINGLE_CLICK;
    } else {
        ESP_LOGW(TAG, "ERROR: INVALID EVENT: %i\n", event);
        return;
    }

    bridge->trigger_click_change(bridge_enum);

//    xTaskCreate(
//            blink,    // Function that should be called
//            "Click LED",   // Name of the task (for debugging)
//            4000,            // Stack size (bytes)
//            NULL,            // Parameter to pass
//            3,               // Task priority
//            NULL             // Task handle
//    );
}

void setup_button(UiBridge* bridge) {
    button_config_t cfg = {
            .type = BUTTON_TYPE_GPIO,
            .long_press_time = CONFIG_BUTTON_LONG_PRESS_TIME_MS,
            .short_press_time = 100,
            .gpio_button_config = {
                    .gpio_num = BUTTON_PIN,
                    .active_level = 0,

            }
    };

    auto btn = iot_button_create(&cfg);

    if(NULL == btn) {
        ESP_LOGE(TAG, "Button create failed");
    }

    iot_button_register_cb(btn, BUTTON_PRESS_END, click_cb, static_cast<void*>(bridge));
//    iot_button_register_cb(btn, BUTTON_PRESS_UP, click_cb, static_cast<void*>(bridge));
//    iot_button_register_cb(btn, BUTTON_PRESS_REPEAT, click_cb, static_cast<void*>(bridge));
//    iot_button_register_cb(btn, BUTTON_PRESS_REPEAT_DONE, click_cb, static_cast<void*>(bridge));
//    iot_button_register_cb(btn, BUTTON_SINGLE_CLICK, click_cb, static_cast<void*>(bridge));
//    iot_button_register_cb(btn, BUTTON_DOUBLE_CLICK, click_cb, static_cast<void*>(bridge));
//    iot_button_register_cb(btn, BUTTON_MULTIPLE_CLICK, click_cb, static_cast<void*>(bridge));
//    iot_button_register_cb(btn, BUTTON_LONG_PRESS_START, click_cb, static_cast<void*>(bridge));
//    iot_button_register_cb(btn, BUTTON_LONG_PRESS_HOLD, click_cb, static_cast<void*>(bridge));
//    iot_button_register_cb(btn, BUTTON_LONG_PRESS_UP, click_cb, static_cast<void*>(bridge));
//    iot_button_register_cb(btn, BUTTON_PRESS_END, click_cb, static_cast<void*>(bridge));

}

void setup_bridge_driver(UiMain* ui) {
    led_strip_config_t strip_config = {
            .strip_gpio_num = LED_PIN, // The GPIO that connected to the LED strip's data line
            .max_leds = 12, // The number of LEDs in the strip,
            .led_pixel_format = LED_PIXEL_FORMAT_GRB, // Pixel format of your LED strip
            .led_model = LED_MODEL_SK6812, // LED strip model
//            .flags.invert_out = false, // whether to invert the output signal (useful when your hardware has a level inverter)
    };

    led_strip_rmt_config_t rmt_config = {
            .clk_src = RMT_CLK_SRC_DEFAULT, // different clock source can lead to different power consumption
            .resolution_hz = 10 * 1000 * 1000, // 10MHz
            .flags = {
                    .with_dma = false
            }

    };

    ESP_ERROR_CHECK(led_strip_new_rmt_device(&strip_config, &rmt_config, &led_strip));

    for (int i = 0; i < 12; i++) {
        ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, i, 0, 0,0));
    }

    ESP_ERROR_CHECK(led_strip_refresh(led_strip));


    setup_rotary_encoder(&ui->bridge);
    setup_button(&ui->bridge);
    setup_haptic(&ui->bridge);

    ui->bridge.set_led_ring_callback((&led_ring_callback));
}