#ifndef UNICORNMODE_H
#define UNICORNMODE_H


#include <functional>

#include "AbstractMode.h"


#define MIN_RANDOM 0
#define MAX_RANDOM 100


class UnicornMode : public AbstractMode {
    private:
        // internal properties
        String hexColorOn = "#a300a3";
        String newHexColorOn = this->hexColorOn;
        String hexColorOff = "#000000";
        String newHexColorOff = this->hexColorOff;
        byte brightness = 0;
        byte newBrightness = this->brightness;
        unsigned int intensity = 0;
        unsigned int newIntensity = this->intensity;
        unsigned int speed = 5;
        unsigned int newSpeed = this->speed;
        bool crossfade = true;
        bool newCrossfade = this->crossfade;

        // internal methods
        void hexOnCallback(String payload);
        void hexOffCallback(String payload);
        void brightnessCallback(String payload);
        void intensityCallback(String payload);
        void speedCallback(String payload);
        void crossfadeCallback(String payload);

        std::function<void(String payload)> pushHexOnTopic;
        std::function<void(String payload)> pushHexOffTopic;
        std::function<void(String payload)> pushBrightnessTopic;
        std::function<void(String payload)> pushIntensityTopic;
        std::function<void(String payload)> pushSpeedTopic;
        std::function<void(String payload)> pushCrossfadeTopic;

        bool isNewHexOn();
        bool isNewHexOff();
        bool isNewBrightness();
        bool isNewIntensity();
        bool isNewSpeed();
        bool isNewCrossfade();

    public:
        UnicornMode(ControllerService* controllerService);

        void customSetup() override;
        void customLoop(unsigned long long steps) override;
};

#endif