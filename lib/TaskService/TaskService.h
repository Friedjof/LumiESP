#ifndef TASKSERVICE_H
#define TASKSERVICE_H

#include "MqttService.h"
#include "LoggingService.h"
#include "LedService.h"
#include "ClockService.h"

#include "../../config/config.h"


class TaskService
{
    private:
        MqttService *mqttService;
        ClockService *clockService;
        LoggingService *loggingService;
        LedService *ledService;

        bool initialized = false;
    public:
        TaskService(MqttService *mqttService, ClockService *clockService, LoggingService *loggingService, LedService *ledService);

        void setup();

        // task wrappers
        void mqttServiceStatusUpdateWrapper();
        void mqttServiceLoopWrapper();
        void mqttServiceUpdateDateTimeWrapper();
        void mqttServiceCallbackWrapper(char* topic, byte* payload, unsigned int length);
        void clockServiceTimeSyncWrapper();
        void ledServiceLoopWrapper();

};

#endif