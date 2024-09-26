#ifndef RAINBOWMODE_H


#include <functional>

#include "AbstractMode.h"


class RainbowMode : public AbstractMode {
    private:
        // internal properties
        byte brightness = 255;
        byte newBrightness = 255;
        byte saturation = 255;
        byte newSaturation = 255;

        // internal methods
        void brightnessCallback(String payload);
        void saturationCallback(String payload);

        std::function<void(String payload)> pushBrightnessTopicFun = nullptr;
        std::function<void(String payload)> pushSaturationTopicFun = nullptr;

        bool isNewBrightness();
        bool isNewSaturation();

    public:
        RainbowMode(ControllerService* controllerService);

        void customSetup() override;
        void customLoop(int steps) override;
};

#endif