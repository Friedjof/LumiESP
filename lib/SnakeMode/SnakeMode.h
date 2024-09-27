#ifndef SnakeMode_H
#define SnakeMode_H


#include <functional>

#include "AbstractMode.h"


class SnakeMode : public AbstractMode {
    private:
        // internal properties
        String onHexColor = "#ffffff";
        String newOnHexColor = "#ffffff";
        String offHexColor = "#000000";
        String newOffHexColor = "#000000";
        byte brightness = 255;
        byte newBrightness = 255;
        int snakeLength = 3;
        int newSnakeLength = 3;
        bool direction = true;
        bool newDirection = true;
        bool crawling = true;
        bool newCrawling = true;
        int position = 0;
        int newPosition = 0;

        unsigned int snakeHead = 0;

        // internal methods
        void onHexCallback(String payload);
        void offHexCallback(String payload);
        void brightnessCallback(String payload);
        void snakeLengthCallback(String payload);
        void directionCallback(String payload);
        void crawlingCallback(String payload);
        void positionCallback(String payload);

        std::function<void(String payload)> pushOnHexTopic = nullptr;
        std::function<void(String payload)> pushOffHexTopic = nullptr;
        std::function<void(String payload)> pushBrightnessTopic = nullptr;
        std::function<void(String payload)> pushSnakeLengthTopic = nullptr;
        std::function<void(String payload)> pushDirectionTopic = nullptr;
        std::function<void(String payload)> pushCrawlingTopic = nullptr;
        std::function<void(String payload)> pushPositionTopic = nullptr;

        bool isNewBrightness();
        bool isNewOnHexColor();
        bool isNewOffHexColor();
        bool isNewSnakeLength();
        bool isNewDirection();
        bool isNewCrawling();
        bool isNewPosition();

    public:
        SnakeMode(ControllerService* controllerService);

        void customSetup() override;
        void customLoop(unsigned long long steps) override;
};

#endif