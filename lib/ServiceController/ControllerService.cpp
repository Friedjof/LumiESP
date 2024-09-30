#include "ControllerService.h"


ControllerService::ControllerService(MqttService *mqttService, ClockService *clockService, LoggingService *loggingService, LedService *ledService)
{
    this->mqttService = mqttService;
    this->clockService = clockService;
    this->loggingService = loggingService;
    this->ledService = ledService;
}


// ------- CONTROLLER METHODS -------
void ControllerService::setup()
{
    this->ledService->registerLogFunction([this](short logLevel, short mode, String message) {
        this->loggingService->logMessage(logLevel, mode, message);
    });

    this->initialized = true;

    this->registerGetDatetimeFun([this]() -> std::string {
        char datetime[128];
        this->clockService->getDateTime(datetime);
        return std::string(datetime);
    });
}

void ControllerService::setStatus(String status)
{
    this->currentStatus = status;
}

String ControllerService::getStatus()
{
    return this->currentStatus;
}

void ControllerService::setMode(String mode)
{
    if (!this->initialized)
    {
        this->loggingService->logMessage(LOG_LEVEL_WARN, LOG_MODE_ALL, "Controller Service not initialized");
        return;
    }

    this->loggingService->logMessage(LOG_LEVEL_DEBUG, LOG_MODE_ALL, "Setting mode: " + mode);

    this->ledService->setMode(mode);
}

// ------- LED SERVICE METHODS -------
void ControllerService::registerMode(String name, std::function<void(int steps)> modeLoop, std::function<void()> enableFirstRun)
{
    if (!this->initialized)
    {
        this->loggingService->logMessage(LOG_LEVEL_WARN, LOG_MODE_ALL, "Controller Service not initialized");
        return;
    }

    this->ledService->registerMode(name, modeLoop, enableFirstRun);
}

void ControllerService::unregisterMode(String name)
{
    if (!this->initialized)
    {
        this->loggingService->logMessage(LOG_LEVEL_WARN, LOG_MODE_ALL, "Controller Service not initialized");
        return;
    }

    this->ledService->unregisterMode(name);
}

void ControllerService::setLed(short index, byte r, byte g, byte b)
{
    if (!this->initialized)
    {
        this->loggingService->logMessage(LOG_LEVEL_WARN, LOG_MODE_ALL, "Controller Service not initialized");
        return;
    }

    this->ledService->setLed(index, r, g, b);
}

void ControllerService::setLed(short index, CRGB color)
{
    if (!this->initialized)
    {
        this->loggingService->logMessage(LOG_LEVEL_WARN, LOG_MODE_ALL, "Controller Service not initialized");
        return;
    }

    this->ledService->setLed(index, color);
}

void ControllerService::setLed(short index, CHSV color)
{
    if (!this->initialized)
    {
        this->loggingService->logMessage(LOG_LEVEL_WARN, LOG_MODE_ALL, "Controller Service not initialized");
        return;
    }

    this->ledService->setLed(index, color);
}

void ControllerService::setLed(short index, String hexColor)
{
    if (!this->initialized)
    {
        this->loggingService->logMessage(LOG_LEVEL_WARN, LOG_MODE_ALL, "Controller Service not initialized");
        return;
    }

    this->ledService->setLed(index, hexColor);
}

void ControllerService::setLed(byte r, byte g, byte b)
{
    if (!this->initialized)
    {
        this->loggingService->logMessage(LOG_LEVEL_WARN, LOG_MODE_ALL, "Controller Service not initialized");
        return;
    }

    this->ledService->setLed(r, g, b);
}

void ControllerService::setHexColor(String hexColor)
{
    if (!this->initialized)
    {
        this->loggingService->logMessage(LOG_LEVEL_WARN, LOG_MODE_ALL, "Controller Service not initialized");
        return;
    }

    this->ledService->setHexColor(hexColor);
}

void ControllerService::setColor(CRGB color)
{
    if (!this->initialized)
    {
        this->loggingService->logMessage(LOG_LEVEL_WARN, LOG_MODE_ALL, "Controller Service not initialized");
        return;
    }
    
    this->ledService->setColor(color);
}

void ControllerService::setBrightness(byte brightness)
{
    if (!this->initialized)
    {
        this->loggingService->logMessage(LOG_LEVEL_WARN, LOG_MODE_ALL, "Controller Service not initialized");
        return;
    }

    this->ledService->setBrightness(brightness);
}

void ControllerService::confirmLedConfig()
{
    if (!this->initialized)
    {
        this->loggingService->logMessage(LOG_LEVEL_WARN, LOG_MODE_ALL, "Controller Service not initialized");
        return;
    }

    this->ledService->confirmLedConfig();
}

// ------- MQTT SERVICE METHODS -------
std::function<void(String payload)> ControllerService::subscribeModeTopic(String modeName, String localTopic, String defaultPayload, boundaries_t boundaries, payload_e payloadType, topic_e topicType, std::function<void(String payload)> topicCallback)
{
    if (!this->initialized)
    {
        this->loggingService->logMessage(LOG_LEVEL_WARN, LOG_MODE_ALL, "Controller Service not initialized");
        return nullptr;
    }

    return this->mqttService->subscribeModeTopic(modeName, localTopic, defaultPayload, boundaries, payloadType, topicType, topicCallback);
}

std::function<void(String payload)> ControllerService::subscribeModeTopic(String modeName, String localTopic, payload_e payloadType, topic_e topicType)
{
    if (!this->initialized)
    {
        this->loggingService->logMessage(LOG_LEVEL_WARN, LOG_MODE_ALL, "Controller Service not initialized");
        return nullptr;
    }

    return this->mqttService->subscribeModeTopic(modeName, localTopic, payloadType, topicType);
}

std::function<void(String payload)> ControllerService::subscribeModeTopic(String modeName, String localTopic)
{
    if (!this->initialized)
    {
        this->loggingService->logMessage(LOG_LEVEL_WARN, LOG_MODE_ALL, "Controller Service not initialized");
        return nullptr;
    }

    return this->mqttService->subscribeModeTopic(modeName, localTopic);
}

std::function<void(String payload)> ControllerService::subscribeModeTopic(String modeName, String localTopic, const char* defaultPayload, payload_e payloadType, topic_e topicType, std::function<void(String payload)> topicCallback)
{
    if (!this->initialized)
    {
        this->loggingService->logMessage(LOG_LEVEL_WARN, LOG_MODE_ALL, "Controller Service not initialized");
        return nullptr;
    }

    return this->mqttService->subscribeModeTopic(modeName, localTopic, defaultPayload, payloadType, topicType, topicCallback);
}

std::function<void(String payload)> ControllerService::subscribeModeTopic(String modeName, String localTopic, const char* defaultPayload, payload_e payloadType, std::function<void(String payload)> topicCallback)
{
    if (!this->initialized)
    {
        this->loggingService->logMessage(LOG_LEVEL_WARN, LOG_MODE_ALL, "Controller Service not initialized");
        return nullptr;
    }

    return this->mqttService->subscribeModeTopic(modeName, localTopic, defaultPayload, payloadType, topicCallback);
}

std::function<void(String payload)> ControllerService::subscribeModeTopic(String modeName, String localTopic, int defaultPayload, boundaries_t boundaries, payload_e payloadType, std::function<void(String payload)> topicCallback)
{
    if (!this->initialized)
    {
        this->loggingService->logMessage(LOG_LEVEL_WARN, LOG_MODE_ALL, "Controller Service not initialized");
        return nullptr;
    }

    return this->mqttService->subscribeModeTopic(modeName, localTopic, defaultPayload, boundaries, payloadType, topicCallback);
}

// ------- LOGGING SERVICE METHODS -------
void ControllerService::logMessage(short logLevel, short mode, const char* message)
{
    if (!this->initialized)
    {
        return;
    }

    this->loggingService->logMessage(logLevel, mode, message);
}

void ControllerService::logMessage(short logLevel, short mode, String message)
{
    this->logMessage(logLevel, mode, message.c_str());
}

void ControllerService::registerPushLog(std::function<void(const char* message)> pushLogMessage)
{
    if (!this->initialized)
    {
        return;
    }

    this->loggingService->registerPushLog(pushLogMessage);
}

void ControllerService::registerPushDateTime(std::function<void(const char* message)> pushDateTimeMessage)
{
    if (!this->initialized)
    {
        return;
    }

    this->pushDateTimeMessage = pushDateTimeMessage;
}

void ControllerService::registerPushStatus(std::function<void(const char* message)> pushStatusMessage)
{
    if (!this->initialized)
    {
        return;
    }

    this->pushStatusMessage = pushStatusMessage;
}

void ControllerService::registerPushLevel(std::function<void(const char* message)> pushLevelMessage)
{
    if (!this->initialized)
    {
        return;
    }

    this->loggingService->registerPushLevel(pushLevelMessage);
}

void ControllerService::registerPushMode(std::function<void(String mode)> modeCallback)
{
    if (!this->initialized)
    {
        return;
    }

    this->ledService->registerPushModeCallback(modeCallback);
}

void ControllerService::registerGetDatetimeFun(std::function<std::string()> getDatetime)
{
    if (!this->initialized)
    {
        return;
    }

    this->loggingService->registerGetDatetimeFun(getDatetime);
}

// ------- TASK WRAPPERS -------
void ControllerService::mqttServiceStatusUpdateWrapper()
{
    if (!this->pushStatusMessage)
    {
        return;
    }

    this->pushStatusMessage(this->currentStatus.c_str());

    this->loggingService->logMessage(LOG_LEVEL_DEBUG, LOG_MODE_ALL, "MQTT status update");
}

void ControllerService::mqttServiceLoopWrapper()
{
    this->mqttService->loop();
}

void ControllerService::mqttServiceUpdateDateTimeWrapper()
{
    if (!this->pushDateTimeMessage)
    {
        return;
    }

    this->pushDateTimeMessage(this->clockService->getDateTime().c_str());

    this->loggingService->logMessage(LOG_LEVEL_DEBUG, LOG_MODE_ALL, "MQTT DateTime update");
}

void ControllerService::mqttServiceCallbackWrapper(const espMqttClientTypes::MessageProperties& properties, const char* topic, const uint8_t* payload, size_t len, size_t index, size_t total)
{
    String playloadStr = "";

    for (int i = 0; i < len; i++)
    {
        playloadStr += (char)payload[i];
    }

    this->loggingService->logMessage(LOG_LEVEL_DEBUG, LOG_MODE_SERIAL, "MQTT message received: " + String(topic) + " = " + playloadStr);

    bool valid = this->mqttService->onMessageCallback(topic, playloadStr);

    if (!valid)
    {
        this->loggingService->logMessage(LOG_LEVEL_WARN, LOG_MODE_ALL, "MQTT topic or payload invalid: " + String(topic) + " = " + playloadStr);
    }
}

void ControllerService::clockServiceTimeSyncWrapper()
{
    this->clockService->syncTime();

    this->loggingService->logMessage(LOG_LEVEL_INFO, LOG_MODE_ALL, "Date and time synchronized");
}

void ControllerService::ledServiceLoopWrapper()
{
    this->ledService->loop();
}
