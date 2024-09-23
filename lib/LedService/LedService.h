#ifndef LEDSERVICE_H
#define LEDSERVICE_H

#include <map>

#include <FastLED.h>

#include "LoggingService.h"
#include "MqttService.h"

#include "../../config/config.h"


enum LedModes
{
    MODE_NONE,
    MODE_OFF,
    MODE_ON,
    MODE_CUSTOM,
    MODE_AUTOMATIC,
    MODE_BLINK,
    MODE_FADE,
    MODE_RAINBOW,
    MODE_LOOP,
};


enum Colors
{
    COLOR_R,
    COLOR_G,
    COLOR_B,
    COLOR_BRIGHTNESS
};


class LedService {
    private:
        LoggingService *loggingService;
        MqttService *mqttService;

        CRGB leds[LED_NUM_LEDS];

        CRGB newCustomColor = CRGB(LED_MODE_CUSTOM_DEFAULT_R, LED_MODE_CUSTOM_DEFAULT_G, LED_MODE_CUSTOM_DEFAULT_B);
        CRGB customColor = CRGB(LED_MODE_CUSTOM_DEFAULT_R, LED_MODE_CUSTOM_DEFAULT_G, LED_MODE_CUSTOM_DEFAULT_B);
        byte newCustomBrightness = LED_MODE_CUSTOM_DEFAULT_BRIGHTNESS;
        byte customBrightness = LED_MODE_CUSTOM_DEFAULT_BRIGHTNESS;

        LedModes newCurrentMode = MODE_NONE;
        LedModes currentMode = MODE_NONE;

        int maxModeSteps = LED_MODE_CONFIG_MAX_STEPS;
        int newInternalModeSteps = 0;
        int internalModeSteps = 0;

        void confirmMode();
        void setLed(short index, byte r, byte g, byte b);
        void setLed(short index, CRGB color);
        void setLed(short index, CHSV color);
        void setLed(byte r, byte g, byte b);

        bool isfirstModeTrigger(LedModes mode);
        bool isNewCustomColor();
    public:
        LedService();
        LedService(LoggingService *loggingService, MqttService *mqttService);

        void setup();

        void loop();
        void setMode(LedModes mode);

        void setCustomColor(byte r, byte g, byte b, byte brightness);
        void setCustomColor(Colors color, byte value);

        String getModeStr(LedModes mode);
        LedModes getModeEnum(String mode);

        void callback(String topic, String payload);
        void callback(char* topic, byte* payload, unsigned int length);

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