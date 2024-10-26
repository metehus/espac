#include "bridge.h"
#include <cstdio>
#include <utility>
#include <esp_log.h>

void UiBridge::trigger_rotary_change(RotaryEventEnum event) {
    parent_ui.handle_rotary_event(event);
}

void UiBridge::trigger_click_change(ButtonEventEnum event) {
    ESP_LOGW("Click", "click bridge");
    esp_log_timestamp();
    parent_ui.handle_click_event(event);
}

void UiBridge::update_led_ring(std::vector<LedPixelColor>& pixels) {
    if (!pixels.empty()) {
        printf("a: %d", pixels[0].red);  // Ensure we don't access an empty vector
    }

    if (led_ring_change_callback == nullptr) {
        printf("CALLBACK IS NULL");
    } else {
        led_ring_change_callback(pixels);  // Call the callback
    }
}

void UiBridge::set_led_ring_callback(std::function<void(std::vector<LedPixelColor>)> callback) {
    led_ring_change_callback = std::move(callback);  // You could add a null check if needed
}

