#ifndef MqttService_h
#define MqttService_h


#include <WiFi.h>
#include <AsyncTCP.h>
#include <espMqttClient.h>

#include "../../config/config.h"


class MqttService
{
    private:
        WiFiClient wifiClient;
        espMqttClient mqttClient;

        bool initialized = false;

        void onConnect(bool sessionPresent);
        void onSubscribe(uint16_t packetId, const espMqttClientTypes::SubscribeReturncode* codes, size_t len);
        void onMessage(const espMqttClientTypes::MessageProperties& properties, const char* topic, const uint8_t* payload, size_t len, size_t index, size_t total);

        void subscribe(const char* subTopic);
        void subscribe(String subTopic);
    public:
        MqttService();

        void setup();
        void createTopics();
        void loop();
        void connect();

        void connectToWiFi();
        void mqttStatusUpdate();
        void mqttDatetimeUpdate(const char* datetime);

        void subscribe();

        void publish(const char* subTopic, const char* message);
        void publish(String subTopic, const char* message);
        void publish(String subTopic, String message);

        void setCallback(void (*callback)(const espMqttClientTypes::MessageProperties& properties, const char* topic, const uint8_t* payload, size_t len, size_t index, size_t total));

        // helper functions
        String mqttGlobalTopic(const char* subTopic);
        String mqttStatusTopic(const char* subTopic);
        String mqttLedPubTopic(const char* subTopic);
        String mqttLedSubTopic(const char* subTopic);
        String mqttLedModeCustomTopic(const char* subTopic);

        bool isLedModeSubTopic(String topic);
        bool isLedModeCustomSubTopic(String topic);
        bool isLedModeCustomRSubTopic(String topic);
        bool isLedModeCustomGSubTopic(String topic);
        bool isLedModeCustomBSubTopic(String topic);
        bool isLedModeCustomBrightnessSubTopic(String topic);
        
        bool isConnected() { return this->mqttClient.connected(); }
        bool isInitialized() { return this->initialized; }
};

#endif