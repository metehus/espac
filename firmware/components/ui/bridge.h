#ifndef ESPAC_FIRMWARE_BRIDGE_H
#define ESPAC_FIRMWARE_BRIDGE_H
#include "events.h"
#include "iui_main.h"

typedef void (*EventCallback)(void*);

typedef struct {
    EventCallback rotary_change_callback;
} BridgeInternalBindings;

typedef struct {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
} LedPixelColor;

typedef void (*LedRingCallback)(std::vector<LedPixelColor>);

class UiBridge {
private:
    IUiMain& parent_ui;
    std::function<void(std::vector<LedPixelColor>)> led_ring_change_callback;
public:
    explicit UiBridge(IUiMain& parent_ui): parent_ui(parent_ui) {};

    void trigger_rotary_change(RotaryEventEnum event);
    void trigger_click_change(ButtonEventEnum event);

    void update_led_ring(std::vector<LedPixelColor>& pixels);

    void set_led_ring_callback(std::function<void(std::vector<LedPixelColor>)> callback);
};

#endif //ESPAC_FIRMWARE_BRIDGE_H
