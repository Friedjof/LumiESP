#include "ControllerService.h"


ControllerService::ControllerService(MqttService *mqttService, ClockService *clockService, LoggingService *loggingService, LedService *ledService)
{
    this->mqttService = mqttService;
    this->clockService = clockService;
    this->loggingService = loggingService;
    this->ledService = ledService;
}

void ControllerService::setup()
{
    this->initialized = true;
}

void ControllerService::mqttSubscribtion(String topic, String payload)
{
    this->loggingService->logMessage(LOG_LEVEL_DEBUG, LOG_MODE_ALL, "MQTT callback topic: " + topic + ", payload: " + payload);

    if (this->mqttService->isLedModeSubTopic(topic))
    {
        LedModes mode = this->ledService->getModeEnum(payload);
        if (mode != MODE_NONE)
        {
            this->ledService->setMode(mode);
        } else {
            this->loggingService->logMessage(LOG_LEVEL_WARN, LOG_MODE_ALL, "LED Service requested mode is not valid: " + payload);
        }
    } else if (this->mqttService->isLedModeCustomSubTopic(topic)) {
        this->loggingService->logMessage(LOG_LEVEL_DEBUG, LOG_MODE_ALL, "LED Service custom mode topic: " + topic);

        long value = payload.toInt();
        if (value < 0 || value > 255) {
            this->loggingService->logMessage(LOG_LEVEL_WARN, LOG_MODE_ALL, "Invalid value for byte conversion: " + payload);
            return;
        }
        byte byteValue = static_cast<byte>(value);

        if (this->mqttService->isLedModeCustomRSubTopic(topic)) {
            this->ledService->setCustomColor(COLOR_R, byteValue);
        } else if (this->mqttService->isLedModeCustomGSubTopic(topic)) {
            this->ledService->setCustomColor(COLOR_G, byteValue);
        } else if (this->mqttService->isLedModeCustomBSubTopic(topic)) {
            this->ledService->setCustomColor(COLOR_B, byteValue);
        } else if (this->mqttService->isLedModeCustomBrightnessSubTopic(topic)) {
            this->ledService->setCustomColor(COLOR_BRIGHTNESS, byteValue);
        }
    }
}

// ------- TASK WRAPPERS -------
void ControllerService::mqttServiceStatusUpdateWrapper()
{
    this->mqttService->mqttStatusUpdate();
}

void ControllerService::mqttServiceLoopWrapper()
{
    this->mqttService->loop();
}

void ControllerService::mqttServiceUpdateDateTimeWrapper()
{
    char datetime[128];
    this->clockService->getDateTime(datetime);

    this->mqttService->mqttDatetimeUpdate(datetime);
}

void ControllerService::mqttServiceCallbackWrapper(const espMqttClientTypes::MessageProperties& properties, const char* topic, const uint8_t* payload, size_t len, size_t index, size_t total)
{
    String playloadStr = "";

    for (int i = 0; i < len; i++)
    {
        playloadStr += (char)payload[i];
    }

    this->mqttSubscribtion(String(topic), playloadStr);
}

void ControllerService::clockServiceTimeSyncWrapper()
{
    this->clockService->syncTime();
}

void ControllerService::ledServiceLoopWrapper()
{
    this->ledService->loop();
}