#include "StaticMode.h"


StaticMode::StaticMode(ControllerService* controllerService) : AbstractMode(controllerService) {
    this->modeTitle = "Static Mode";
    this->modeDescription = "This mode sets the LEDs to a static color configured via MQTT.";
    this->modeInternalName = "StaticMode";
    this->modeAuthor = "Friedjof Noweck";
    this->modeContact = "programming@noweck.info";
    this->modeVersion = "0.1.0";
    this->modeLicense = "MIT";
}

void StaticMode::customSetup() {
    // register mqtt topics
    this->pushHex = this->controllerService->subscribeModeTopic(
        this->modeInternalName, "hex", this->hexColor.c_str(), payload_e::COLOR, std::function<void(String)>(std::bind(&StaticMode::hexCallback, this, std::placeholders::_1)));
    this->pushBrightness = this->controllerService->subscribeModeTopic(
        this->modeInternalName, "brightness", this->brightness, boundaries_t{0, 255}, payload_e::BYTE, std::function<void(String)>(std::bind(&StaticMode::brightnessCallback, this, std::placeholders::_1)));
    this->pushUltrasonicEnabled = this->controllerService->subscribeModeTopic(
        this->modeInternalName, "ultrasonicEnabled", this->ultrasonicEnabled ? "true" : "false", payload_e::BOOL, std::function<void(String)>(std::bind(&StaticMode::ultrasonicEnabledCallback, this, std::placeholders::_1)));
    this->pushDistance = this->controllerService->subscribeModeTopic(this->modeInternalName, "distance", String(this->distance));

    // enable ultrasonic sensor if enabled in the config/config.h file
    if (ULTRASONIC) {
        this->controllerService->logMessage(LOG_LEVEL_DEBUG, LOG_MODE_ALL, "StaticMode uses ultrasonic sensor");

        pinMode(TRIGGER_PIN, OUTPUT);
        pinMode(ECHO_PIN, INPUT);
    }
}

void StaticMode::customLoop(unsigned long long steps) {
    if (this->isFirstRun()) {
        this->controllerService->setBrightness(this->brightness);
        this->controllerService->setHexColor(this->hexColor);

        this->controllerService->confirmLedConfig();
    }

    if (this->isNewUltrasonicEnabled()) {
        this->ultrasonicEnabled = this->newUltrasonicEnabled;

        if (this->ultrasonicEnabled) {
            this->controllerService->logMessage(LOG_LEVEL_DEBUG, LOG_MODE_ALL, "StaticMode uses ultrasonic sensor");
        } else {
            this->controllerService->logMessage(LOG_LEVEL_DEBUG, LOG_MODE_ALL, "StaticMode does not use ultrasonic sensor");
        }
    }

    // if ultrasonic sensor is enabled in the config/config.h file
    if (ULTRASONIC && this->ultrasonicEnabled) {
        // read distance from ultrasonic sensor
        this->readAndSetDistance();

        // update distance every PUSH_DISTANCE_EVERY_ITERATIONS to avoid flooding the mqtt broker
        if (steps % PUSH_DISTANCE_EVERY_ITERATIONS == 0) {
            // publish the distance to the pub topic
            this->pushDistance(String(this->newDistance));

            this->controllerService->logMessage(LOG_LEVEL_DEBUG, LOG_MODE_ALL, "StaticMode push distance: " + String(this->newDistance));
        }

        // if the distance is greater than DISTANCE_NO_CHANGE, the brightness will not change
        if (this->newDistance < DISTANCE_NO_CHANGE && steps % UPDATE_DISTANCE_EVERY_ITERATIONS == 0) {
            uint8_t tmpScaling = 0;

            // scale the distance to town into 9 steps
            if (this->newDistance <= DISTANCE_MIN) {
                tmpScaling = 0;
            } else if (this->newDistance >= DISTANCE_MAX) {
                tmpScaling = 8;
            } else {
                tmpScaling = map(this->newDistance, DISTANCE_MIN, DISTANCE_MAX, 0, 8);
            }

            uint8_t brightness = 0;

            // scale the brightness to 255
            if (tmpScaling == 0) {
                brightness = 0;
            } else if (tmpScaling == 8) {
                brightness = 255;
            } else {
                brightness = map(tmpScaling, 0, 8, 0, 255);
            }

            if (brightness != this->newBrightness) {
                this->newBrightness = brightness;

                this->pushBrightness(String(this->newBrightness));

                this->controllerService->logMessage(LOG_LEVEL_DEBUG, LOG_MODE_ALL, "StaticMode set brightness: " + String(this->newBrightness));
            }
        }
    }

    if (this->isNewDistance()) {
        // smooth distance change using median filter
        this->updateDistance(this->newDistance);
    }

    if (this->isNewHexColor()) {
        this->hexColor = this->newHexColor;

        this->controllerService->setHexColor(this->newHexColor);
        this->controllerService->confirmLedConfig();

        this->controllerService->logMessage(LOG_LEVEL_DEBUG, LOG_MODE_ALL, "StaticMode set hex color: " + this->hexColor);
    }

    if (this->isNewBrightness()) {
        this->brightness = this->newBrightness;

        this->controllerService->setBrightness(this->newBrightness);
        this->controllerService->confirmLedConfig();
    }
}

void StaticMode::hexCallback(String payload) {
    this->controllerService->logMessage(LOG_LEVEL_DEBUG, LOG_MODE_ALL, "StaticMode hex callback: " + payload);

    this->newHexColor = payload;

    // publish the hex to the pub topic
    this->pushHex(payload);
}

void StaticMode::brightnessCallback(String payload) {
    this->controllerService->logMessage(LOG_LEVEL_DEBUG, LOG_MODE_ALL, "StaticMode brightness callback: " + payload);

    this->newBrightness = payload.toInt();

    // publish the brightness to the pub topic
    this->pushBrightness(payload);
}

void StaticMode::ultrasonicEnabledCallback(String payload) {
    this->controllerService->logMessage(LOG_LEVEL_DEBUG, LOG_MODE_ALL, "StaticMode ultrasonic enabled callback: " + payload);

    this->newUltrasonicEnabled = payload == "true" || payload == "1";

    // publish the ultrasonic enabled to the pub topic
    this->pushUltrasonicEnabled(payload);
}

bool StaticMode::isNewHexColor() {
    return this->hexColor != this->newHexColor;
}

bool StaticMode::isNewBrightness() {
    return this->brightness != this->newBrightness;
}

bool StaticMode::isNewUltrasonicEnabled() {
    return this->ultrasonicEnabled != this->newUltrasonicEnabled;
}

bool StaticMode::isNewDistance() {
    return this->distance != this->newDistance;
}

void StaticMode::readAndSetDistance() {
    digitalWrite(TRIGGER_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIGGER_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIGGER_PIN, LOW);

    // Read the pulse and calculate the distance
    float pulseDuration = pulseIn(ECHO_PIN, HIGH);
    float calculatedDistance = pulseDuration * 0.034 / 2;

    if (calculatedDistance <= 0) {
        this->controllerService->logMessage(LOG_LEVEL_DEBUG, LOG_MODE_ALL, "StaticMode invalid distance: " + String(calculatedDistance));

        return;
    }

    // Update the distance using the median filter
    this->newDistance = this->updateDistance(calculatedDistance);
}

// Update the distance using a median filter
float StaticMode::updateDistance(float newDistance) {
    // Add the new distance to the history
    this->distanceHistory.push_back(newDistance);

    // Remove the oldest distance if the history exceeds the maximum size
    if (this->distanceHistory.size() > this->maxHistorySize) {
        this->distanceHistory.pop_front();
    }

    // Calculate the median
    std::vector<float> sortedDistances(this->distanceHistory.begin(), this->distanceHistory.end());
    std::sort(sortedDistances.begin(), sortedDistances.end());

    if (this->distanceHistory.size() % 2 == 0) {
        this->distance = (sortedDistances[sortedDistances.size() / 2 - 1] + sortedDistances[sortedDistances.size() / 2]) / 2;
    } else {
        this->distance = sortedDistances[sortedDistances.size() / 2];
    }

    return this->distance;
}
