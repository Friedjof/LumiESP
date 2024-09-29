#include "LedService.h"


// ------- CONSTRUCTORS -------
LedService::LedService()
{
}

// ------- SETUP & LOOP -------
void LedService::setup()
{
    FastLED.addLeds<LED_TYPE, LED_PIN, GRB>(this->leds, LED_NUM_LEDS).setCorrection(TypicalLEDStrip);
    FastLED.setBrightness(LED_MODE_CONFIG_BRIGHTNESS);

    this->log(LOG_LEVEL_DEBUG, LOG_MODE_SERIAL, "LED Service setup completed");
}

void LedService::loop()
{
    if (this->internalModeSteps >= LED_SERVICE_MAX_STEPS)
    {
        this->internalModeSteps = 0;
    }

    if (this->isNewMode())
    {
        this->currentMode = this->newCurrentMode;

        this->internalModeSteps = 0;

        // enable first run for the new mode
        this->modes[this->currentMode].enableFirstRun();
    }

    this->modes[this->currentMode].modeLoop(this->internalModeSteps);

    this->internalModeSteps++;
}

void LedService::setMode(String mode)
{
    if (!this->modeExists(mode))
    {
        this->log(LOG_LEVEL_WARN, LOG_MODE_ALL, "LED Service mode does not exist: " + mode);
        return;
    }

    // check if mode is already set
    if (this->currentMode == mode)
    {
        this->log(LOG_LEVEL_WARN, LOG_MODE_ALL, "LED Service requested mode is already set: " + mode);
        return;
    }

    this->log(LOG_LEVEL_DEBUG, LOG_MODE_ALL, "LED Service requested mode: " + mode);

    this->newCurrentMode = mode;

    if (this->pushModeCallback != nullptr)
    {
        this->pushModeCallback(mode);
    }

    this->log(LOG_LEVEL_DEBUG, LOG_MODE_ALL, "LED Service mode set: " + mode);
}

String LedService::getMode()
{
    return this->currentMode;
}

void LedService::registerMode(String name, std::function<void(int steps)> mode, std::function<void()> enableFirstRun) {
    // check if mode already exists
    if (this->modes.find(name) != this->modes.end()) {
        this->log(LOG_LEVEL_WARN, LOG_MODE_ALL, "LED Service mode already exists: " + name);
        return;
    }

    this->log(LOG_LEVEL_DEBUG, LOG_MODE_SERIAL, "LED Service mode registered: " + name);

    this->modes[name] = {mode, enableFirstRun};
}

void LedService::unregisterMode(String name) {
    // check if mode exists
    if (this->modes.find(name) == this->modes.end()) {
        this->log(LOG_LEVEL_WARN, LOG_MODE_ALL, "LED Service mode does not exist: " + name);
        return;
    }

    this->modes.erase(name);
}

// ------- PUBLIC METHODS -------
void LedService::setHexColor(String hexColor)
{
    if (!isHexColor(hexColor))
    {
        this->log(LOG_LEVEL_WARN, LOG_MODE_ALL, "LED Service invalid hex color: " + hexColor);
        return;
    }

    CRGB color = CRGB::Black;

    if (hexColor.length() == 7) {
        color = CRGB(
            strtol(hexColor.substring(1, 3).c_str(), NULL, 16),
            strtol(hexColor.substring(3, 5).c_str(), NULL, 16),
            strtol(hexColor.substring(5, 7).c_str(), NULL, 16)
        );
    } else if (hexColor.length() == 4) {
        hexColor = this->expandHexColor(hexColor);

        color = CRGB(
            strtol(hexColor.substring(1, 3).c_str(), NULL, 16),
            strtol(hexColor.substring(3, 5).c_str(), NULL, 16),
            strtol(hexColor.substring(5, 7).c_str(), NULL, 16)
        );
    }

    for (int i = 0; i < LED_NUM_LEDS; i++)
    {
        this->leds[i] = color;
    }
}

void LedService::setColor(CRGB color)
{
    for (int i = 0; i < LED_NUM_LEDS; i++)
    {
        this->leds[i] = color;
    }
}

void LedService::setBrightness(byte brightness)
{
    FastLED.setBrightness(brightness);
}

void LedService::confirmLedConfig()
{
    FastLED.show();
}

// ------- HELPER FUNCTIONS -------
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

void LedService::setLed(short index, String hexColor)
{
    if (!isHexColor(hexColor))
    {
        this->log(LOG_LEVEL_WARN, LOG_MODE_ALL, "LED Service invalid hex color: " + hexColor);
        return;
    }

    CRGB color = CRGB::Black;

    if (hexColor.length() == 7) {
        color = CRGB(
            strtol(hexColor.substring(1, 3).c_str(), NULL, 16),
            strtol(hexColor.substring(3, 5).c_str(), NULL, 16),
            strtol(hexColor.substring(5, 7).c_str(), NULL, 16)
        );
    } else if (hexColor.length() == 4) {
        hexColor = this->expandHexColor(hexColor);

        color = CRGB(
            strtol(hexColor.substring(1, 3).c_str(), NULL, 16),
            strtol(hexColor.substring(3, 5).c_str(), NULL, 16),
            strtol(hexColor.substring(5, 7).c_str(), NULL, 16)
        );
    }

    this->leds[index] = color;
}

void LedService::setLed(byte r, byte g, byte b)
{
    for (int i = 0; i < LED_NUM_LEDS; i++)
    {
        this->setLed(i, r, g, b);
    }
}

bool LedService::modeExists(String name)
{
    return this->modes.find(name) != this->modes.end();
}

void LedService::log(short logLevel, short mode, String message)
{
    if (this->logFunction)
    {
        this->logFunction(logLevel, mode, message);
    }
}

bool LedService::registerLogFunction(std::function<void(short logLevel, short mode, String message)> logFunction)
{
    this->logFunction = logFunction;

    return true;
}

void  LedService::registerPushModeCallback(std::function<void(String mode)> pushModeCallback)
{
    this->pushModeCallback = pushModeCallback;
}

bool LedService::isNewMode()
{
    return this->currentMode != this->newCurrentMode;
}

// ------- PRIVATE METHODS -------
String LedService::expandHexColor(String hexColor)
{
    String expandedHexColor = "#";

    for (int i = 1; i < hexColor.length(); i++)
    {
        expandedHexColor += hexColor[i];
        expandedHexColor += hexColor[i];
    }

    return expandedHexColor;
}
