#ifndef STATICMODE_H
#define STATICMODE_H


#include <functional>

#include "AbstractMode.h"


class StaticMode : public AbstractMode {
    private:
        void hexCallback(String payload);
        void brightnessCallback(String payload);

        std::function<void(String payload)> pushPubHexTopicFun;
        std::function<void(String payload)> pushPubBrightnessTopicFun;

    public:
        StaticMode(LedService* ledService, LoggingService* loggingService, MqttService* mqttService);

        void customSetup() override;
        void customLoop(int steps) override;
};

#endif