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
        void createTopics();
        void loop();

        bool isInitialized() { return this->initialized; }
        bool isConnected() { return this->mqttClient.connected(); }

        void connectToWiFi();
        void mqttReconnect();
        void mqttStatusUpdate();
        void mqttDatetimeUpdate(const char* datetime);

        void publish(const char* subTopic, const char* message);
        void publish(String subTopic, const char* message);
        void publish(String subTopic, String message);

        void callback(char *topic, byte *payload, unsigned int length);
        void setCallback(void (*callback)(char*, byte*, unsigned int));

        // helper functions
        String mqttGlobalTopic(const char* subTopic);
        String mqttStatusTopic(const char* subTopic);
        String mqttLedPubTopic(const char* subTopic);
        String mqttLedSubTopic(const char* subTopic);

        bool isLedTopic(const char* topic);
        bool isLedTopic(String topic);

        void mqttLoop() { this->mqttClient.loop(); }
        bool mqttIsConnected() { return this->mqttClient.connected(); }
};

#endif