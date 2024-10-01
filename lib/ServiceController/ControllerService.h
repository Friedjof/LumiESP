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

        String currentStatus = "none";

        std::function<void(const char* message)> pushStatusMessage = nullptr;
        std::function<void(const char* message)> pushDateTimeMessage = nullptr;

        bool initialized = false;

    public:
        ControllerService(MqttService *mqttService, ClockService *clockService, LoggingService *loggingService, LedService *ledService);

        void setup();

        void setStatus(String status);
        String getStatus();

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
        void registerMode(String name, std::function<void(int steps)> modeLoop, std::function<void()> enableFirstRun);
        void unregisterMode(String name);

        void setLed(short index, byte r, byte g, byte b);
        void setLed(short index, CRGB color);
        void setLed(short index, CHSV color);
        void setLed(short index, String hexColor);
        void setLed(byte r, byte g, byte b);
        void setHexColor(String hexColor);
        void setColor(CRGB color);
        void setBrightness(byte brightness);

        void confirmLedConfig();

        // MqttService methods
        std::function<void(String payload)> subscribeModeTopic(String modeName, String localTopic, String defaultPayload, boundaries_t boundaries, payload_e payloadType, topic_e topicType, std::function<void(String payload)> topicCallback);
        // LumiEsp
        std::function<void(String payload)> subscribeModeTopic(String modeName, String localTopic, payload_e payloadType, topic_e topicType);
        std::function<void(String payload)> subscribeModeTopic(String modeName, String localTopic, String defaultPayload);
        std::function<void(String payload)> subscribeModeTopic(String modeName, String localTopic);
        std::function<void(String payload)> subscribeModeTopic(String modeName, String localTopic, const char* defaultPayload, payload_e payloadType, topic_e topicType, std::function<void(String payload)> topicCallback);
        // StaticMode
        std::function<void(String payload)> subscribeModeTopic(String modeName, String localTopic, const char* defaultPayload, payload_e payloadType, std::function<void(String payload)> topicCallback);
        std::function<void(String payload)> subscribeModeTopic(String modeName, String localTopic, int defaultPayload, boundaries_t boundaries, payload_e payloadType, std::function<void(String payload)> topicCallback);

        // LoggingService methods
        void logMessage(short logLevel, short mode, const char* message);
        void logMessage(short logLevel, short mode, String message);

        void registerPushLog(std::function<void(const char* message)> pushLogMessage);
        void registerPushDateTime(std::function<void(const char* message)> pushDateTimeMessage);
        void registerPushStatus(std::function<void(const char* message)> pushStatusMessage);
        void registerPushLevel(std::function<void(const char* message)> pushLevelMessage);
        void registerPushMode(std::function<void(String mode)> pushMode);

        void registerGetDatetimeFun(std::function<std::string()> getDatetime);
};

#endif