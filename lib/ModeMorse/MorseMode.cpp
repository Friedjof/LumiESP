#include "MorseMode.h"


MorseMode::MorseMode(ControllerService* controllerService) : AbstractMode(controllerService) {
    this->modeTitle = "Morse Mode";
    this->modeDescription = "This is a mode can be used to send Morse code messages on the LED strip.";
    this->modeInternalName = "MorseMode";
    this->modeAuthor = "Friedjof Noweck";
    this->modeContact = "programming@noweck.info";
    this->modeVersion = "1.0.0";
    this->modeLicense = "MIT";
}

void MorseMode::customSetup() {
    this->pushColorOn = this->controllerService->subscribeModeTopic(
        this->modeInternalName, "coloron", this->colorOn.c_str(), payload_e::COLOR, std::bind(&MorseMode::onColorOnCallback, this, std::placeholders::_1));
    this->pushColorOff = this->controllerService->subscribeModeTopic(
        this->modeInternalName, "coloroff", this->colorOff.c_str(), payload_e::COLOR, std::bind(&MorseMode::onColorOffCallback, this, std::placeholders::_1));
    this->pushBrightness = this->controllerService->subscribeModeTopic(
        this->modeInternalName, "brightness", this->brightness, boundaries_t{0, 255}, payload_e::BYTE, std::bind(&MorseMode::onBrightnessCallback, this, std::placeholders::_1));
    this->pushMessage = this->controllerService->subscribeModeTopic(
        this->modeInternalName, "message", this->message.c_str(), payload_e::STRING, std::bind(&MorseMode::onMessageCallback, this, std::placeholders::_1));
}

void MorseMode::customLoop(unsigned long long steps) {
    // TODO: Implement custom loop
}

void MorseMode::onColorOnCallback(String payload) {
    this->newColorOn = payload;
    
    if (this->pushColorOn != nullptr) {
        this->pushColorOn(this->newColorOn);
    }
}

void MorseMode::onColorOffCallback(String payload) {
    this->newColorOff = payload;
    
    if (this->pushColorOff != nullptr) {
        this->pushColorOff(this->newColorOff);
    }
}

void MorseMode::onBrightnessCallback(String payload) {
    this->newBrightness = payload.toInt();
    
    if (this->pushBrightness != nullptr) {
        this->pushBrightness(String(this->newBrightness));
    }
}

void MorseMode::onMessageCallback(String payload) {
    this->newMessage = payload;
    
    if (this->pushMessage != nullptr) {
        this->pushMessage(this->newMessage);
    }
}

bool MorseMode::isNewColorOn() {
    return this->colorOn != this->newColorOn;
}

bool MorseMode::isNewColorOff() {
    return this->colorOff != this->newColorOff;
}

bool MorseMode::isNewBrightness() {
    return this->brightness != this->newBrightness;
}

bool MorseMode::isNewMessage() {
    return this->message != this->newMessage;
}
