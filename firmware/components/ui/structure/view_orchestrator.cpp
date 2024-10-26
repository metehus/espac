#include "view_orchestrator.h"

void ViewOrchestrator::register_view(char* key, ViewBase *view) {
    views[key] = view;
}