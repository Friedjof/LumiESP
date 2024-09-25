#ifndef LOGGINGMQTT_H
#define LOGGINGMQTT_H


#include <functional>

#include "ControllerService.h"

#include "AbstractApp.h"


class LumiEsp : public AbstractApp {
    private:
        ControllerService* controllerService;

        // mqtt callbacks
        std::function<void(String payload)> logCallback;
        std::function<void(String payload)> levelCallback;
        std::function<void(String payload)> statusCallback;
        std::function<void(String payload)> datetimeCallback;

        std::function<void(String payload)> modeCallback;
    public:
        LumiEsp(MqttService* mqttService, ControllerService* controllerService);

        void customSetup() override;
        void customLoop(int steps) override;

        void logMessage(String message);
        void logLevel(String level);
        void logStatus(String status);
        void logDatetime(String datetime);
};

#endif