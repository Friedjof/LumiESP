#ifndef LOGGINGMQTT_H
#define LOGGINGMQTT_H


#include <functional>

#include "AbstractApp.h"


class LumiEsp : public AbstractApp {
    private:
        ControllerService* controllerService;

        String currentMode = "";

        // mqtt callbacks
        std::function<void(String payload)> pushLog = nullptr;
        std::function<void(String payload)> pushLevel = nullptr;
        std::function<void(String payload)> pushStatus = nullptr;
        std::function<void(String payload)> pushDatetime = nullptr;
        std::function<void(String payload)> pushMode = nullptr;

        void modeCallback(String payload);

        // logging methods
        void logMessage(String message);    
        void logMessage(const char* message);
        void logLevel(String level);
        void logStatus(String status);
        void logDatetime(String datetime);
    
    protected:
        void customSetup() override;
        void customLoop(int steps) override;

    public:
        LumiEsp(ControllerService* controllerService);
};

#endif