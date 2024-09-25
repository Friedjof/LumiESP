#ifndef LOGGINGSERVICE_H
#define LOGGINGSERVICE_H

#include "ClockService.h"
#include "StatusApp.h"

#include "../../config/config.h"


class LoggingService
{
    private:
        ClockService *clockService;
        StatusApp *statusApp;

        short logLevel = LOG_LEVEL;
        bool initialized = false;
        bool statusLoggingActive = false;
    public:
        LoggingService();
        LoggingService(ClockService *clockService);

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

        void updateEspStatus();
        void updateStatusDateTime();

        void registerStatusApp(StatusApp *statusApp);
};

#endif