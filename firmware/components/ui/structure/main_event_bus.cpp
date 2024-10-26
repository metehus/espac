#include "main_event_bus.h"
void MainEventBus::subscribe(const std::string& event, std::function<void(void*)> listener) {
    listeners[event].emplace_back(std::move(listener));
}

void MainEventBus::trigger(const std::string& event, void* arg) {
    if (listeners.count(event)) {
        for (const auto& listener : listeners[event]) {
            listener(arg);
        }
    } else {
        printf("WARN: no listener for %s\n", event.c_str());
    }
}