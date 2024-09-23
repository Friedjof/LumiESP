#include "LoggingService.h"


// Default constructor
LoggingService::LoggingService() : logLevel(LOG_LEVEL), initialized(false) {}

// Parameterized constructor
LoggingService::LoggingService(MqttService *mqttService, ClockService *clockService) : mqttService(mqttService), clockService(clockService), logLevel(LOG_LEVEL), initialized(false) {}

// Setup method
void LoggingService::setup() {
    Serial.begin(LOG_SERIAL_SPEED);

    this->initialized = true;
}

void LoggingService::setup(short logLevel) {
    Serial.begin(LOG_SERIAL_SPEED);

    this->logLevel = logLevel;
    this->initialized = true;
}

void LoggingService::logLevelStr(short logLevel, char* logLevelStr) {
    switch (logLevel) {
        case LOG_LEVEL_DEBUG:
            strcpy(logLevelStr, "DEBUG");
            break;
        case LOG_LEVEL_INFO:
            strcpy(logLevelStr, "INFO");
            break;
        case LOG_LEVEL_WARN:
            strcpy(logLevelStr, "WARN");
            break;
        case LOG_LEVEL_ERROR:
            strcpy(logLevelStr, "ERROR");
            break;
        default:
            strcpy(logLevelStr, "UNKNOWN");
            break;
    }
}

// Log message methods
void LoggingService::logMessage(short logLevel, short mode, const char* message) {
    if (this->initialized && logLevel <= this->logLevel) {
        char logLevelStr[8];

        this->logLevelStr(logLevel, logLevelStr);

        if ((mode == LOG_MODE_SERIAL || mode == LOG_MODE_ALL) && Serial.available()) {
            char datetime[128];
            this->clockService->getDateTime(datetime);

            char log_msg[256];
            snprintf(log_msg, 256, LOG_STRING, DEVICE_NAME, datetime, logLevelStr, message);

            Serial.println(log_msg);
        }

        if ((mode == LOG_MODE_MQTT || mode == LOG_MODE_ALL) && this->mqttService->isConnected()) {
            char datetime[128];
            this->clockService->getDateTime(datetime);

            char log_msg[256];
            snprintf(log_msg, 256, LOG_STRING, DEVICE_NAME, datetime, logLevelStr, message);

            this->mqttService->publish(MQTT_STATUS_LOG_TOPIC, log_msg);
        }
    }
}

void LoggingService::logMessage(short logLevel, const char* message) {
    this->logMessage(logLevel, LOG_MODE, message);
}

void LoggingService::logMessage(const char* message) {
    this->logMessage(LOG_LEVEL_DEBUG, LOG_MODE, message);
}
