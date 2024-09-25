#ifndef LOGGINGMQTT_H
#define LOGGINGMQTT_H


#include <functional>

#include "AbstractApp.h"


class StatusApp : public AbstractApp {
    private:
        // mqtt callbacks
        std::function<void(String payload)> logCallback;
        std::function<void(String payload)> levelCallback;
        std::function<void(String payload)> statusCallback;
        std::function<void(String payload)> datetimeCallback;

        std::function<void(String payload)> modeCallback;
    public:
        StatusApp(MqttService* mqttService);

        void customSetup() override;
        void customLoop(int steps) override;

        void logMessage(String message);
        void logLevel(String level);
        void logStatus(String status);
        void logDatetime(String datetime);
};

#endif