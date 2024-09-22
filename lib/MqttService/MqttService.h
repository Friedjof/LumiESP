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

        void mqttLoop() { this->mqttClient.loop(); }
        bool mqttIsConnected() { return this->mqttClient.connected(); }
        void publish(const char* topic, const char* message) { this->mqttClient.publish(topic, message); }
};

#endif