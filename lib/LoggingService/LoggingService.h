#ifndef LOGGINGSERVICE_H
#define LOGGINGSERVICE_H


#include <functional>

#include <Arduino.h>


#include "../../config/config.h"


class LoggingService
{
    private:
        short logLevel = LOG_LEVEL;

        // status flags
        bool initialized = false;
        bool mqttLoggingActive = false;
        bool datetimeLoggingIsActive = false;
        bool mqttLevelLoggingActive = false;

        // callback functions
        std::function<void(const char* message)> mqttLogMessage;
        std::function<std::string()> getDateTime;
        std::function<void(const char* message)> mqttLevelMessage;

    public:
        LoggingService();

        void setup();
        void setup(short logLevel);

        void logLevelStr(short logLevel, char* logLevelStr);
        String logLevelStr(short logLevel);
        String logMessageStr(short logLevel, const char* message);

        void logMessage(short logLevel, short mode, const char* message);
        void logMessage(short logLevel, short mode, String message);
        void logMessage(short logLevel, const char* message);
        void logMessage(short logLevel, String message);
        void logMessage(const char* message);

        // register callback functions
        void registerMqttLogFun(std::function<void(const char* message)> mqttLogMessage);
        void registerGetDatetimeFun(std::function<std::string()> getDatetime);
        void registerMqttLevelFun(std::function<void(const char* message)> mqttLevelMessage);
};

#endif