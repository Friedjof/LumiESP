#include "FlashMode.h"


FlashMode::FlashMode(ControllerService* controllerService) : AbstractMode(controllerService) {
    this->modeTitle = "Init Mode";
    this->modeDescription = "This mode runs at the beginning of the application and blink the LEDs in a specific color.";
    this->modeInternalName = "FlashMode";
    this->modeAuthor = "Friedjof Noweck";
    this->modeContact = "programming@noweck.info";
    this->modeVersion = "0.1.0";
    this->modeLicense = "MIT";
}

void FlashMode::customSetup() {
    // register mqtt topics
    this->pushStartColor = this->controllerService->subscribeModeTopic(
        this->modeInternalName, "startColor", this->startColorHex.c_str(), payload_e::COLOR, std::function<void(String)>(std::bind(&FlashMode::startColorCallback, this, std::placeholders::_1)));
    this->pushStopColor = this->controllerService->subscribeModeTopic(
        this->modeInternalName, "stopColor", this->stopColorHex.c_str(), payload_e::COLOR, std::function<void(String)>(std::bind(&FlashMode::stopColorCallback, this, std::placeholders::_1)));
    this->pushBrightness = this->controllerService->subscribeModeTopic(
        this->modeInternalName, "brightness", this->brightness, boundaries_t{0, 255}, payload_e::BYTE, std::function<void(String)>(std::bind(&FlashMode::brightnessCallback, this, std::placeholders::_1)));
    this->pushMaxFlashNum = this->controllerService->subscribeModeTopic(
        this->modeInternalName, "maxFlashNum", this->maxFlashNum, boundaries_t{1, 100}, payload_e::INT, std::function<void(String)>(std::bind(&FlashMode::maxFlashNumCallback, this, std::placeholders::_1)));
    this->pushMaxCrossfadeSteps = this->controllerService->subscribeModeTopic(
        this->modeInternalName, "maxCrossfadeSteps", this->maxCrossfadeSteps, boundaries_t{1, 200}, payload_e::INT, std::function<void(String)>(std::bind(&FlashMode::maxCrossfadeStepsCallback, this, std::placeholders::_1)));
    this->pushInfiniteFlash = this->controllerService->subscribeModeTopic(
        this->modeInternalName, "infiniteFlash", this->infiniteFlash ? "true" : "false", payload_e::BOOL, std::function<void(String)>(std::bind(&FlashMode::infiniteFlashCallback, this, std::placeholders::_1)));
}

void FlashMode::customLoop(unsigned long long steps) {
    if (this->isFirstRun()) {
        this->controllerService->setBrightness(this->brightness);
        this->controllerService->setHexColor(this->startColorHex);

        this->controllerService->confirmLedConfig();

        this->blinkCounter = 0;
        this->stepCounter = 0;

        this->startColor = this->hexColor2CRGB(this->startColorHex);
        this->endColor = this->hexColor2CRGB(this->stopColorHex);
    } else {
        this->blinkCounter += this->stepCounter % this->maxCrossfadeSteps == 0 ? 1 : 0;
    }

    if (this->isNewStartColor()) {
        this->startColorHex = this->newStartColorHex;

        this->startColor = this->hexColor2CRGB(this->startColorHex);
    }

    if (this->isNewStopColor()) {
        this->stopColorHex = this->newStopColorHex;

        this->endColor = this->hexColor2CRGB(this->stopColorHex);
    }

    if (this->isNewBrightness()) {
        this->brightness = this->newBrightness;
    }

    if (this->isNewMaxFlashNum()) {
        this->maxFlashNum = this->newMaxFlashNum;
    }

    if (this->isNewMaxCrossfadeSteps()) {
        this->maxCrossfadeSteps = this->newMaxCrossfadeSteps;
    }

    if (this->isNewInfiniteFlash()) {
        this->infiniteFlash = this->newInfiniteFlash;
    }

    float fraction = (this->blinkCounter % 2 != 0 ? (this->maxCrossfadeSteps - 1) - (this->stepCounter % this->maxCrossfadeSteps) : (this->stepCounter % this->maxCrossfadeSteps)) / float(this->maxCrossfadeSteps - 1);
    CRGB currentColor = interpolateColor(this->startColor, this->endColor, fraction);

    this->controllerService->setColor(currentColor);

    this->controllerService->confirmLedConfig();

    if (this->blinkCounter / 2 >= this->maxFlashNum && !this->infiniteFlash) {
        this->controllerService->setMode(this->nextMode);
    }

    this->stepCounter++;
}

void FlashMode::setNextMode(String nextMode) {
    this->nextMode = nextMode;
}

CRGB FlashMode::interpolateColor(CRGB start, CRGB end, float fraction) {
    return CRGB(
        start.r + fraction * (end.r - start.r),
        start.g + fraction * (end.g - start.g),
        start.b + fraction * (end.b - start.b)
    );
}

CRGB FlashMode::hexColor2CRGB(String hexColor) {
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

    return color;
}

String FlashMode::expandHexColor(String hexColor)
{
    String expandedHexColor = "#";

    for (int i = 1; i < hexColor.length(); i++)
    {
        expandedHexColor += hexColor[i];
        expandedHexColor += hexColor[i];
    }

    return expandedHexColor;
}

void FlashMode::startColorCallback(String payload) {
    this->controllerService->logMessage(LOG_LEVEL_DEBUG, LOG_MODE_ALL, "FlashMode start color callback: " + payload);

    this->newStartColorHex = payload;

    // publish the start color to the pub topic
    this->pushStartColor(payload);
}

void FlashMode::stopColorCallback(String payload) {
    this->controllerService->logMessage(LOG_LEVEL_DEBUG, LOG_MODE_ALL, "FlashMode stop color callback: " + payload);

    this->newStopColorHex = payload;

    // publish the stop color to the pub topic
    this->pushStopColor(payload);
}

void FlashMode::brightnessCallback(String payload) {
    this->controllerService->logMessage(LOG_LEVEL_DEBUG, LOG_MODE_ALL, "FlashMode brightness callback: " + payload);

    this->newBrightness = payload.toInt();

    // publish the brightness to the pub topic
    this->pushBrightness(payload);
}

void FlashMode::maxFlashNumCallback(String payload) {
    this->controllerService->logMessage(LOG_LEVEL_DEBUG, LOG_MODE_ALL, "FlashMode max blink num callback: " + payload);

    this->newMaxFlashNum = payload.toInt();

    // publish the max blink num to the pub topic
    this->pushMaxFlashNum(payload);
}

void FlashMode::maxCrossfadeStepsCallback(String payload) {
    this->controllerService->logMessage(LOG_LEVEL_DEBUG, LOG_MODE_ALL, "FlashMode max dimming steps callback: " + payload);

    this->newMaxCrossfadeSteps = payload.toInt();

    // publish the max dimming steps to the pub topic
    this->pushMaxCrossfadeSteps(payload);
}

void FlashMode::infiniteFlashCallback(String payload) {
    this->controllerService->logMessage(LOG_LEVEL_DEBUG, LOG_MODE_ALL, "FlashMode infinite blink callback: " + payload);

    this->newInfiniteFlash = payload == "true";

    // publish the infinite blink to the pub topic
    this->pushInfiniteFlash(payload);
}

bool FlashMode::isNewStartColor() {
    return this->startColorHex != this->newStartColorHex;
}

bool FlashMode::isNewStopColor() {
    return this->stopColorHex != this->newStopColorHex;
}

bool FlashMode::isNewBrightness() {
    return this->brightness != this->newBrightness;
}

bool FlashMode::isNewMaxFlashNum() {
    return this->maxFlashNum != this->newMaxFlashNum;
}

bool FlashMode::isNewMaxCrossfadeSteps() {
    return this->maxCrossfadeSteps != this->newMaxCrossfadeSteps;
}

bool FlashMode::isNewInfiniteFlash() {
    return this->infiniteFlash != this->newInfiniteFlash;
}
