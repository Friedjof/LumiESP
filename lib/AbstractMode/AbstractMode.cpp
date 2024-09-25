#include "AbstractMode.h"


AbstractMode::AbstractMode(LedService* ledService, LoggingService* loggingService, MqttService* mqttService)
{
    this->ledService = ledService;
    this->loggingService = loggingService;
    this->mqttService = mqttService;
}

AbstractMode::~AbstractMode()
{
}

void AbstractMode::setup()
{
    // call custom setup
    this->customSetup();

    this->loggingService->logMessage(LOG_LEVEL_INFO, "Mode setup completed for: " + this->modeInternalName);
}

void AbstractMode::loop(const int steps)
{
    this->customLoop(steps);
}
