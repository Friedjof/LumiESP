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

        bool isInitialized() { return this->initialized; }
        bool isConnected() { return this->mqttClient.connected(); }

        void connectToWiFi();
        void mqttReconnect();
        void mqttStatusUpdate();
        void mqttGlobalTopic(const char* subTopic, char* globalTopic);
        void mqttDatetimeUpdate(const char* datetime);
        void publish(const char* subTopic, const char* message);
        void callback(char *topic, byte *payload, unsigned int length);
        void setCallback(void (*callback)(char*, byte*, unsigned int));

        void mqttLoop() { this->mqttClient.loop(); }
        bool mqttIsConnected() { return this->mqttClient.connected(); }
};

#endif