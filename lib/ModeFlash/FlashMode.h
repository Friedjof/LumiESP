#ifndef FLASHMODE_H
#define FLASHMODE_H

#define DEFUALT_BRIGHTNESS 255


#include <functional>

#include "AbstractMode.h"


class FlashMode : public AbstractMode {
    private:
        // internal states
        String startColorHex = "#000000";
        String newStartColorHex = this->startColorHex;
        String stopColorHex = "#7f7f7f";
        String newStopColorHex = this->stopColorHex;
        String nextMode = "OffMode";
        String newNextMode = this->nextMode;
        byte brightness = 255;
        byte newBrightness = this->brightness;
        short maxFlashNum = 3;
        short newMaxFlashNum = this->maxFlashNum;
        short maxCrossfadeSteps = 20;
        short newMaxCrossfadeSteps = this->maxCrossfadeSteps;
        bool infinityFlash = false;
        bool newInfinityFlash = this->infinityFlash;

        // internal properties
        CRGB startColor;
        CRGB endColor;

        // internal counters
        unsigned long long blinkCounter = 0;
        unsigned long long stepCounter = 0;

        // internal methods
        void startColorCallback(String payload);
        void stopColorCallback(String payload);
        void brightnessCallback(String payload);
        void maxFlashNumCallback(String payload);
        void maxCrossfadeStepsCallback(String payload);
        void infinityFlashCallback(String payload);
        void nextModeCallback(String payload);

        std::function<void(String payload)> pushStartColor;
        std::function<void(String payload)> pushStopColor;
        std::function<void(String payload)> pushBrightness;
        std::function<void(String payload)> pushMaxFlashNum;
        std::function<void(String payload)> pushMaxCrossfadeSteps;
        std::function<void(String payload)> pushInfinityFlash;
        std::function<void(String payload)> pushNextMode;

        bool isNewStartColor();
        bool isNewStopColor();
        bool isNewBrightness();
        bool isNewMaxFlashNum();
        bool isNewMaxCrossfadeSteps();
        bool isNewInfinityFlash();
        bool isNewNextMode();

        // helper methods
        CRGB interpolateColor(CRGB start, CRGB end, float fraction);
        CRGB hexColor2CRGB(String hexColor);
        String expandHexColor(String hexColor);

    protected:
        void customSetup() override;
        void customLoop(unsigned long long steps) override;

    public:
        FlashMode(ControllerService* controllerService);
};

#endif