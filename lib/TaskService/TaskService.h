#ifndef TASKSERVICE_H
#define TASKSERVICE_H

#include "MqttService.h"
#include "LoggingService.h"
#include "ClockService.h"

#include "../../config/config.h"


class TaskService
{
    private:
        MqttService *mqttService;
        ClockService *clockService;
        LoggingService *loggingService;

        bool initialized = false;
    public:
        TaskService(MqttService *mqttService, ClockService *clockService, LoggingService *loggingService);

        void setup();

        void mqttStatusUpdate();
};

#endif