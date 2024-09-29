#ifndef MORSEMODE_H
#define MORSEMODE_H


#include <functional>
#include "AbstractMode.h"


class MorseMode : public AbstractMode {
private:
    String colorOn = "#FFFFFF";
    String newColorOn = this->colorOn;
    String colorOff = "#000000";
    String newColorOff = this->colorOff;
    byte brightness = 255;
    byte newBrightness = 255;
    String message = "This is the default message.";
    String newMessage = this->message;

    // Callback methods if a value has changed
    void onColorOnCallback(String payload);
    void onColorOffCallback(String payload);
    void onBrightnessCallback(String payload);
    void onMessageCallback(String payload);

    // Functions to publish updated values back to MQTT to confirm changes
    std::function<void(String payload)> pushColorOn = nullptr;
    std::function<void(String payload)> pushColorOff = nullptr;
    std::function<void(String payload)> pushBrightness = nullptr;
    std::function<void(String payload)> pushMessage = nullptr;

    // Helper methods to detect changes
    bool isNewColorOn();
    bool isNewColorOff();
    bool isNewBrightness();
    bool isNewMessage();

public:
    MorseMode(ControllerService* controllerService);

    // Setup method called once when the mode is initialized
    void customSetup() override;

    // Loop method called in every iteration; 'steps' is a counter for every mode in this project
    void customLoop(unsigned long long steps) override;
};

#endif