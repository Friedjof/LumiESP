#include "StaticMode.h"


StaticMode::StaticMode(LedService* ledService, LoggingService* loggingService, MqttService* mqttService) : AbstractMode(ledService, loggingService, mqttService) {
    this->modeTitle = "Static Mode";
    this->modeDescription = "This mode sets the LEDs to a static color configured via MQTT.";
    this->modeInternalName = "StaticMode";
    this->modeAuthor = "Friedjof Noweck";
    this->modeContact = "git@noweck.info";
    this->modeVersion = "0.1.0";
    this->modeLicense = "MIT";
}

void StaticMode::customSetup() {
    this->ledService->registerMode(this->modeInternalName, this);

    // TODO: topics and default values should be configurable in the config.h
    this->pushPubHexTopicFun = this->mqttService->subscribeModeTopic(
        this->modeInternalName, "hex", "#000000", payload_e::COLOR, std::function<void(String)>(std::bind(&StaticMode::hexCallback, this, std::placeholders::_1)));
    this->pushPubBrightnessTopicFun = this->mqttService->subscribeModeTopic(
        this->modeInternalName, "brightness", 255, boundaries_t{0, 255}, payload_e::BYTE, std::function<void(String)>(std::bind(&StaticMode::brightnessCallback, this, std::placeholders::_1)));
}

void StaticMode::hexCallback(String payload) {
    this->loggingService->logMessage(LOG_LEVEL_DEBUG, "StaticMode hex callback: " + payload);

    this->newHexColor = payload;

    // publish the hex to the pub topic
    this->pushPubHexTopicFun(payload);
}

void StaticMode::brightnessCallback(String payload) {
    this->loggingService->logMessage(LOG_LEVEL_DEBUG, "StaticMode brightness callback: " + payload);

    this->newBrightness = payload.toInt();

    // publish the brightness to the pub topic
    this->pushPubBrightnessTopicFun(payload);
}

void StaticMode::customLoop(int steps) {
    if (this->isNewHexColor()) {
        this->ledService->setHexColor(this->newHexColor);
        this->hexColor = this->newHexColor;

        this->loggingService->logMessage(LOG_LEVEL_DEBUG, "StaticMode set hex color: " + this->hexColor);
    }

    if (this->isNewBrightness()) {
        this->ledService->setBrightness(this->newBrightness);
        this->brightness = this->newBrightness;

        this->loggingService->logMessage(LOG_LEVEL_DEBUG, "StaticMode set brightness: " + String(this->brightness));
    }
}

bool StaticMode::isNewHexColor() {
    return this->hexColor != this->newHexColor;
}

bool StaticMode::isNewBrightness() {
    return this->brightness != this->newBrightness;
}
