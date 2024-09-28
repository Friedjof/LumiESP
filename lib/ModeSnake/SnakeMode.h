#ifndef SnakeMode_H
#define SnakeMode_H


#include <functional>

#include "AbstractMode.h"


class SnakeMode : public AbstractMode {
    private:
        // internal properties
        String onHexColor = "#ffffff";
        String newOnHexColor = this->onHexColor;
        String offHexColor = "#000000";
        String newOffHexColor = this->offHexColor;
        byte brightness = 255;
        byte newBrightness = this->brightness;
        int snakeLength = 3;
        int newSnakeLength = this->snakeLength;
        bool direction = true;
        bool newDirection = this->direction;
        bool crawling = true;
        bool newCrawling = this->crawling;
        int position = 0;
        int newPosition = this->position;
        byte speed = 4;
        byte newSpeed = this->speed;

        unsigned int snakeHead = 0;

        // internal methods
        void onHexCallback(String payload);
        void offHexCallback(String payload);
        void brightnessCallback(String payload);
        void snakeLengthCallback(String payload);
        void directionCallback(String payload);
        void crawlingCallback(String payload);
        void positionCallback(String payload);
        void speedCallback(String payload);

        std::function<void(String payload)> pushOnHexTopic = nullptr;
        std::function<void(String payload)> pushOffHexTopic = nullptr;
        std::function<void(String payload)> pushBrightnessTopic = nullptr;
        std::function<void(String payload)> pushSnakeLengthTopic = nullptr;
        std::function<void(String payload)> pushDirectionTopic = nullptr;
        std::function<void(String payload)> pushCrawlingTopic = nullptr;
        std::function<void(String payload)> pushPositionTopic = nullptr;
        std::function<void(String payload)> pushSpeedTopic = nullptr;

        bool isNewBrightness();
        bool isNewOnHexColor();
        bool isNewOffHexColor();
        bool isNewSnakeLength();
        bool isNewDirection();
        bool isNewCrawling();
        bool isNewPosition();
        bool isNewSpeed();

    public:
        SnakeMode(ControllerService* controllerService);

        void customSetup() override;
        void customLoop(unsigned long long steps) override;
};

#endif