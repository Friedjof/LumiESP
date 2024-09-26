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
    this->controllerService->registerMode(this->modeInternalName, [this](int steps) {
        this->loop(steps);
    });

    this->pushPubHexTopicFun = this->controllerService->subscribeModeTopic(
        this->modeInternalName, "hex", "#000000", payload_e::COLOR, std::function<void(String)>(std::bind(&StaticMode::hexCallback, this, std::placeholders::_1)));
    this->pushPubBrightnessTopicFun = this->controllerService->subscribeModeTopic(
        this->modeInternalName, "brightness", 255, boundaries_t{0, 255}, payload_e::BYTE, std::function<void(String)>(std::bind(&StaticMode::brightnessCallback, this, std::placeholders::_1)));
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

void StaticMode::customLoop(unsigned long long steps) {
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

bool StaticMode::isNewHexColor() {
    return this->hexColor != this->newHexColor;
}

bool StaticMode::isNewBrightness() {
    return this->brightness != this->newBrightness;
}
