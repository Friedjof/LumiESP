#ifndef MqttService_h
#define MqttService_h


#include <map>
#include <functional>

#include <WiFi.h>
#include <AsyncTCP.h>
#include <espMqttClient.h>

#include "../../include/CommonTypes.h"
#include "../../config/config.h"


class MqttService
{
    private:
        WiFiClient wifiClient;
        espMqttClient mqttClient;

        std::map<String, mode_callback_t> modeTopics;

        bool initialized = false;

        void onSubscribe(uint16_t packetId, const espMqttClientTypes::SubscribeReturncode* codes, size_t len);
        void onMessage(const espMqttClientTypes::MessageProperties& properties, const char* topic, const uint8_t* payload, size_t len, size_t index, size_t total);

        void subscribe(const char* subTopic);
        void subscribe(String subTopic);

        void logWarning(String message);

    public:
        MqttService();

        void setup(); void initTopics(); void loop(); void connect();

        void connectToWiFi(); void mqttStatusUpdate(); void mqttDatetimeUpdate(const char* datetime);

        std::function<void(String payload)> subscribeModeTopic(String modeName, String localTopic, String defaultPayload, boundaries_t boundaries, payload_type_e payloadType, std::function<void(String payload)> topicCallback);
        std::function<void(String payload)> subscribeModeTopic(String modeName, String localTopic, String defaultPayload, payload_type_e payloadType, std::function<void(String payload)> topicCallback);
        std::function<void(String payload)> subscribeModeTopic(String modeName, String localTopic, String defaultPayload, std::function<void(String payload)> topicCallback);
        std::function<void(String payload)> subscribeModeTopic(String modeName, String localTopic, int defaultPayload, boundaries_t boundaries, payload_type_e payloadType, std::function<void(String payload)> topicCallback);
        std::function<void(String payload)> subscribeModeTopic(String modeName, String localTopic, int defaultPayload, boundaries_t boundaries, std::function<void(String payload)> topicCallback);
        std::function<void(String payload)> subscribeModeTopic(String modeName, String localTopic, int defaultPayload, std::function<void(String payload)> topicCallback);
        std::function<void(String payload)> subscribeModeTopic(String modeName, String localTopic, float defaultPayload, std::function<void(String payload)> topicCallback);

        void publish(const char* subTopic, const char* message); void publish(String subTopic, const char* message); void publish(String subTopic, String message);

        bool onMessageCallback(String topic, String payload); void setOnMessageCallback(void (*callback)(const espMqttClientTypes::MessageProperties& properties, const char* topic, const uint8_t* payload, size_t len, size_t index, size_t total));

        // helper functions
        String mqttGlobalTopic(const char* subTopic);
        String mqttGlobalTopic(String subTopic);
        
        bool isConnected() { return this->mqttClient.connected(); }
        bool isInitialized() { return this->initialized; }
        bool isValidHexColor(String hex);
};

#endif