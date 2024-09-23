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

        void mqttSubscribtion(String topic, String payload);

        // task wrappers
        void mqttServiceStatusUpdateWrapper();
        void mqttServiceLoopWrapper();
        void mqttServiceUpdateDateTimeWrapper();
        void mqttServiceCallbackWrapper(const espMqttClientTypes::MessageProperties& properties, const char* topic, const uint8_t* payload, size_t len, size_t index, size_t total);
        void clockServiceTimeSyncWrapper();
        void ledServiceLoopWrapper();

};

#endif