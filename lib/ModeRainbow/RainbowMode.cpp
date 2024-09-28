#include "RainbowMode.h"


RainbowMode::RainbowMode(ControllerService* controllerService) : AbstractMode(controllerService) {
    this->modeTitle = "Rainbow Mode";
    this->modeDescription = "This mode loops through all LEDs.";
    this->modeInternalName = "RainbowMode";
    this->modeAuthor = "Friedjof Noweck";
    this->modeContact = "programming@noweck.info";
    this->modeVersion = "0.1.0";
    this->modeLicense = "MIT";
}

void RainbowMode::customSetup() {
    // register mqtt topics
    this->pushSaturation = this->controllerService->subscribeModeTopic(
        this->modeInternalName, "saturation", this->saturation, boundaries_t{0, 255}, payload_e::BYTE, std::function<void(String)>(std::bind(&RainbowMode::saturationCallback, this, std::placeholders::_1)));
    this->pushBrightness = this->controllerService->subscribeModeTopic(
        this->modeInternalName, "brightness", this->brightness, boundaries_t{0, 255}, payload_e::BYTE, std::function<void(String)>(std::bind(&RainbowMode::brightnessCallback, this, std::placeholders::_1)));
    this->pushPosition = this->controllerService->subscribeModeTopic(
        this->modeInternalName, "position", this->position, boundaries_t{0, LED_NUM_LEDS}, payload_e::INT, std::function<void(String)>(std::bind(&RainbowMode::positionCallback, this, std::placeholders::_1)));
    this->pushMoving = this->controllerService->subscribeModeTopic(
        this->modeInternalName, "moving", this->moving ? "true" : "false", payload_e::BOOL, std::function<void(String)>(std::bind(&RainbowMode::movingCallback, this, std::placeholders::_1)));
    this->pushSpeed = this->controllerService->subscribeModeTopic(
        this->modeInternalName, "speed", this->speed, boundaries_t{0, 255}, payload_e::BYTE, std::function<void(String)>(std::bind(&RainbowMode::speedCallback, this, std::placeholders::_1)));
}

void RainbowMode::customLoop(unsigned long long steps) {
    if (this->isFirstRun()) {
        this->controllerService->setBrightness(this->brightness);
    }

    if (this->isNewBrightness()) {
        this->brightness = this->newBrightness;
        this->controllerService->setBrightness(this->brightness);
    }

    if (this->isNewSaturation()) {
        this->saturation = this->newSaturation;
    }

    if (this->isNewPosition()) {
        this->position = this->newPosition;
    }

    if (this->isNewSpeed()) {
        this->speed = this->newSpeed;
    }

    if (this->isNewMoving()) {
        this->moving = this->newMoving;

        if (!this->moving) {
            this->position = this->currentStep % LED_NUM_LEDS;

            // publish the position
            this->pushPosition(String(this->position % LED_NUM_LEDS));
        }
    }

    for (int i = 0; i < LED_NUM_LEDS; i++) {
        int hue = map((i + (this->moving ? this->currentStep : this->position)) % LED_NUM_LEDS, 0, LED_NUM_LEDS, 0, 255);

        CRGB color = CHSV(hue, this->saturation, this->brightness);
        this->controllerService->setLed(i, color);
    }

    if (steps % (this->speed + 1) == 0) {
        this->currentStep += this->moving ? 1 : 0;
    }

    this->controllerService->confirmLedConfig();
}

void RainbowMode::brightnessCallback(String payload) {
    this->controllerService->logMessage(LOG_LEVEL_DEBUG, LOG_MODE_ALL, "RainbowMode brightness callback: " + payload);

    this->newBrightness = payload.toInt();

    // publish the brightness to the pub topic
    if (this->pushBrightness != nullptr) {
        this->pushBrightness(payload);
    } else {
        this->controllerService->logMessage(LOG_LEVEL_ERROR, LOG_MODE_ALL, "RainbowMode brightness callback: pushBrightnessTopicFun is null");
    }
}

void RainbowMode::saturationCallback(String payload) {
    this->controllerService->logMessage(LOG_LEVEL_DEBUG, LOG_MODE_ALL, "RainbowMode saturation callback: " + payload);

    this->newSaturation = payload.toInt();

    // publish the saturation to the pub topic
    if (this->pushSaturation != nullptr) {
        this->pushSaturation(payload);
    } else {
        this->controllerService->logMessage(LOG_LEVEL_ERROR, LOG_MODE_ALL, "RainbowMode saturation callback: pushSaturationTopicFun is null");
    }
}

void RainbowMode::positionCallback(String payload) {
    this->controllerService->logMessage(LOG_LEVEL_DEBUG, LOG_MODE_ALL, "RainbowMode position callback: " + payload);

    this->newPosition = payload.toInt();

    // publish the position to the pub topic
    if (this->pushPosition != nullptr) {
        this->pushPosition(payload);
    } else {
        this->controllerService->logMessage(LOG_LEVEL_ERROR, LOG_MODE_ALL, "RainbowMode position callback: pushPositionTopicFun is null");
    }
}

void RainbowMode::movingCallback(String payload) {
    this->controllerService->logMessage(LOG_LEVEL_DEBUG, LOG_MODE_ALL, "RainbowMode moving callback: " + payload);

    this->newMoving = payload == "true";

    // publish the moving to the pub topic
    if (this->pushMoving != nullptr) {
        this->pushMoving(payload);
    } else {
        this->controllerService->logMessage(LOG_LEVEL_ERROR, LOG_MODE_ALL, "RainbowMode moving callback: pushMovingTopicFun is null");
    }
}

void RainbowMode::speedCallback(String payload) {
    this->controllerService->logMessage(LOG_LEVEL_DEBUG, LOG_MODE_ALL, "RainbowMode speed callback: " + payload);

    this->newSpeed = payload.toInt();

    // publish the speed to the pub topic
    if (this->pushSpeed != nullptr) {
        this->pushSpeed(payload);
    } else {
        this->controllerService->logMessage(LOG_LEVEL_ERROR, LOG_MODE_ALL, "RainbowMode speed callback: pushSpeedTopicFun is null");
    }
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

bool RainbowMode::isNewMoving() {
    return this->moving != this->newMoving;
}

bool RainbowMode::isNewSpeed() {
    return this->speed != this->newSpeed;
}
