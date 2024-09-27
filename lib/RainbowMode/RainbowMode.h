#ifndef RAINBOWMODE_H
#define RAINBOWMODE_H


#include <functional>

#include "AbstractMode.h"


class RainbowMode : public AbstractMode {
    private:
        // internal properties
        byte brightness = 255;
        byte newBrightness = 255;
        byte saturation = 255;
        byte newSaturation = 255;
        int position = 0;
        int newPosition = 0;

        // internal methods
        void brightnessCallback(String payload);
        void saturationCallback(String payload);
        void positionCallback(String payload);

        std::function<void(String payload)> pushBrightnessTopicFun = nullptr;
        std::function<void(String payload)> pushSaturationTopicFun = nullptr;
        std::function<void(String payload)> pushPositionTopicFun = nullptr;

        bool isNewBrightness();
        bool isNewSaturation();
        bool isNewPosition();

    public:
        RainbowMode(ControllerService* controllerService);

        void customSetup() override;
        void customLoop(unsigned long long steps) override;
};

#endif