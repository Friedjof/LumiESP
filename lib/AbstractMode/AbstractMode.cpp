#include "AbstractMode.h"


AbstractMode::AbstractMode(ControllerService* controllerService)
{
    this->controllerService = controllerService;
}

AbstractMode::~AbstractMode()
{
}

void AbstractMode::setup()
{
    // call custom setup
    this->customSetup();

    this->controllerService->logMessage(LOG_LEVEL_INFO, LOG_MODE_ALL, "Mode setup completed for: " + this->modeInternalName);
}

void AbstractMode::loop(const int steps)
{
    this->customLoop(steps);
}
