#ifndef STATICMODE_H
#define STATICMODE_H


#include <functional>

#include "AbstractMode.h"


class StaticMode : public AbstractMode {
    private:
        // internal properties
        String hexColor = "#a300a3";
        String newHexColor = "#a300a3";

        byte brightness = 0;
        byte newBrightness = 0;

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