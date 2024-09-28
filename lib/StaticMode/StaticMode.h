#ifndef STATICMODE_H
#define STATICMODE_H


#include <functional>

#include "AbstractMode.h"


class StaticMode : public AbstractMode {
    private:
        // internal properties
        String hexColor = "#a300a3";
        String newHexColor = this->hexColor;

        byte brightness = 255;
        byte newBrightness = this->brightness;

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