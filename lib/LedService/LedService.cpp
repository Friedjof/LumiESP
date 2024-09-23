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
    if (this->newInternalModeSteps >= this->maxModeSteps)
    {
        this->newInternalModeSteps = 0;
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

    this->internalModeSteps = this->newInternalModeSteps++;
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

void LedService::setCustomColor(byte r, byte g, byte b, byte brightness)
{
    this->newCustomColor = CRGB(r, g, b);
    this->newCustomBrightness = brightness;
}

void LedService::setCustomColor(Colors color, byte value)
{
    switch (color)
    {
        case COLOR_R:
            this->newCustomColor.r = value;
            break;
        case COLOR_G:
            this->newCustomColor.g = value;
            break;
        case COLOR_B:
            this->newCustomColor.b = value;
            break;
        case COLOR_BRIGHTNESS:
            this->newCustomBrightness = value;
            break;
        default:
            break;
    }
}

void LedService::confirmMode()
{
    if (this->isfirstModeTrigger(this->newCurrentMode))
    {
        this->currentMode = this->newCurrentMode;
        this->loggingService->logMessage(LOG_LEVEL_INFO, LOG_MODE_ALL, "LED Service confirmed mode: " + this->getModeStr(this->currentMode));

        this->mqttService->publish(this->mqttService->mqttLedPubTopic(MQTT_LED_MODE_TOPIC), this->getModeStr(this->currentMode));
    }

    FastLED.show();
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
bool LedService::isfirstModeTrigger(LedModes mode)
{
    return this->newCurrentMode == mode && this->currentMode != mode;
}

bool LedService::isNewCustomColor()
{
    return this->newCustomColor != this->customColor || this->newCustomBrightness != this->customBrightness;
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

void LedService::setLed(short index, CRGB color)
{
    this->leds[index] = color;
}

void LedService::setLed(short index, CHSV color)
{
    this->leds[index] = color;
}

void LedService::setLed(byte r, byte g, byte b)
{
    for (int i = 0; i < LED_NUM_LEDS; i++)
    {
        this->setLed(i, r, g, b);
    }
}

// ------- START LED MODES -------
void LedService::mode_on()
{
    const int maxModeSteps = 0;

    if (this->isfirstModeTrigger(MODE_ON)) {
        for (int i = 0; i < LED_NUM_LEDS; i++) {
            this->setLed(i, LED_MODE_ON_START_R, LED_MODE_ON_START_G, LED_MODE_ON_START_B);
        }

        this->confirmMode();
    }
}

void LedService::mode_off()
{
    const int maxModeSteps = 0;

    if (this->isfirstModeTrigger(MODE_OFF)) {
        for (int i = 0; i < LED_NUM_LEDS; i++) {
            this->setLed(i, LED_MODE_OFF_START_R, LED_MODE_OFF_START_G, LED_MODE_OFF_START_B);
        }

        this->confirmMode();
    }
}

void LedService::mode_custom()
{
    if (this->isNewCustomColor() || this->isfirstModeTrigger(MODE_CUSTOM))
    {
        this->customColor = this->newCustomColor;
        this->customBrightness = this->newCustomBrightness;

        for (int i = 0; i < LED_NUM_LEDS; i++)
        {
            this->setLed(i, this->customColor.r, this->customColor.g, this->customColor.b);
        }

        FastLED.setBrightness(this->customBrightness);

        this->confirmMode();
    }
}

void LedService::mode_automatic()
{
    
}

void LedService::mode_blink()
{
    const int maxModeSteps = LED_NUM_LEDS;

    if (this->isfirstModeTrigger(MODE_BLINK))
    {
        this->confirmMode();
    }

    for (int i = 0; i < LED_NUM_LEDS; i++)
    {
        if (random(100) % 15 == 0)
        {
            this->setLed(i, LED_MODE_BLINK_START_R, LED_MODE_BLINK_START_G, LED_MODE_BLINK_START_B);
        }
        else
        {
            this->setLed(i, 0, 0, 0);
        }
    }

    this->confirmMode();
}

void LedService::mode_fade()
{
    const int maxModeSteps = LED_NUM_LEDS * 2;

    if (this->isfirstModeTrigger(MODE_FADE))
    {
        this->setLed(LED_MODE_FADE_START_R, LED_MODE_FADE_START_G, LED_MODE_FADE_START_B);

        this->confirmMode();
    }

    int step = this->internalModeSteps % maxModeSteps;

    if (step >= LED_NUM_LEDS) {
        step = maxModeSteps - step;
    }

    const int r = (step * LED_MODE_FADE_END_R + (LED_NUM_LEDS - step) * LED_MODE_FADE_START_R) / LED_NUM_LEDS;
    const int g = (step * LED_MODE_FADE_END_G + (LED_NUM_LEDS - step) * LED_MODE_FADE_START_G) / LED_NUM_LEDS;
    const int b = (step * LED_MODE_FADE_END_B + (LED_NUM_LEDS - step) * LED_MODE_FADE_START_B) / LED_NUM_LEDS;

    for (int i = 0; i < LED_NUM_LEDS; i++)
    {
        this->setLed(i, r, g, b);
    }

    this->confirmMode();
}

void LedService::mode_rainbow()
{
    const int maxModeSteps = LED_NUM_LEDS;

    for (int i = 0; i < maxModeSteps; i++) {
        int hue = (i * 256 / maxModeSteps + this->newInternalModeSteps) % 256;
        this->setLed(i, CHSV(hue, 255, 255));
    }

    this->confirmMode();
}

void LedService::mode_loop()
{
    const int maxModeSteps = LED_NUM_LEDS;

    if (this->isfirstModeTrigger(MODE_LOOP))
    {
        this->setLed(LED_MODE_LOOP_PATTERN_OFF_R, LED_MODE_LOOP_PATTERN_OFF_G, LED_MODE_LOOP_PATTERN_OFF_B);
    }

    const int r = this->internalModeSteps % maxModeSteps;
    const int newR = this->newInternalModeSteps % maxModeSteps;

    const int g = (this->internalModeSteps + maxModeSteps / 3) % maxModeSteps;
    const int newG = (this->newInternalModeSteps + maxModeSteps / 3) % maxModeSteps;

    const int b = (this->internalModeSteps + 2 * maxModeSteps / 3) % maxModeSteps;
    const int newB = (this->newInternalModeSteps + 2 * maxModeSteps / 3) % maxModeSteps;

    this->setLed(r, 0, 0, 0);
    this->setLed(newR, LED_MODE_LOOP_PATTERN_ON_R, 0, 0);

    this->setLed(g, 0, 0, 0);
    this->setLed(newG, 0, LED_MODE_LOOP_PATTERN_ON_G, 0);

    this->setLed(b, 0, 0, 0);
    this->setLed(newB, 0, 0, LED_MODE_LOOP_PATTERN_ON_B);
    
    this->confirmMode();
}

// ------- END LED MODES -------
