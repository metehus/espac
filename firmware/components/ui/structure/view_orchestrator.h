#ifndef ESPAC_FIRMWARE_VIEW_ORCHESTRATOR_H
#define ESPAC_FIRMWARE_VIEW_ORCHESTRATOR_H
#include <map>
#include "view_base.h"
#include <string>

class ViewOrchestrator {
private:
    std::map<char*, ViewBase*> views;
public:
    void register_view(char*, ViewBase* view);

    template<typename T>
    T* get_view(char* key) {
        if (views.find(key) != views.end()) {
            return dynamic_cast<T*>(views[key]);
        }
        return nullptr;
    };
};


#endif //ESPAC_FIRMWARE_VIEW_ORCHESTRATOR_H
