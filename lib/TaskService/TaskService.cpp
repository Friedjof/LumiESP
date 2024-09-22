#include "TaskService.h"


TaskService::TaskService(MqttService *mqttService, ClockService *clockService, LoggingService *loggingService)
{
    this->mqttService = mqttService;
    this->clockService = clockService;
    this->loggingService = loggingService;
}

void TaskService::setup()
{
    this->initialized = true;
}

void TaskService::mqttStatusUpdate()
{
    this->mqttService->mqttStatusUpdate();
    this->loggingService->logMessage(LOG_LEVEL_INFO, LOG_MODE_ALL, "MQTT Status Update");
}