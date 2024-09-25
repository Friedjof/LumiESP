#ifndef ABSTRACTMODE_H
#define ABSTRACTMODE_H


#include <functional>

#include "LedService.h"
#include "LoggingService.h"
#include "MqttService.h"

#include "../../config/config.h"


// forward declaration
class LedService;


class AbstractMode {
    protected:
        LedService* ledService;
        LoggingService* loggingService;
        MqttService* mqttService;

        // mode properties
        String modeTitle;
        String modeInternalName;
        String modeDescription;
        String modeAuthor;
        String modeContact;
        String modeVersion;
        String modeLicense;

        bool isHexColor(String hex);

    public:
        AbstractMode(LedService* ledService, LoggingService* loggingService, MqttService* mqttService);
        ~AbstractMode();

        void setup();
        void loop(int steps);

        virtual void customSetup() = 0;
        virtual void customLoop(const int steps) = 0;
};

#endif