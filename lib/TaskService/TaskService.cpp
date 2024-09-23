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

void TaskService::mqttServiceCallbackWrapper(char *topic, byte *payload, unsigned int length)
{
    this->loggingService->logMessage(LOG_LEVEL_DEBUG, LOG_MODE_SERIAL, "MQTT callback triggered");

    this->mqttService->callback(topic, payload, length);

    this->loggingService->logMessage(LOG_LEVEL_DEBUG, LOG_MODE_SERIAL, "MQTT callback topic: " + String(topic));
}

void TaskService::clockServiceTimeSyncWrapper()
{
    this->clockService->syncTime();
}

void TaskService::ledServiceLoopWrapper()
{
    this->ledService->loop();
}