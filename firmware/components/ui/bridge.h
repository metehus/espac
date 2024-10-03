#ifndef ESPAC_FIRMWARE_BRIDGE_H
#define ESPAC_FIRMWARE_BRIDGE_H
#include "events.h"
#include "iui_main.h"

typedef void (*EventCallback)(void*);

typedef struct {
    EventCallback rotary_change_callback;
} BridgeInternalBindings;



class UiBridge {
private:
    IUiMain& parent_ui;
    EventCallback rotary_change_callback{};
public:
    explicit UiBridge(IUiMain& parent_ui): parent_ui(parent_ui) {};

    void trigger_rotary_change(RotaryEventEnum event);

    void internal_bind(BridgeInternalBindings bindings);
};

#endif //ESPAC_FIRMWARE_BRIDGE_H
