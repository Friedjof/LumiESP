#include "UnicornMode.h"


UnicornMode::UnicornMode(ControllerService* controllerService) : AbstractMode(controllerService) {
    this->modeTitle = "Unicorn Mode";
    this->modeDescription = "This mode sparkles like a unicorn.";
    this->modeInternalName = "UnicornMode";
    this->modeAuthor = "Friedjof Noweck";
    this->modeContact = "programming@noweck.info";
    this->modeVersion = "0.1.0";
    this->modeLicense = "MIT";
}

void UnicornMode::customSetup() {
    // register mqtt topics
    this->pushHexOnTopic = this->controllerService->subscribeModeTopic(
        this->modeInternalName, "hexOn", this->hexColorOn.c_str(), payload_e::COLOR, std::function<void(String)>(std::bind(&UnicornMode::hexOnCallback, this, std::placeholders::_1)));
    this->pushHexOffTopic = this->controllerService->subscribeModeTopic(
        this->modeInternalName, "hexOff", this->hexColorOff.c_str(), payload_e::COLOR, std::function<void(String)>(std::bind(&UnicornMode::hexOffCallback, this, std::placeholders::_1)));
    this->pushBrightnessTopic = this->controllerService->subscribeModeTopic(
        this->modeInternalName, "brightness", this->brightness, boundaries_t{0, 255}, payload_e::BYTE, std::function<void(String)>(std::bind(&UnicornMode::brightnessCallback, this, std::placeholders::_1)));
    this->pushIntensityTopic = this->controllerService->subscribeModeTopic(
        this->modeInternalName, "intensity", this->intensity, boundaries_t{0, 255}, payload_e::BYTE, std::function<void(String)>(std::bind(&UnicornMode::intensityCallback, this, std::placeholders::_1)));
    this->pushSpeedTopic = this->controllerService->subscribeModeTopic(
        this->modeInternalName, "speed", this->speed, boundaries_t{1, 100}, payload_e::INT, std::function<void(String)>(std::bind(&UnicornMode::speedCallback, this, std::placeholders::_1)));
    this->pushCrossfadeTopic = this->controllerService->subscribeModeTopic(
        this->modeInternalName, "crossfade", this->crossfade ? "true" : "false", payload_e::BOOL, std::function<void(String)>(std::bind(&UnicornMode::crossfadeCallback, this, std::placeholders::_1)));
}

void UnicornMode::customLoop(unsigned long long steps) {
    if (this->isFirstRun()) {
        this->controllerService->setBrightness(this->brightness);
        this->controllerService->confirmLedConfig();
    }

    if (this->isNewHexOn()) {
        this->hexColorOn = this->newHexColorOn;
    }

    if (this->isNewHexOff()) {
        this->hexColorOff = this->newHexColorOff;
    }

    if (this->isNewBrightness()) {
        this->brightness = this->newBrightness;
    }

    if (this->isNewIntensity()) {
        this->intensity = this->newIntensity;
    }

    if (this->isNewSpeed()) {
        this->speed = this->newSpeed;
    }

    if (this->isNewCrossfade()) {
        this->crossfade = this->newCrossfade;
    }

    // if the steps are not divisible by the speed, return
    if (steps % this->speed != 0) {
        return;
    }

    // set random leds to the hex color
    for (int i = 0; i < LED_NUM_LEDS; i++) {
        if (random(MIN_RANDOM, MAX_RANDOM) < this->intensity) {
            this->controllerService->setLed(i, this->hexColorOn);
        } else {
            this->controllerService->setLed(i, this->hexColorOff);
        }
    }

    this->controllerService->setBrightness(this->newBrightness);

    this->controllerService->confirmLedConfig();
}

void UnicornMode::hexOnCallback(String payload) {
    this->controllerService->logMessage(LOG_LEVEL_DEBUG, LOG_MODE_ALL, "UnicornMode hex on callback: " + payload);

    this->newHexColorOn = payload;

    // publish the hex on to the pub topic
    this->pushHexOnTopic(payload);
}

void UnicornMode::hexOffCallback(String payload) {
    this->controllerService->logMessage(LOG_LEVEL_DEBUG, LOG_MODE_ALL, "UnicornMode hex off callback: " + payload);

    this->newHexColorOff = payload;

    // publish the hex off to the pub topic
    this->pushHexOffTopic(payload);
}

void UnicornMode::brightnessCallback(String payload) {
    this->controllerService->logMessage(LOG_LEVEL_DEBUG, LOG_MODE_ALL, "UnicornMode brightness callback: " + payload);

    this->newBrightness = payload.toInt();

    // publish the brightness to the pub topic
    this->pushBrightnessTopic(payload);
}

void UnicornMode::intensityCallback(String payload) {
    this->controllerService->logMessage(LOG_LEVEL_DEBUG, LOG_MODE_ALL, "UnicornMode intensity callback: " + payload);

    this->newIntensity = payload.toInt();

    // publish the intensity to the pub topic
    this->pushIntensityTopic(payload);
}

void UnicornMode::speedCallback(String payload) {
    this->controllerService->logMessage(LOG_LEVEL_DEBUG, LOG_MODE_ALL, "UnicornMode speed callback: " + payload);

    this->newSpeed = payload.toInt();

    // publish the speed to the pub topic
    this->pushSpeedTopic(payload);
}

void UnicornMode::crossfadeCallback(String payload) {
    this->controllerService->logMessage(LOG_LEVEL_DEBUG, LOG_MODE_ALL, "UnicornMode crossfade callback: " + payload);

    this->newCrossfade = payload.toInt();

    // publish the crossfade to the pub topic
    this->pushCrossfadeTopic(payload);
}

bool UnicornMode::isNewHexOn() {
    return this->hexColorOn != this->newHexColorOn;
}

bool UnicornMode::isNewHexOff() {
    return this->hexColorOff != this->newHexColorOff;
}

bool UnicornMode::isNewBrightness() {
    return this->brightness != this->newBrightness;
}

bool UnicornMode::isNewIntensity() {
    return this->intensity != this->newIntensity;
}

bool UnicornMode::isNewSpeed() {
    return this->speed != this->newSpeed;
}

bool UnicornMode::isNewCrossfade() {
    return this->crossfade != this->newCrossfade;
}
