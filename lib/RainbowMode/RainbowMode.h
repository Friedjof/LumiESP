#ifndef RAINBOWMODE_H
#define RAINBOWMODE_H


#include <functional>

#include "AbstractMode.h"


class RainbowMode : public AbstractMode {
    private:
        // internal properties
        byte brightness = 255;
        byte newBrightness = this->brightness;
        byte saturation = 255;
        byte newSaturation = this->saturation;
        int position = 0;
        int newPosition = this->position;
        bool moving = true;
        bool newMoving = this->moving;
        byte speed = 0;
        byte newSpeed = this->speed;

        unsigned int currentStep = 0;

        // internal methods
        void brightnessCallback(String payload);
        void saturationCallback(String payload);
        void positionCallback(String payload);
        void movingCallback(String payload);
        void speedCallback(String payload);

        std::function<void(String payload)> pushBrightness = nullptr;
        std::function<void(String payload)> pushSaturation = nullptr;
        std::function<void(String payload)> pushPosition = nullptr;
        std::function<void(String payload)> pushMoving = nullptr;
        std::function<void(String payload)> pushSpeed = nullptr;

        bool isNewBrightness();
        bool isNewSaturation();
        bool isNewPosition();
        bool isNewMoving();
        bool isNewSpeed();

    public:
        RainbowMode(ControllerService* controllerService);

        void customSetup() override;
        void customLoop(unsigned long long steps) override;
};

#endif