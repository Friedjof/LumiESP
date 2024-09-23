#include "LedService.h"


LedService::LedService()
{
}

LedService::LedService(LoggingService *loggingService, MqttService *mqttService)
{
    this->loggingService = loggingService;
    this->mqttService = mqttService;
}

void LedService::setup()
{
    FastLED.addLeds<WS2812, LED_PIN, GRB>(this->leds, LED_NUM_LEDS).setCorrection(TypicalLEDStrip);
    FastLED.setBrightness(LED_MODE_CONFIG_BRIGHTNESS);

    // turn off LEDs on startup
    this->setMode(MODE_OFF);

    this->loggingService->logMessage(LOG_LEVEL_DEBUG, LOG_MODE_SERIAL, "LED Service setup completed");
}

void LedService::loop()
{
    if (this->internalModeSteps >= this->maxModeSteps)
    {
        this->internalModeSteps = 0;
    }

    switch (this->newCurrentMode)
    {
        case MODE_OFF:
            this->mode_off();
            break;
        case MODE_ON:
            this->mode_on();
            break;
        case MODE_CUSTOM:
            this->mode_custom();
            break;
        case MODE_AUTOMATIC:
            this->mode_automatic();
            break;
        case MODE_BLINK:
            this->mode_blink();
            break;
        case MODE_FADE:
            this->mode_fade();
            break;
        case MODE_RAINBOW:
            this->mode_rainbow();
            break;
        case MODE_LOOP:
            this->mode_loop();
            break;
        default:
            break;
    }

    this->internalModeSteps++;
}

void LedService::setMode(LedModes mode)
{
    this->newCurrentMode = mode;
}

void LedService::setLed(short index, byte r, byte g, byte b)
{
    this->leds[index] = CRGB(r, g, b);
}

// ------- HELPER FUNCTIONS -------
bool LedService::firstModeTrigger(LedModes mode)
{
    return this->newCurrentMode == mode && this->currentMode != mode;
}

String LedService::getModeStr(LedModes mode)
{
    switch (mode)
    {
        case MODE_OFF:
            return "off";
        case MODE_ON:
            return "on";
        case MODE_CUSTOM:
            return "custom";
        case MODE_AUTOMATIC:
            return "automatic";
        case MODE_BLINK:
            return "blink";
        case MODE_FADE:
            return "fade";
        case MODE_RAINBOW:
            return "rainbow";
        case MODE_LOOP:
            return "loop";
        default:
            return "unknown";
    }
}


// ------- START LED MODES -------
void LedService::mode_on()
{
    const int maxModeSteps = 0;

    if (this->firstModeTrigger(MODE_ON)) {
        for (int i = 0; i < LED_NUM_LEDS; i++) {
            this->setLed(i, LED_MODE_ON_START_R, LED_MODE_ON_START_G, LED_MODE_ON_START_B);
        }

        FastLED.show();

        this->currentMode = MODE_ON;

        this->loggingService->logMessage(LOG_LEVEL_DEBUG, LOG_MODE_SERIAL, "LEDs turned on");
    }
}

void LedService::mode_off()
{
    const int maxModeSteps = 0;

    if (this->firstModeTrigger(MODE_OFF)) {
        for (int i = 0; i < LED_NUM_LEDS; i++) {
            this->setLed(i, LED_MODE_OFF_START_R, LED_MODE_OFF_START_G, LED_MODE_OFF_START_B);
        }

        FastLED.show();

        this->currentMode = MODE_OFF;

        this->loggingService->logMessage(LOG_LEVEL_DEBUG, LOG_MODE_SERIAL, "LEDs turned off");
    }
}

void LedService::mode_custom()
{
    // TODO: implement custom mode
}

void LedService::mode_automatic()
{
    // TODO: implement automatic mode
}

void LedService::mode_blink()
{
    // TODO: implement blink mode
}

void LedService::mode_fade()
{
    // TODO: implement fade mode
}

void LedService::mode_rainbow()
{
    // TODO: implement rainbow mode
}

void LedService::mode_loop()
{
    // TODO: implement loop mode
}

// ------- END LED MODES -------
