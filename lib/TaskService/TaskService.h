#ifndef TASKSERVICE_H
#define TASKSERVICE_H

#include "MqttService.h"
#include "LoggingService.h"
#include "ClockService.h"

#include "../../config/config.h"


class TaskService
{
    private:
        WiFiClient wifiClient;
        PubSubClient mqttClient;

        ClockService clockService;
        LoggingService loggingService;

        bool isWiFiConnected = false;

        bool initialized = false;
    public:
        TaskService(MqttService mqttService, ClockService clockService, LoggingService loggingService);

        void setup();

        void initWiFi();

        void mqttReconnect();
        void mqttStatusUpdate();
        void loop();
        void mqttLoop() { this->mqttClient.loop(); }
        bool mqttIsConnected() { return this->mqttClient.connected(); }
};

#endif