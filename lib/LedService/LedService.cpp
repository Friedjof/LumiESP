#include "LedService.h"

// LED mode strings representation
const char* LedModeStrings[] = {
    "none",
    "off",
    "on",
    "custom",
    "automatic",
    "blink",
    "fade",
    "rainbow",
    "loop"
};

std::map<String, LedModes> stringToEnumMap = {
    {"none", MODE_NONE},
    {"off", MODE_OFF},
    {"on", MODE_ON},
    {"custom", MODE_CUSTOM},
    {"automatic", MODE_AUTOMATIC},
    {"blink", MODE_BLINK},
    {"fade", MODE_FADE},
    {"rainbow", MODE_RAINBOW},
    {"loop", MODE_LOOP}
};

// ------- CONSTRUCTORS -------
LedService::LedService()
{
}

LedService::LedService(LoggingService *loggingService, MqttService *mqttService)
{
    this->loggingService = loggingService;
    this->mqttService = mqttService;
}

// ------- SETUP & LOOP -------
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
    // check if mode is already set
    if (this->newCurrentMode == mode)
    {
        this->loggingService->logMessage(LOG_LEVEL_WARN, LOG_MODE_ALL, "LED Service requested mode is already set: " + this->getModeStr(mode));
        return;
    }

    this->newCurrentMode = mode;

    this->loggingService->logMessage(LOG_LEVEL_DEBUG, LOG_MODE_ALL, "LED Service requested mode: " + this->getModeStr(this->newCurrentMode));
}

void LedService::confirmMode()
{
    this->currentMode = this->newCurrentMode;

    FastLED.show();

    // publish mode to MQTT
    this->loggingService->logMessage(LOG_LEVEL_DEBUG, LOG_MODE_ALL, "LED Service confirmed mode: " + this->getModeStr(this->currentMode));

    this->mqttService->publish(this->mqttService->mqttLedPubTopic(MQTT_LED_MODE_TOPIC), this->getModeStr(this->currentMode));
}

// ------- CALLBACKS -------
void LedService::callback(String topic, String payload)
{    
    this->loggingService->logMessage(LOG_LEVEL_DEBUG, LOG_MODE_SERIAL, "LED Service callback triggered");
    this->loggingService->logMessage(LOG_LEVEL_DEBUG, LOG_MODE_SERIAL, "LED Service callback topic: " + topic);
    this->loggingService->logMessage(LOG_LEVEL_DEBUG, LOG_MODE_SERIAL, "LED Service callback payload: " + payload);
}

void LedService::callback(char* topic, byte* payload, unsigned int length)
{
    String strTopic = String(topic);
    String strPayload = "";

    for (int i = 0; i < length; i++)
    {
        strPayload += (char)payload[i];
    }

    this->callback(strTopic, strPayload);
}


// ------- HELPER FUNCTIONS -------
bool LedService::firstModeTrigger(LedModes mode)
{
    return this->newCurrentMode == mode && this->currentMode != mode;
}

String LedService::getModeStr(LedModes mode) {
    if (mode >= MODE_NONE && mode <= MODE_LOOP) {
        return LedModeStrings[mode];
    } else {
        return "none";
    }
}

LedModes LedService::getModeEnum(String mode) {
    auto it = stringToEnumMap.find(mode);
    if (it != stringToEnumMap.end()) {
        return it->second;
    } else {
        return MODE_NONE;
    }
}

void LedService::setLed(short index, byte r, byte g, byte b)
{
    this->leds[index] = CRGB(r, g, b);
}


// ------- START LED MODES -------
void LedService::mode_on()
{
    const int maxModeSteps = 0;

    if (this->firstModeTrigger(MODE_ON)) {
        for (int i = 0; i < LED_NUM_LEDS; i++) {
            this->setLed(i, LED_MODE_ON_START_R, LED_MODE_ON_START_G, LED_MODE_ON_START_B);
        }

        this->confirmMode();
    }
}

void LedService::mode_off()
{
    const int maxModeSteps = 0;

    if (this->firstModeTrigger(MODE_OFF)) {
        for (int i = 0; i < LED_NUM_LEDS; i++) {
            this->setLed(i, LED_MODE_OFF_START_R, LED_MODE_OFF_START_G, LED_MODE_OFF_START_B);
        }

        this->confirmMode();
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
