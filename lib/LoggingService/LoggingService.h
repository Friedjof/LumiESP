#ifndef LoggingService_h
#define LoggingService_h

#include <string>
#include <PubSubClient.h>

#include "MqttService.h"
#include "ClockService.h"

#include "../../config/config.h"


class LoggingService
{
    private:
        MqttService *mqttService;
        ClockService *clockService;

        short logLevel = LOG_LEVEL;
        bool initialized = false;
    public:
        LoggingService();
        LoggingService(MqttService *mqttService, ClockService *clockService);

        void setup();
        void setup(short logLevel);

        void logLevelStr(short logLevel, char* logLevelStr);

        void logMessage(short logLevel, short mode, const char* message);
        void logMessage(short logLevel, const char* message);
        void logMessage(const char* message);
};

#endif