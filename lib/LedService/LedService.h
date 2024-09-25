#ifndef LEDSERVICE_H
#define LEDSERVICE_H

#include <map>
#include <functional>

#include <FastLED.h>

#include "LoggingService.h"
#include "MqttService.h"

#include "AbstractMode.h"

#include "utils.h"

#include "../../config/config.h"


// forward declaration
class AbstractMode;
class LoggingService;


class LedService {
    private:
        LoggingService *loggingService;
        MqttService *mqttService;

        // store for all modes
        std::map<String, AbstractMode*> modes;

        CRGB leds[LED_NUM_LEDS];

        String newCurrentMode = "none";
        String currentMode = "none";

        int maxModeSteps = LED_MODE_CONFIG_MAX_STEPS;
        int newInternalModeSteps = 0;
        int internalModeSteps = 0;

        void setLed(short index, byte r, byte g, byte b);
        void setLed(short index, CRGB color);
        void setLed(short index, CHSV color);
        void setLed(byte r, byte g, byte b);

    public:
        LedService();
        LedService(LoggingService *loggingService, MqttService *mqttService);

        void setup();

        void loop();
        void setMode(String mode);

        void setHexColor(String hexColor);
        void setBrightness(byte brightness);

        // modes
        void registerMode(String name, AbstractMode* mode);
        void unregisterMode(String name);

        bool modeExists(String name);
};


#endif