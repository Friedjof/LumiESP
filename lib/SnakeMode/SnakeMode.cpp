#include "SnakeMode.h"


SnakeMode::SnakeMode(ControllerService* controllerService) : AbstractMode(controllerService) {
    this->modeTitle = "Snake Mode";
    this->modeDescription = "This mode creates a snake on the LED strip.";
    this->modeInternalName = "SnakeMode";
    this->modeAuthor = "Friedjof Noweck";
    this->modeContact = "git@noweck.info";
    this->modeVersion = "0.1.0";
    this->modeLicense = "MIT";
}

void SnakeMode::customSetup() {
    // register mqtt topics
    this->pushOnHexTopic = this->controllerService->subscribeModeTopic(
        this->modeInternalName, "onhex", this->onHexColor.c_str(), payload_e::COLOR, std::function<void(String)>(std::bind(&SnakeMode::onHexCallback, this, std::placeholders::_1)));
    this->pushOffHexTopic = this->controllerService->subscribeModeTopic(
        this->modeInternalName, "offhex", this->offHexColor.c_str(), payload_e::COLOR, std::function<void(String)>(std::bind(&SnakeMode::offHexCallback, this, std::placeholders::_1)));
    this->pushBrightnessTopic = this->controllerService->subscribeModeTopic(
        this->modeInternalName, "brightness", this->brightness, boundaries_t{0, 255}, payload_e::BYTE, std::function<void(String)>(std::bind(&SnakeMode::brightnessCallback, this, std::placeholders::_1)));
    this->pushSnakeLengthTopic = this->controllerService->subscribeModeTopic(
        this->modeInternalName, "snakelength", this->snakeLength, boundaries_t{1, LED_NUM_LEDS}, payload_e::INT, std::function<void(String)>(std::bind(&SnakeMode::snakeLengthCallback, this, std::placeholders::_1)));
    this->pushDirectionTopic = this->controllerService->subscribeModeTopic(
        this->modeInternalName, "direction", this->direction? "true": "false", payload_e::BOOL, std::function<void(String)>(std::bind(&SnakeMode::directionCallback, this, std::placeholders::_1)));
    this->pushCrawlingTopic = this->controllerService->subscribeModeTopic(
        this->modeInternalName, "crawling", this->crawling? "true": "false", payload_e::BOOL, std::function<void(String)>(std::bind(&SnakeMode::crawlingCallback, this, std::placeholders::_1)));
    this->pushPositionTopic = this->controllerService->subscribeModeTopic(
        this->modeInternalName, "position", this->position, boundaries_t{0, LED_NUM_LEDS - 1}, payload_e::INT, std::function<void(String)>(std::bind(&SnakeMode::positionCallback, this, std::placeholders::_1)));
    this->pushSpeedTopic = this->controllerService->subscribeModeTopic(
        this->modeInternalName, "speed", this->speed, boundaries_t{0, 255}, payload_e::BYTE, std::function<void(String)>(std::bind(&SnakeMode::speedCallback, this, std::placeholders::_1)));
}

void SnakeMode::customLoop(unsigned long long steps) {
    if (this->isFirstRun()) {
        this->controllerService->setBrightness(this->brightness);
    }

    if (this->isNewBrightness()) {
        this->controllerService->setBrightness(this->newBrightness);
        this->brightness = this->newBrightness;
    }

    if (this->isNewOnHexColor()) {
        this->onHexColor = this->newOnHexColor;
    }

    if (this->isNewOffHexColor()) {
        this->offHexColor = this->newOffHexColor;
    }

    if (this->isNewSnakeLength()) {
        this->snakeLength = this->newSnakeLength;
    }

    if (this->isNewDirection()) {
        this->direction = this->newDirection;
    }

    if (this->isNewCrawling()) {
        this->crawling = this->newCrawling;

        if (!this->crawling) {
            this->newPosition = this->snakeHead;
            this->pushPositionTopic(String(this->newPosition));
        }
    }

    if (this->isNewPosition()) {
        this->position = this->newPosition;
        
        if (!this->crawling) {
            this->snakeHead = this->position;
        }
    }

    if (this->isNewSpeed()) {
        this->speed = this->newSpeed;
    }

    // set the off color for all leds
    this->controllerService->setHexColor(this->offHexColor);

    // set the on color for the snake
    for (int i = snakeHead; i < snakeHead + snakeLength; i++) {
        this->controllerService->setLed(i % LED_NUM_LEDS, this->onHexColor);
    }

    // move the snake
    if (steps % (this->speed + 1) == 0) {
        this->snakeHead = (this->snakeHead + (this->crawling ? (this->direction ? 1 : LED_NUM_LEDS - 1) : 0)) % LED_NUM_LEDS;
    }

    this->controllerService->confirmLedConfig();
}

void SnakeMode::onHexCallback(String payload) {
    this->controllerService->logMessage(LOG_LEVEL_DEBUG, LOG_MODE_ALL, "SnakeMode hex callback: " + payload);

    this->newOnHexColor = payload;

    // publish the on hex to the pub topic
    if (this->pushOnHexTopic != nullptr) {
        this->pushOnHexTopic(payload);
    } else {
        this->controllerService->logMessage(LOG_LEVEL_ERROR, LOG_MODE_ALL, "SnakeMode on hex topic is not set. pushOnHexTopic is null.");
    }
}

void SnakeMode::offHexCallback(String payload) {
    this->controllerService->logMessage(LOG_LEVEL_DEBUG, LOG_MODE_ALL, "SnakeMode hex callback: " + payload);

    this->newOffHexColor = payload;

    // publish the off hex to the pub topic
    if (this->pushOffHexTopic != nullptr) {
        this->pushOffHexTopic(payload);
    } else {
        this->controllerService->logMessage(LOG_LEVEL_ERROR, LOG_MODE_ALL, "SnakeMode off hex topic is not set. pushOffHexTopic is null.");
    }
}

void SnakeMode::brightnessCallback(String payload) {
    this->controllerService->logMessage(LOG_LEVEL_DEBUG, LOG_MODE_ALL, "SnakeMode brightness callback: " + payload);

    this->newBrightness = payload.toInt();

    // publish the brightness to the pub topic
    if (this->pushBrightnessTopic != nullptr) {
        this->pushBrightnessTopic(payload);
    } else {
        this->controllerService->logMessage(LOG_LEVEL_ERROR, LOG_MODE_ALL, "SnakeMode brightness topic is not set. pushBrightnessTopic is null.");
    }
}

void SnakeMode::snakeLengthCallback(String payload) {
    this->controllerService->logMessage(LOG_LEVEL_DEBUG, LOG_MODE_ALL, "SnakeMode queue length callback: " + payload);

    this->newSnakeLength = payload.toInt();

    // publish the queue length to the pub topic
    if (this->pushSnakeLengthTopic != nullptr) {
        this->pushSnakeLengthTopic(payload);
    } else {
        this->controllerService->logMessage(LOG_LEVEL_ERROR, LOG_MODE_ALL, "SnakeMode queue length topic is not set. pushSnakeLengthTopic is null.");
    }
}

void SnakeMode::directionCallback(String payload) {
    this->controllerService->logMessage(LOG_LEVEL_DEBUG, LOG_MODE_ALL, "SnakeMode direction callback: " + payload);

    this->newDirection = payload == "true";

    // publish the direction to the pub topic
    if (this->pushDirectionTopic != nullptr) {
        this->pushDirectionTopic(payload);
    } else {
        this->controllerService->logMessage(LOG_LEVEL_ERROR, LOG_MODE_ALL, "SnakeMode direction topic is not set. pushDirectionTopic is null.");
    }
}

void SnakeMode::crawlingCallback(String payload) {
    this->controllerService->logMessage(LOG_LEVEL_DEBUG, LOG_MODE_ALL, "SnakeMode crawling callback: " + payload);

    this->newCrawling = payload == "true";

    // publish the crawling to the pub topic
    if (this->pushCrawlingTopic != nullptr) {
        this->pushCrawlingTopic(payload);
    } else {
        this->controllerService->logMessage(LOG_LEVEL_ERROR, LOG_MODE_ALL, "SnakeMode crawling topic is not set. pushCrawlingTopic is null.");
    }
}

void SnakeMode::positionCallback(String payload) {
    this->controllerService->logMessage(LOG_LEVEL_DEBUG, LOG_MODE_ALL, "SnakeMode position callback: " + payload);

    this->newPosition = payload.toInt();

    // publish the position to the pub topic
    if (this->pushPositionTopic != nullptr) {
        this->pushPositionTopic(payload);
    } else {
        this->controllerService->logMessage(LOG_LEVEL_ERROR, LOG_MODE_ALL, "SnakeMode position topic is not set. pushPositionTopic is null.");
    }
}

void SnakeMode::speedCallback(String payload) {
    this->controllerService->logMessage(LOG_LEVEL_DEBUG, LOG_MODE_ALL, "SnakeMode speed callback: " + payload);

    this->newSpeed = payload.toInt();

    // publish the speed to the pub topic
    if (this->pushSpeedTopic != nullptr) {
        this->pushSpeedTopic(payload);
    } else {
        this->controllerService->logMessage(LOG_LEVEL_ERROR, LOG_MODE_ALL, "SnakeMode speed topic is not set. pushSpeedTopic is null.");
    }
}

bool SnakeMode::isNewBrightness() {
    return this->brightness != this->newBrightness;
}

bool SnakeMode::isNewOnHexColor() {
    return this->onHexColor != this->newOnHexColor;
}

bool SnakeMode::isNewOffHexColor() {
    return this->offHexColor != this->newOffHexColor;
}

bool SnakeMode::isNewSnakeLength() {
    return this->snakeLength != this->newSnakeLength;
}

bool SnakeMode::isNewDirection() {
    return this->direction != this->newDirection;
}

bool SnakeMode::isNewCrawling() {
    return this->crawling != this->newCrawling;
}

bool SnakeMode::isNewPosition() {
    return this->position != this->newPosition;
}

bool SnakeMode::isNewSpeed() {
    return this->speed != this->newSpeed;
}
