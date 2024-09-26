#include "LoggingService.h"


// Default constructor
LoggingService::LoggingService() : logLevel(LOG_LEVEL), initialized(false) {}

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

String LoggingService::logLevelStr(short logLevel) {
    char logLevelStr[8];
    this->logLevelStr(logLevel, logLevelStr);

    return String(logLevelStr);
}

String LoggingService::logMessageStr(short logLevel, const char* message) {
    String logLevelStr = this->logLevelStr(logLevel);
    char log_msg[256];

    if (this->datetimeLoggingActive) {
        String datetime = this->getDateTime();
        snprintf(log_msg, 256, LOG_STRING, DEVICE_NAME, datetime.c_str(), logLevelStr.c_str(), message);
    } else {
        snprintf(log_msg, 256, LOG_STRING_NO_DATETIME, DEVICE_NAME, logLevelStr.c_str(), message);
    }

    return String(log_msg);
}

// Log message methods
void LoggingService::logMessage(short logLevel, short mode, const char* message) {
    if (this->initialized && logLevel <= this->logLevel) {
        if (mode == LOG_MODE_ALL || mode == LOG_MODE_SERIAL) {
            Serial.println(this->logMessageStr(logLevel, message));
        }

        if ((mode == LOG_MODE_ALL || mode == LOG_MODE_MQTT) && this->mqttLoggingActive) {
            this->mqttLogMessage(this->logMessageStr(logLevel, message).c_str());
        }
    }
}

void LoggingService::logMessage(short logLevel, short mode, String message) {
    this->logMessage(logLevel, mode, message.c_str());
}

void LoggingService::logMessage(short logLevel, const char* message) {
    this->logMessage(logLevel, LOG_MODE, message);
}

void LoggingService::logMessage(short logLevel, String message) {
    this->logMessage(logLevel, LOG_MODE, message);
}

void LoggingService::logMessage(const char* message) {
    this->logMessage(LOG_LEVEL_DEBUG, LOG_MODE, message);
}

// Register callback functions
void LoggingService::registerMqttLogFun(std::function<void(const char* message)> mqttLogMessage) {
    this->mqttLogMessage = mqttLogMessage;
    this->mqttLoggingActive = true;
}

void LoggingService::registerGetDatetimeFun(std::function<const char*()> getDatetime) {
    this->getDateTime = getDatetime;
    this->datetimeLoggingActive = true;
}