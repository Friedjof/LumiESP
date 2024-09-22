#ifndef MqttService_h
#define MqttService_h


#include <WiFi.h>
#include <PubSubClient.h>

#include "../../config/config.h"


class MqttService
{
    private:
        WiFiClient wifiClient;
        PubSubClient mqttClient;

        bool initialized = false;
    public:
        MqttService();

        void setup();
        void loop();

        void connectToWiFi();
        void mqttReconnect();
        void mqttStatusUpdate();
        void mqttGlobalTopic(const char* subTopic, char* globalTopic);
        void mqttDatetimeUpdate(const char* datetime);
        void publish(const char* subTopic, const char* message);

        void mqttLoop() { this->mqttClient.loop(); }
        bool mqttIsConnected() { return this->mqttClient.connected(); }
};

#endif