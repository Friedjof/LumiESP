#include "MorseMode.h"


// Morse-Code Tabelle als Map
std::map<char, std::vector<uint8_t>> morseCodeMap = {
    {'A', {1, 0, 1, 1, 1, 0, 0, 0}},                                           // A: .-
    {'B', {1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0}},                               // B: -...
    {'C', {1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0}},                         // C: -.-.
    {'D', {1, 1, 1, 0, 1, 0, 1, 0, 0, 0}},                                     // D: -..
    {'E', {1, 0, 0, 0}},                                                       // E: .
    {'F', {1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0}},                               // F: ..-.
    {'G', {1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0}},                               // G: --.
    {'H', {1, 0, 1, 0, 1, 0, 1, 0, 0, 0}},                                     // H: ....
    {'I', {1, 0, 1, 0, 0, 0}},                                                 // I: ..
    {'J', {1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 0, 0}},                   // J: .---
    {'K', {1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 0, 0}},                               // K: -.-
    {'L', {1, 0, 1, 1, 1, 0, 1, 0, 1, 0, 0, 0}},                               // L: .-..
    {'M', {1, 1, 1, 0, 1, 1, 1, 0, 0, 0}},                                     // M: --
    {'N', {1, 1, 1, 0, 1, 0, 0, 0}},                                           // N: -.
    {'O', {1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 0, 0}},                         // O: ---
    {'P', {1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0}},                         // P: .--.
    {'Q', {1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 0, 0}},                   // Q: --.-
    {'R', {1, 0, 1, 1, 1, 0, 1, 0, 0, 0}},                                     // R: .-.
    {'S', {1, 0, 1, 0, 1, 0, 0, 0}},                                           // S: ...
    {'T', {1, 1, 1, 0, 0, 0}},                                                 // T: -
    {'U', {1, 0, 1, 0, 1, 1, 1, 0, 0, 0}},                                     // U: ..-
    {'V', {1, 0, 1, 0, 1, 0, 1, 1, 1, 0, 0, 0}},                               // V: ...-
    {'W', {1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 0, 0}},                               // W: .--
    {'X', {1, 1, 1, 0, 1, 0, 1, 0, 1, 1, 1, 0, 0, 0}},                         // X: -..-
    {'Y', {1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 0, 0}},                   // Y: -.--
    {'Z', {1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1, 0, 0, 0}},                         // Z: --..
    {'1', {1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 0, 0}},       // 1: .----
    {'2', {1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 0, 0}},             // 2: ..---
    {'3', {1, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 0, 0}},                   // 3: ...--
    {'4', {1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 0, 0, 0}},                         // 4: ....-
    {'5', {1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0}},                               // 5: .....
    {'6', {1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0}},                         // 6: -....
    {'7', {1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0}},                   // 7: --...
    {'8', {1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0}},                   // 8: ---..
    {'9', {1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 0, 0}},             // 9: ----.
    {'0', {1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 0, 0}}, // 0: -----
    {'.', {1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 0, 0}},       // .: .-.-.-
    {',', {1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 0, 0}}, // ,: --..--
    {'?', {1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1, 0, 0, 0}},             // ?: ..--..
    {'!', {1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 0, 0}},             // !: -.-.--
    {'-', {1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 0, 0, 0}},       // -: -....-
    {'/', {1, 1, 1, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0}},                   // /: -..-.
    {'(', {1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0}},             // (: -.--.
    {')', {1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 0, 0}}, // ): -.--.-
    {' ', {0, 0, 0, 0, 0, 0, 0}},                                              // space
    {'*', {1, 1, 1, 1, 1, 1, 1}}                                               // unknown
};

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
        this->modeInternalName, "hexon", this->colorOn.c_str(), payload_e::COLOR, std::bind(&MorseMode::onColorOnCallback, this, std::placeholders::_1));
    this->pushColorOff = this->controllerService->subscribeModeTopic(
        this->modeInternalName, "hexoff", this->colorOff.c_str(), payload_e::COLOR, std::bind(&MorseMode::onColorOffCallback, this, std::placeholders::_1));
    this->pushBrightness = this->controllerService->subscribeModeTopic(
        this->modeInternalName, "brightness", this->brightness, boundaries_t{0, 255}, payload_e::BYTE, std::bind(&MorseMode::onBrightnessCallback, this, std::placeholders::_1));
    this->pushMessage = this->controllerService->subscribeModeTopic(
        this->modeInternalName, "message", this->message.c_str(), payload_e::STRING, std::bind(&MorseMode::onMessageCallback, this, std::placeholders::_1));
    this->pushTimeUnit = this->controllerService->subscribeModeTopic(
        this->modeInternalName, "timeunit", this->timeUnit, boundaries_t{1, 100}, payload_e::BYTE, std::bind(&MorseMode::onTimeUnitCallback, this, std::placeholders::_1));
    this->pushMorseActive = this->controllerService->subscribeModeTopic(
        this->modeInternalName, "active", this->morseActive ? "true" : "false", payload_e::BOOL, std::bind(&MorseMode::onMorseActiveCallback, this, std::placeholders::_1));
    this->pushInfinity = this->controllerService->subscribeModeTopic(
        this->modeInternalName, "infinity", this->infinity ? "true" : "false", payload_e::BOOL, std::bind(&MorseMode::onInfinityCallback, this, std::placeholders::_1));
}

void MorseMode::customLoop(unsigned long long steps) {
    if (this->isFirstRun()) {
        // set initial values
        this->newMorseActive = true;

        this->pushMorseActive(this->newMorseActive ? "true" : "false");
        this->pushMessage(this->newMessage);

        this->currentPositon = 0;
        this->currentMorseCodeUnit = 0;
        this->currentState = 0;

        this->controllerService->setHexColor(this->colorOff);
        this->controllerService->setBrightness(this->brightness);
        this->controllerService->confirmLedConfig();
    }

    if (this->isNewColorOn()) {
        this->colorOn = this->newColorOn;
    }

    if (this->isNewColorOff()) {
        this->colorOff = this->newColorOff;

        if (!this->morseActive) {
            this->controllerService->setHexColor(this->colorOff);
            this->controllerService->confirmLedConfig();
        }
    }

    if (this->isNewBrightness()) {
        this->brightness = this->newBrightness;

        this->controllerService->setBrightness(this->brightness);
        this->controllerService->confirmLedConfig();
    }

    if (this->isNewMessage()) {
        this->message = this->newMessage;

        if (!this->morseActive) {
            this->newMorseActive = true;

            this->pushMorseActive(this->newMorseActive ? "true" : "false");
        }
    }

    if (this->isNewTimeUnit()) {
        this->timeUnit = this->newTimeUnit;
    }

    if (this->isNewMorseActive()) {
        this->morseActive = this->newMorseActive;

        if (this->morseActive) {
            this->currentPositon = 0;
            this->currentMorseCodeUnit = 0;
            this->currentState = 0;
            this->newMessageArrived = false;

            this->controllerService->logMessage(LOG_LEVEL_DEBUG, LOG_MODE_ALL, "Started sending morse code message >>" + this->message + "<<.");
        } else {
            this->controllerService->setHexColor(this->colorOff);
            this->controllerService->confirmLedConfig();

            String morsedMessage = "";

            for (uint16_t i = 0; i < this->currentPositon; i++) {
                morsedMessage += this->message[i];
            }

            if (morsedMessage.length() == 0 && this->currentPositon == 0 && this->currentMorseCodeUnit == 0) {
                this->controllerService->logMessage(LOG_LEVEL_INFO, LOG_MODE_ALL, "finished sending morse code message >>" + this->message + "<<.");
            } else {
                this->controllerService->logMessage(LOG_LEVEL_INFO, LOG_MODE_ALL, "Aborted, only the part >>" + morsedMessage + "<< was sent.");
            }
        }
    }

    if (this->isNewInfinity()) {
        this->infinity = this->newInfinity;
    }

    if (this->morseActive && steps % this->timeUnit == 0) {
        char currentChar = this->message[this->currentPositon];

        // convert to uppercase
        if (currentChar >= 'a' && currentChar <= 'z') {
            currentChar -= 32;
        }

        // if the character is not in the map, use "nan" as default
        if (morseCodeMap.find(currentChar) == morseCodeMap.end()) {
            currentChar = '*';
        }

        std::vector<uint8_t> morseCode = morseCodeMap[currentChar];

        uint8_t newState = morseCode[this->currentMorseCodeUnit];

        if (newState != this->currentState) {
            this->currentState = newState;

            this->controllerService->setHexColor(newState == 1 ? this->colorOn : this->colorOff);

            this->controllerService->confirmLedConfig();
        }

        if (++this->currentMorseCodeUnit >= morseCode.size()) {
            this->currentMorseCodeUnit = 0;
            this->currentPositon++;

            this->controllerService->logMessage(LOG_LEVEL_DEBUG, LOG_MODE_ALL, "Morse code: " + String(currentChar) + " (" + String(this->currentPositon) + "/" + String(this->message.length()) + ")");
        }

        if (this->currentPositon >= this->message.length()) {
            this->currentPositon = 0;
            this->currentMorseCodeUnit = 0;

            if (!this->infinity) {
                this->newMorseActive = false;

                this->pushMorseActive(this->newMorseActive ? "true" : "false");
                this->pushMessage(this->message);

                this->controllerService->setHexColor(this->colorOff);

                this->controllerService->confirmLedConfig();
            }
        }
    }
}

void MorseMode::onColorOnCallback(String payload) {
    this->newColorOn = payload;

    if (this->newMorseActive) {
        this->controllerService->logMessage(LOG_LEVEL_WARN, LOG_MODE_ALL, "Morse code is active. Please stop the current message before changing the color.");
        return;
    }

    if (this->pushColorOn != nullptr) {
        this->pushColorOn(this->newColorOn);
    }
}

void MorseMode::onColorOffCallback(String payload) {
    this->newColorOff = payload;

    if (this->newMorseActive) {
        this->controllerService->logMessage(LOG_LEVEL_WARN, LOG_MODE_ALL, "Morse code is active. Please stop the current message before changing the color.");
        return;
    }
    
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

    if (this->morseActive && !this->infinity) {
        this->controllerService->logMessage(LOG_LEVEL_WARN, LOG_MODE_ALL, "Morse code is active. Please stop the current message before changing the message.");
        return;
    }

    if (this->infinity) {
        this->currentPositon = 0;
        this->currentMorseCodeUnit = 0;
    }

    this->newMessageArrived = true;

    if (this->pushMessage != nullptr) {
        this->pushMessage(this->newMessage);
    }
}

void MorseMode::onTimeUnitCallback(String payload) {
    this->newTimeUnit = payload.toInt();

    if (this->newMorseActive && !this->infinity) {
        this->controllerService->logMessage(LOG_LEVEL_WARN, LOG_MODE_ALL, "Morse code is active. Please stop the current message before changing the time unit.");
        return;
    }

    if (this->pushTimeUnit != nullptr) {
        this->pushTimeUnit(String(this->newTimeUnit));
    }
}

void MorseMode::onMorseActiveCallback(String payload) {
    this->newMorseActive = payload == "true";

    if (this->pushMorseActive != nullptr) {
        this->pushMorseActive(this->newMorseActive ? "true" : "false");
    }
}

void MorseMode::onInfinityCallback(String payload) {
    this->newInfinity = payload == "true";

    if (this->pushInfinity != nullptr) {
        this->pushInfinity(this->newInfinity ? "true" : "false");
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
    return this->message != this->newMessage || (this->newMessageArrived && !this->morseActive) || (this->infinity && this->newMessageArrived);
}

bool MorseMode::isNewTimeUnit() {
    return this->timeUnit != this->newTimeUnit && (this->infinity || !this->morseActive);
}

bool MorseMode::isNewMorseActive() {
    return this->morseActive != this->newMorseActive;
}

bool MorseMode::isNewInfinity() {
    return this->infinity != this->newInfinity;
}
