#ifndef ESPAC_FIRMWARE_MAIN_EVENT_BUS_H
#define ESPAC_FIRMWARE_MAIN_EVENT_BUS_H
#include <unordered_map>
#include <string>
#include <functional>
#include <utility>


class MainEventBus {
    // Map to hold listeners for each event, which can take arguments
    std::unordered_map<std::string, std::vector<std::function<void(void*)>>> listeners;

public:
    // Subscribe to an event with a listener that takes two parameters
    void subscribe(const std::string& event, std::function<void(void*)> listener);

    // Trigger all listeners for an event, passing arguments
    void trigger(const std::string& event, void* arg);
};


#endif //ESPAC_FIRMWARE_MAIN_EVENT_BUS_H
