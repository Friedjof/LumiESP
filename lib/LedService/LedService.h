#ifndef LEDSERVICE_H
#define LEDSERVICE_H

#include "LoggingService.h"
#include "MqttService.h"

#include "../../config/config.h"


// LED modes as enum
enum LedModes
{
    MODE_OFF,
    MODE_ON,
    MODE_CUSTOM,
    MODE_AUTOMATIC,
    MODE_BLINK,
    MODE_FADE,
    MODE_RAINBOW,
    MODE_LOOP,
};


class LedService {
    private:
        LoggingService *loggingService;
        MqttService *mqttService;

        LedModes currentMode = MODE_OFF;

        int maxModeSteps = LED_MODE_CONFIG_MAX_STEPS;
        int internalModeSteps = 0;

    public:
        LedService();
        LedService(LoggingService *loggingService, MqttService *mqttService);

        void setup();

        void loop();
        void setMode(LedModes mode);

        void setLed(short index, char r, char g, char b);

        // modes
        void mode_on();
        void mode_off();
        void mode_custom();
        void mode_automatic();
        void mode_blink();
        void mode_fade();
        void mode_rainbow();
        void mode_loop();
};


#endif