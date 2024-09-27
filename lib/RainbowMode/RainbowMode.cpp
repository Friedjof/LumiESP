#include "RainbowMode.h"


RainbowMode::RainbowMode(ControllerService* controllerService) : AbstractMode(controllerService) {
    this->modeTitle = "Rainbow Mode";
    this->modeDescription = "This mode loops through all LEDs.";
    this->modeInternalName = "RainbowMode";
    this->modeAuthor = "Friedjof Noweck";
    this->modeContact = "git@noweck.info";
    this->modeVersion = "0.1.0";
    this->modeLicense = "MIT";
}

void RainbowMode::customSetup() {
    // register mqtt topics
    this->pushSaturationTopicFun = this->controllerService->subscribeModeTopic(
        this->modeInternalName, "saturation", 255, boundaries_t{0, 255}, payload_e::BYTE, std::function<void(String)>(std::bind(&RainbowMode::saturationCallback, this, std::placeholders::_1)));
    this->pushBrightnessTopicFun = this->controllerService->subscribeModeTopic(
        this->modeInternalName, "brightness", 255, boundaries_t{0, 255}, payload_e::BYTE, std::function<void(String)>(std::bind(&RainbowMode::brightnessCallback, this, std::placeholders::_1)));
    this->pushPositionTopicFun = this->controllerService->subscribeModeTopic(
        this->modeInternalName, "position", 0, boundaries_t{0, LED_NUM_LEDS}, payload_e::INT, std::function<void(String)>(std::bind(&RainbowMode::positionCallback, this, std::placeholders::_1)));
}

void RainbowMode::brightnessCallback(String payload) {
    this->controllerService->logMessage(LOG_LEVEL_DEBUG, LOG_MODE_ALL, "RainbowMode brightness callback: " + payload);

    this->newBrightness = payload.toInt();

    // publish the brightness to the pub topic
    this->pushBrightnessTopicFun(payload);
}

void RainbowMode::saturationCallback(String payload) {
    this->controllerService->logMessage(LOG_LEVEL_DEBUG, LOG_MODE_ALL, "RainbowMode saturation callback: " + payload);

    this->newSaturation = payload.toInt();

    // publish the saturation to the pub topic
    this->pushSaturationTopicFun(payload);
}

void RainbowMode::positionCallback(String payload) {
    this->controllerService->logMessage(LOG_LEVEL_DEBUG, LOG_MODE_ALL, "RainbowMode position callback: " + payload);

    this->newPosition = payload.toInt();

    // publish the position to the pub topic
    this->pushPositionTopicFun(payload);
}

void RainbowMode::customLoop(unsigned long long steps) {
    bool setLed = this->isFirstRun();

    if (this->isNewBrightness()) {
        this->controllerService->setBrightness(this->newBrightness);
        this->brightness = this->newBrightness;
        setLed = true;
    }

    if (this->isNewSaturation()) {
        this->saturation = this->newSaturation;
        setLed = true;
    }

    if (this->isNewPosition()) {
        this->position = this->newPosition;
        setLed = true;
    }

    if (setLed) {
        for (int i = 0; i < LED_NUM_LEDS; i++) {
            int colorIndex = map(i, 0, LED_NUM_LEDS, 0, 255);
            int hue = colorIndex + this->position;

            CRGB color = CHSV(hue, this->saturation, this->brightness);
            this->controllerService->setLed(i, color);
        }
    }

    this->controllerService->confirmLedConfig();
}

bool RainbowMode::isNewBrightness() {
    return this->brightness != this->newBrightness;
}

bool RainbowMode::isNewSaturation() {
    return this->saturation != this->newSaturation;
}

bool RainbowMode::isNewPosition() {
    return this->position != this->newPosition;
}
