#include "ControllerService.h"


ControllerService::ControllerService(MqttService *mqttService, ClockService *clockService, LoggingService *loggingService, LedService *ledService)
{
    this->mqttService = mqttService;
    this->clockService = clockService;
    this->loggingService = loggingService;
    this->ledService = ledService;
}

void ControllerService::setup()
{
    this->initialized = true;
}

// ------- CONTROLLER METHODS -------
void ControllerService::setMode(String mode)
{
    if (!this->initialized)
    {
        this->loggingService->logMessage(LOG_LEVEL_WARN, LOG_MODE_ALL, "Controller Service not initialized");
        return;
    }

    this->ledService->setMode(mode);
}

// ------- TASK WRAPPERS -------
void ControllerService::mqttServiceStatusUpdateWrapper()
{
    this->loggingService->updateEspStatus();
}

void ControllerService::mqttServiceLoopWrapper()
{
    this->mqttService->loop();
}

void ControllerService::mqttServiceUpdateDateTimeWrapper()
{
    this->loggingService->updateStatusDateTime();
}

void ControllerService::mqttServiceCallbackWrapper(const espMqttClientTypes::MessageProperties& properties, const char* topic, const uint8_t* payload, size_t len, size_t index, size_t total)
{
    String playloadStr = "";

    for (int i = 0; i < len; i++)
    {
        playloadStr += (char)payload[i];
    }

    this->loggingService->logMessage(LOG_LEVEL_DEBUG, LOG_MODE_SERIAL, "MQTT message received: " + String(topic) + " > " + playloadStr);

    bool valid = this->mqttService->onMessageCallback(topic, playloadStr);

    if (!valid)
    {
        this->loggingService->logMessage(LOG_LEVEL_WARN, LOG_MODE_ALL, "MQTT topic or payload invalid: " + String(topic) + " > " + playloadStr);
    }
}

void ControllerService::clockServiceTimeSyncWrapper()
{
    this->clockService->syncTime();
}

void ControllerService::ledServiceLoopWrapper()
{
    this->ledService->loop();
}
