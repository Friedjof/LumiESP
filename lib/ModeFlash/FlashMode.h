#ifndef FLASHMODE_H
#define FLASHMODE_H


#include <functional>

#include "AbstractMode.h"


class FlashMode : public AbstractMode {
    private:
        // internal states
        String startColorHex = "#000000";
        String newStartColorHex = this->startColorHex;
        String stopColorHex = "#ffffff";
        String newStopColorHex = this->stopColorHex;
        byte brightness = 255;
        byte newBrightness = this->brightness;
        short maxFlashNum = 3;
        short newMaxFlashNum = this->maxFlashNum;
        short maxCrossfadeSteps = 20;
        short newMaxCrossfadeSteps = this->maxCrossfadeSteps;
        bool infiniteFlash = false;
        bool newInfiniteFlash = this->infiniteFlash;

        // internal properties
        String nextMode;

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
        void infiniteFlashCallback(String payload);

        std::function<void(String payload)> pushStartColor;
        std::function<void(String payload)> pushStopColor;
        std::function<void(String payload)> pushBrightness;
        std::function<void(String payload)> pushMaxFlashNum;
        std::function<void(String payload)> pushMaxCrossfadeSteps;
        std::function<void(String payload)> pushInfiniteFlash;

        bool isNewStartColor();
        bool isNewStopColor();
        bool isNewBrightness();
        bool isNewMaxFlashNum();
        bool isNewMaxCrossfadeSteps();
        bool isNewInfiniteFlash();

        // helper methods
        CRGB interpolateColor(CRGB start, CRGB end, float fraction);
        CRGB hexColor2CRGB(String hexColor);
        String expandHexColor(String hexColor);

    public:
        FlashMode(ControllerService* controllerService);

        void customSetup() override;
        void customLoop(unsigned long long steps) override;

        void setNextMode(String mode);
};

#endif