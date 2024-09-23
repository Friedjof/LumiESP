#include "LedService.h"


LedService::LedService()
{
    this->currentMode = MODE_OFF;
    this->internalModeSteps = 0;
}

LedService::LedService(LoggingService *loggingService, MqttService *mqttService)
{
    this->loggingService = loggingService;
    this->mqttService = mqttService;

    this->currentMode = MODE_OFF;
    this->internalModeSteps = 0;
}

void LedService::setup()
{
    pinMode(LED_PIN, OUTPUT);
}

void LedService::loop()
{
    if (this->internalModeSteps >= this->maxModeSteps)
    {
        this->internalModeSteps = 0;
    }

    switch (this->currentMode)
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
            this->mode_off();
            break;
    }

    this->internalModeSteps++;
}

void LedService::setMode(LedModes mode)
{
    this->currentMode = mode;
}

void LedService::setLed(short index, char r, char g, char b)
{
    char log_msg[128];
    snprintf(log_msg, 128, "Setting LED %d to RGB(%d, %d, %d)", index, r, g, b);

    this->loggingService->logMessage(LOG_LEVEL_DEBUG, LOG_MODE_SERIAL, log_msg);

    // TODO: set LED index to RGB(r, g, b)
}


// ------- LED MODES -------
void LedService::mode_on()
{
    int maxModeSteps = 0;

    analogWrite(LED_PIN, 255);
}

void LedService::mode_off()
{
    int maxModeSteps = 0;

    analogWrite(LED_PIN, 0);
}

// TODO: implement more LED modes (see enum LedModes in LedService.h)
    
