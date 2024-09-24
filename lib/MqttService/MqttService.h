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

        String mqttLedModeOnSubTopic(const char* subTopic);
        String mqttLedModeOnPubTopic(const char* subTopic);

        String mqttLedModeOffSubTopic(const char* subTopic);
        String mqttLedModeOffPubTopic(const char* subTopic);

        String mqttLedModeCustomSubTopic(const char* subTopic);
        String mqttLedModeCustomPubTopic(const char* subTopic);

        String mqttLedModeBlinkSubTopic(const char* subTopic);
        String mqttLedModeBlinkPubTopic(const char* subTopic);

        String mqttLedModeFadeSubTopic(const char* subTopic);
        String mqttLedModeFadePubTopic(const char* subTopic);

        String mqttLedModeRainbowSubTopic(const char* subTopic);
        String mqttLedModeRainbowPubTopic(const char* subTopic);

        String mqttLedModeLoopSubTopic(const char* subTopic);
        String mqttLedModeLoopPubTopic(const char* subTopic);

        bool isLedModeSubTopic(String topic);
        bool isLedModeCustomSubTopic(String topic);
        bool isLedModeCustomHexSubTopic(String topic);
        bool isLedModeCustomBrightnessSubTopic(String topic);
        
        bool isConnected() { return this->mqttClient.connected(); }
        bool isInitialized() { return this->initialized; }
};

#endif