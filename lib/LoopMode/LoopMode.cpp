#include "LoopMode.h"


LoopMode::LoopMode(ControllerService* controllerService) : AbstractMode(controllerService) {
    this->modeTitle = "Loop Mode";
    this->modeDescription = "This mode loops through all LEDs.";
    this->modeInternalName = "LoopMode";
    this->modeAuthor = "Friedjof Noweck";
    this->modeContact = "git@noweck.info";
    this->modeVersion = "0.1.0";
    this->modeLicense = "MIT";
}

void LoopMode::customSetup() {
    this->controllerService->registerMode(this->modeInternalName, [this](int steps) {
        this->loop(steps);
    });

    this->pushPubOnHexTopicFun = this->controllerService->subscribeModeTopic(
        this->modeInternalName, "onhex", this->onHexColor.c_str(), payload_e::COLOR, std::function<void(String)>(std::bind(&LoopMode::onHexCallback, this, std::placeholders::_1)));
    this->pushPubOffHexTopicFun = this->controllerService->subscribeModeTopic(
        this->modeInternalName, "offhex", this->offHexColor.c_str(), payload_e::COLOR, std::function<void(String)>(std::bind(&LoopMode::offHexCallback, this, std::placeholders::_1)));
    this->pushPubBrightnessTopicFun = this->controllerService->subscribeModeTopic(
        this->modeInternalName, "brightness", 255, boundaries_t{0, 255}, payload_e::BYTE, std::function<void(String)>(std::bind(&LoopMode::brightnessCallback, this, std::placeholders::_1)));
}

void LoopMode::onHexCallback(String payload) {
    this->controllerService->logMessage(LOG_LEVEL_DEBUG, LOG_MODE_ALL, "LoopMode hex callback: " + payload);

    this->newOnHexColor = payload;

    // publish the on hex to the pub topic
    this->pushPubOnHexTopicFun(payload);
}

void LoopMode::offHexCallback(String payload) {
    this->controllerService->logMessage(LOG_LEVEL_DEBUG, LOG_MODE_ALL, "LoopMode hex callback: " + payload);

    this->newOffHexColor = payload;

    // publish the off hex to the pub topic
    this->pushPubOffHexTopicFun(payload);
}

void LoopMode::brightnessCallback(String payload) {
    this->controllerService->logMessage(LOG_LEVEL_DEBUG, LOG_MODE_ALL, "LoopMode brightness callback: " + payload);

    this->newBrightness = payload.toInt();

    // publish the brightness to the pub topic
    this->pushPubBrightnessTopicFun(payload);
}

void LoopMode::customLoop(unsigned long long steps) {
    if (this->isNewBrightness()) {
        this->controllerService->setBrightness(this->newBrightness);
        this->brightness = this->newBrightness;
    }

    int step = steps % LED_NUM_LEDS;

    for (int i = 0; i < LED_NUM_LEDS; i++) {
        if (i == step) {
            this->controllerService->setLed(i, this->onHexColor);
        } else {
            this->controllerService->setLed(i, this->offHexColor);
        }
    }

    this->onHexColor = this->newOnHexColor;
    this->offHexColor = this->newOffHexColor;

    this->controllerService->confirmLedConfig();
}

bool LoopMode::isNewBrightness() {
    return this->brightness != this->newBrightness;
}