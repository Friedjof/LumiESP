#include "TaskService.h"


TaskService::TaskService(MqttService *mqttService, ClockService *clockService, LoggingService *loggingService, LedService *ledService)
{
    this->mqttService = mqttService;
    this->clockService = clockService;
    this->loggingService = loggingService;
    this->ledService = ledService;
}

void TaskService::setup()
{
    this->initialized = true;
}

void TaskService::mqttSubscribtion(String topic, String payload)
{
    this->loggingService->logMessage(LOG_LEVEL_DEBUG, LOG_MODE_ALL, "MQTT callback topic: " + topic + ", payload: " + payload);

    if (this->mqttService->isLedModeSubTopic(topic))
    {
        LedModes mode = this->ledService->getModeEnum(payload);
        if (mode != MODE_NONE)
        {
            this->ledService->setMode(mode);
        } else {
            this->loggingService->logMessage(LOG_LEVEL_WARN, LOG_MODE_ALL, "LED Service requested mode is not valid: " + payload);
        }
    }
}

// ------- TASK WRAPPERS -------
void TaskService::mqttServiceStatusUpdateWrapper()
{
    this->mqttService->mqttStatusUpdate();
}

void TaskService::mqttServiceLoopWrapper()
{
    this->mqttService->loop();
}

void TaskService::mqttServiceUpdateDateTimeWrapper()
{
    char datetime[128];
    this->clockService->getDateTime(datetime);

    this->mqttService->mqttDatetimeUpdate(datetime);
}

void TaskService::mqttServiceCallbackWrapper(const espMqttClientTypes::MessageProperties& properties, const char* topic, const uint8_t* payload, size_t len, size_t index, size_t total)
{
    String playloadStr = "";

    for (int i = 0; i < len; i++)
    {
        playloadStr += (char)payload[i];
    }

    this->mqttSubscribtion(String(topic), playloadStr);
}

void TaskService::clockServiceTimeSyncWrapper()
{
    this->clockService->syncTime();
}

void TaskService::ledServiceLoopWrapper()
{
    this->ledService->loop();
}