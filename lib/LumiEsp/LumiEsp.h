#ifndef LOGGINGMQTT_H
#define LOGGINGMQTT_H


#include <functional>

#include "AbstractApp.h"


class LumiEsp : public AbstractApp {
    private:
        ControllerService* controllerService;

        // mqtt callbacks
        std::function<void(String payload)> pushLog = nullptr;
        std::function<void(String payload)> pushLevel = nullptr;
        std::function<void(String payload)> pushStatus = nullptr;
        std::function<void(String payload)> pushDatetime = nullptr;
        std::function<void(String payload)> pushMode = nullptr;

    public:
        LumiEsp(ControllerService* controllerService);

        void customSetup() override;
        void customLoop(int steps) override;

        void logMessage(String message);    
        void logMessage(const char* message);
        void logLevel(String level);
        void logStatus(String status);
        void logDatetime(String datetime);

        void modeCallback(String payload);
};

#endif