#ifndef LEDSERVICE_H
#define LEDSERVICE_H

#include <map>
#include <functional>

#include <FastLED.h>

#include "utils.h"
#include "../../config/config.h"


struct Mode {
    std::function<void(int steps)> modeLoop;
    std::function<void()> enableFirstRun;
};


class LedService {
    private:
        // store for all modes
        std::map<String, Mode> modes;

        CRGB leds[LED_NUM_LEDS];

        String currentMode = "";
        String newCurrentMode = this->currentMode;

        unsigned long long maxModeSteps = LED_SERVICE_MAX_STEPS;
        unsigned long long internalModeSteps = 0;

        // simple log function
        std::function<void(short logLevel, short mode, String message)> logFunction = nullptr;
        std::function<void(String mode)> pushModeCallback = nullptr;

        bool isNewMode();

        // private helper functions
        String expandHexColor(String hexColor);

    public:
        LedService();

        void setup();

        void loop();
        void setMode(String mode);
        String getMode();

        void setLed(short index, byte r, byte g, byte b);
        void setLed(short index, CRGB color);
        void setLed(short index, CHSV color);
        void setLed(short index, String hexColor);
        void setLed(byte r, byte g, byte b);

        void setColor(CRGB color);
        void setHexColor(String hexColor);
        void setBrightness(byte brightness);

        // modes
        void registerMode(String name, std::function<void(int steps)> mode, std::function<void()> enableFirstRun);
        void unregisterMode(String name);

        bool modeExists(String name);

        void confirmLedConfig();

        // simple log functions
        void log(short logLevel, short mode, String message);
        bool registerLogFunction(std::function<void(short logLevel, short mode, String message)> logFunction);
        void registerPushModeCallback(std::function<void(String mode)> pushModeCallback);
};


#endif