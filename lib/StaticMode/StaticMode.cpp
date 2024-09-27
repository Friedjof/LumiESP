#include "StaticMode.h"


StaticMode::StaticMode(ControllerService* controllerService) : AbstractMode(controllerService) {
    this->modeTitle = "Static Mode";
    this->modeDescription = "This mode sets the LEDs to a static color configured via MQTT.";
    this->modeInternalName = "StaticMode";
    this->modeAuthor = "Friedjof Noweck";
    this->modeContact = "git@noweck.info";
    this->modeVersion = "0.1.0";
    this->modeLicense = "MIT";
}

void StaticMode::customSetup() {
    // register mqtt topics
    this->pushPubHexTopicFun = this->controllerService->subscribeModeTopic(
        this->modeInternalName, "hex", this->hexColor.c_str(), payload_e::COLOR, std::function<void(String)>(std::bind(&StaticMode::hexCallback, this, std::placeholders::_1)));
    this->pushPubBrightnessTopicFun = this->controllerService->subscribeModeTopic(
        this->modeInternalName, "brightness", this->brightness, boundaries_t{0, 255}, payload_e::BYTE, std::function<void(String)>(std::bind(&StaticMode::brightnessCallback, this, std::placeholders::_1)));
}

void StaticMode::customLoop(unsigned long long steps) {
    if (this->isFirstRun()) {
        this->controllerService->setBrightness(this->brightness);
        this->controllerService->setHexColor(this->hexColor);

        this->controllerService->confirmLedConfig();
    }

    if (this->isNewHexColor()) {
        this->controllerService->setHexColor(this->newHexColor);
        this->hexColor = this->newHexColor;

        this->controllerService->logMessage(LOG_LEVEL_DEBUG, LOG_MODE_ALL, "StaticMode set hex color: " + this->hexColor);

        this->controllerService->confirmLedConfig();
    }

    if (this->isNewBrightness()) {
        this->controllerService->setBrightness(this->newBrightness);
        this->brightness = this->newBrightness;

        this->controllerService->logMessage(LOG_LEVEL_DEBUG, LOG_MODE_ALL, "StaticMode set brightness: " + String(this->brightness));

        this->controllerService->confirmLedConfig();
    }
}

void StaticMode::hexCallback(String payload) {
    this->controllerService->logMessage(LOG_LEVEL_DEBUG, LOG_MODE_ALL, "StaticMode hex callback: " + payload);

    this->newHexColor = payload;

    // publish the hex to the pub topic
    this->pushPubHexTopicFun(payload);
}

void StaticMode::brightnessCallback(String payload) {
    this->controllerService->logMessage(LOG_LEVEL_DEBUG, LOG_MODE_ALL, "StaticMode brightness callback: " + payload);

    this->newBrightness = payload.toInt();

    // publish the brightness to the pub topic
    this->pushPubBrightnessTopicFun(payload);
}

bool StaticMode::isNewHexColor() {
    return this->hexColor != this->newHexColor;
}

bool StaticMode::isNewBrightness() {
    return this->brightness != this->newBrightness;
}
