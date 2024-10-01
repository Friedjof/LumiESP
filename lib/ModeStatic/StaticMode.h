#ifndef STATICMODE_H
#define STATICMODE_H

#include <deque>
#include <vector>
#include <algorithm>
#include <functional>
#include "AbstractMode.h"


class StaticMode : public AbstractMode {
    private:
        // internal properties
        String hexColor = "#a300a3";
        String newHexColor = this->hexColor;
        uint8_t brightness = 255;
        uint8_t newBrightness = this->brightness;
        bool ultrasonicEnabled = true;
        bool newUltrasonicEnabled = this->ultrasonicEnabled;

        std::deque<float> distanceHistory = {};
        const size_t maxHistorySize = 31; // Adjust the size as needed

        float distance = 0.0;
        float newDistance = this->distance;

        // internal methods
        void hexCallback(String payload);
        void brightnessCallback(String payload);
        void ultrasonicEnabledCallback(String payload);

        std::function<void(String payload)> pushHex = nullptr;
        std::function<void(String payload)> pushBrightness = nullptr;
        std::function<void(String payload)> pushDistance = nullptr;
        std::function<void(String payload)> pushUltrasonicEnabled = nullptr;

        bool isNewHexColor();
        bool isNewBrightness();
        bool isNewDistance();
        bool isNewUltrasonicEnabled();

        void readAndSetDistance();
        float updateDistance(float newDistance);
    
    protected:
        void customSetup() override;
        void customLoop(unsigned long long steps) override;

    public:
        StaticMode(ControllerService* controllerService);
};

#endif