#ifndef CONTROLLERSERVICE_H
#define CONTROLLERSERVICE_H

#include <functional>

#include "MqttService.h"
#include "ClockService.h"
#include "LoggingService.h"
#include "LedService.h"

#include "CommonTypes.h"
#include "../../config/config.h"


class ControllerService
{
    private:
        MqttService *mqttService;
        ClockService *clockService;
        LoggingService *loggingService;
        LedService *ledService;

        bool initialized = false;
    public:
        ControllerService(MqttService *mqttService, ClockService *clockService, LoggingService *loggingService, LedService *ledService);

        void setup();

        void mqttSubscribtion(String topic, String payload);

        // controller methods
        void setMode(String mode);

        // task wrappers
        void mqttServiceStatusUpdateWrapper();
        void mqttServiceLoopWrapper();
        void mqttServiceUpdateDateTimeWrapper();
        void mqttServiceCallbackWrapper(const espMqttClientTypes::MessageProperties& properties, const char* topic, const uint8_t* payload, size_t len, size_t index, size_t total);
        void clockServiceTimeSyncWrapper();
        void ledServiceLoopWrapper();

        // LedService methods
        void registerMode(String name, std::function<void(int steps)> mode);
        void unregisterMode(String name);

        void setLed(short index, byte r, byte g, byte b);
        void setLed(short index, CRGB color);
        void setLed(short index, CHSV color);
        void setLed(byte r, byte g, byte b);

        void setHexColor(String hexColor);
        void setBrightness(byte brightness);

        void confirmLedConfig();

        // MqttService methods
        std::function<void(String payload)> subscribeModeTopic(String modeName, String localTopic, String defaultPayload, boundaries_t boundaries, payload_e payloadType, topic_e topicType, std::function<void(String payload)> topicCallback);
        // LumiEsp
        std::function<void(String payload)> subscribeModeTopic(String modeName, String localTopic, payload_e payloadType, topic_e topicType);
        std::function<void(String payload)> subscribeModeTopic(String modeName, String localTopic);
        std::function<void(String payload)> subscribeModeTopic(String modeName, String localTopic, const char* defaultPayload, payload_e payloadType, topic_e topicType, std::function<void(String payload)> topicCallback);
        // StaticMode
        std::function<void(String payload)> subscribeModeTopic(String modeName, String localTopic, const char* defaultPayload, payload_e payloadType, std::function<void(String payload)> topicCallback);
        std::function<void(String payload)> subscribeModeTopic(String modeName, String localTopic, int defaultPayload, boundaries_t boundaries, payload_e payloadType, std::function<void(String payload)> topicCallback);

        // LoggingService methods
        void logMessage(short logLevel, short mode, const char* message);
        void logMessage(short logLevel, short mode, String message);
        void registerMqttLogFun(std::function<void(const char* message)> mqttLogMessage);
        void registerGetDatetimeFun(std::function<const char*()> getDatetime);
};

#endif