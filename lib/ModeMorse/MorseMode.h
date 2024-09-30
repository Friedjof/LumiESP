#ifndef MORSEMODE_H
#define MORSEMODE_H


#include <functional>
#include "AbstractMode.h"


#include <iostream>
#include <map>
#include <vector>


class MorseMode : public AbstractMode {
private:
    String colorOn = "#FFFFFF";
    String newColorOn = this->colorOn;
    String colorOff = "#000000";
    String newColorOff = this->colorOff;
    byte brightness = 255;
    byte newBrightness = 255;
    uint8_t timeUnit = 10;
    uint8_t newTimeUnit = this->timeUnit;
    bool morseActive = false;
    bool newMorseActive = this->morseActive;
    String message = "Hello World!";
    String newMessage = this->message;
    bool infinity = false;
    bool newInfinity = this->infinity;

    // counter ans state variables
    unsigned int currentPositon = 0;
    uint8_t currentMorseCodeUnit = 0;
    uint8_t currentState = 0;
    bool newMessageArrived = false;

    // Callback methods if a value has changed
    void onColorOnCallback(String payload);
    void onColorOffCallback(String payload);
    void onBrightnessCallback(String payload);
    void onMessageCallback(String payload);
    void onTimeUnitCallback(String payload);
    void onMorseActiveCallback(String payload);
    void onInfinityCallback(String payload);

    // Functions to publish updated values back to MQTT to confirm changes
    std::function<void(String payload)> pushColorOn = nullptr;
    std::function<void(String payload)> pushColorOff = nullptr;
    std::function<void(String payload)> pushBrightness = nullptr;
    std::function<void(String payload)> pushMessage = nullptr;
    std::function<void(String payload)> pushTimeUnit = nullptr;
    std::function<void(String payload)> pushMorseActive = nullptr;
    std::function<void(String payload)> pushInfinity = nullptr;

    // Helper methods to detect changes
    bool isNewColorOn();
    bool isNewColorOff();
    bool isNewBrightness();
    bool isNewMessage();
    bool isNewTimeUnit();
    bool isNewMorseActive();
    bool isNewInfinity();

public:
    MorseMode(ControllerService* controllerService);

    // Setup method called once when the mode is initialized
    void customSetup() override;

    // Loop method called in every iteration; 'steps' is a counter for every mode in this project
    void customLoop(unsigned long long steps) override;
};

#endif