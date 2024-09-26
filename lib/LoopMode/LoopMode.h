#ifndef LOOPMODE_H
#define LOOPMODE_H


#include <functional>

#include "AbstractMode.h"


class LoopMode : public AbstractMode {
    private:
        // internal properties
        String onHexColor = "#ffffff";
        String newOnHexColor = "#ffffff";
        String offHexColor = "#000000";
        String newOffHexColor = "#000000";

        byte brightness = 255;
        byte newBrightness = 255;

        // internal methods
        void onHexCallback(String payload);
        void offHexCallback(String payload);
        void brightnessCallback(String payload);

        std::function<void(String payload)> pushPubOnHexTopicFun;
        std::function<void(String payload)> pushPubOffHexTopicFun;
        std::function<void(String payload)> pushPubBrightnessTopicFun;

        bool isNewBrightness();

    public:
        LoopMode(ControllerService* controllerService);

        void customSetup() override;
        void customLoop(int steps) override;
};

#endif