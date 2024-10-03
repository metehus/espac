#include "bridge.h"
#include <cstdio>

void UiBridge::trigger_rotary_change(RotaryEventEnum event) {
//    printf("rotary event1: %i\n", event);

    parent_ui.handle_rotary_event(event);
}

void UiBridge::internal_bind(BridgeInternalBindings bindings) {
    rotary_change_callback = bindings.rotary_change_callback;
}

