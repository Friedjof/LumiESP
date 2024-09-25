#include "LoggingService.h"


// Default constructor
LoggingService::LoggingService() : logLevel(LOG_LEVEL), initialized(false) {}

// Parameterized constructor
LoggingService::LoggingService(ClockService *clockService) : clockService(clockService), logLevel(LOG_LEVEL), initialized(false) {}

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
    String datetime = this->clockService->getDateTime();

    char log_msg[256];
    snprintf(log_msg, 256, LOG_STRING, DEVICE_NAME, datetime.c_str(), logLevelStr.c_str(), message);

    return String(log_msg);
}

// Log message methods
void LoggingService::logMessage(short logLevel, short mode, const char* message) {
    if (this->initialized && logLevel <= this->logLevel) {
        if (mode == LOG_MODE_ALL || mode == LOG_MODE_SERIAL) {
            Serial.println(this->logMessageStr(logLevel, message));
        }

        if ((mode == LOG_MODE_ALL || mode == LOG_MODE_MQTT) && this->statusLoggingActive) {
            this->statusApp->logMessage(this->logMessageStr(logLevel, message));
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

void LoggingService::registerStatusApp(StatusApp *statusApp) {
    this->statusApp = statusApp;

    this->statusLoggingActive = true;
}

// Update status methods
void LoggingService::updateEspStatus() {
    if (this->statusLoggingActive) {
        this->statusApp->logStatus("active");
        this->logMessage(LOG_LEVEL_INFO, LOG_MODE_ALL, "Status updated");
    }
}

void LoggingService::updateStatusDateTime() {
    if (this->statusLoggingActive) {
        this->statusApp->logDatetime(this->clockService->getDateTime());
    }
}
