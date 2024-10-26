#include "../events.h"
#include "view_base.h"
#include "view_orchestrator.h"
#include "main_event_bus.h"

#ifndef ESPAC_FIRMWARE_VIEW_H
#define ESPAC_FIRMWARE_VIEW_H

class View: public ViewBase {
public:
    ViewOrchestrator* orchestrator;
    MainEventBus* event_bus;
};


#endif //ESPAC_FIRMWARE_VIEW_H
