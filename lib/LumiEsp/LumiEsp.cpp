#include "LumiEsp.h"


LumiEsp::LumiEsp(MqttService* mqttService, ControllerService* controllerService) : AbstractApp(mqttService) {
    this->modeTitle = "Status App";
    this->modeDescription = "This app sends LumiESP status messages to MQTT";
    this->modeInternalName = "Status";
    this->modeAuthor = "Friedjof Noweck";
    this->modeContact = "git@noweck.info";
    this->modeVersion = "0.1.0";
    this->modeLicense = "MIT";

    this->controllerService = controllerService;
}

void LumiEsp::customSetup() {
    this->logCallback = this->mqttService->subscribeModeTopic(this->modeInternalName, "log");
    this->levelCallback = this->mqttService->subscribeModeTopic(this->modeInternalName, "level");
    this->statusCallback = this->mqttService->subscribeModeTopic(this->modeInternalName, "status");
    this->datetimeCallback = this->mqttService->subscribeModeTopic(this->modeInternalName, "datetime");

    this->modeCallback = this->mqttService->subscribeModeTopic(this->modeInternalName, "mode", "", payload_e::STRING, topic_e::PUB_SUB, [this](String payload) {
        this->controllerService->setMode(payload);
    });
}

void LumiEsp::customLoop(int steps) {
    // do nothing
}

void LumiEsp::logMessage(String message) {
    this->logCallback(message);
}

void LumiEsp::logLevel(String level) {
    this->levelCallback(level);
}

void LumiEsp::logStatus(String status) {
    this->statusCallback(status);
}

void LumiEsp::logDatetime(String datetime) {
    this->datetimeCallback(datetime);
}
