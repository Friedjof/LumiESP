#ifndef STATICMODE_H
#define STATICMODE_H


#include <functional>

#include "AbstractMode.h"


class StaticMode : public AbstractMode {
    private:
        // internal properties
        String hexColor = "none";
        String newHexColor = "#000000";

        byte brightness = 255;
        byte newBrightness = 255;

        // internal methods
        void hexCallback(String payload);
        void brightnessCallback(String payload);

        std::function<void(String payload)> pushPubHexTopicFun;
        std::function<void(String payload)> pushPubBrightnessTopicFun;

        bool isNewHexColor();
        bool isNewBrightness();

    public:
        StaticMode(ControllerService* controllerService);

        void customSetup() override;
        void customLoop(unsigned long long steps) override;
};

#endif