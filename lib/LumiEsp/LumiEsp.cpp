#include "LumiEsp.h"


LumiEsp::LumiEsp(ControllerService* controllerService) : AbstractApp(controllerService) {
    this->modeTitle = "Lumi ESP";
    this->modeDescription = "This app sends LumiESP status messages to MQTT";
    this->modeInternalName = "LumiEsp";
    this->modeAuthor = "Friedjof Noweck";
    this->modeContact = "git@noweck.info";
    this->modeVersion = "0.1.0";
    this->modeLicense = "MIT";

    this->controllerService = controllerService;
}

void LumiEsp::customSetup() {
    this->logCallback = this->controllerService->subscribeModeTopic(this->modeInternalName, "log");
    this->levelCallback = this->controllerService->subscribeModeTopic(this->modeInternalName, "level");
    this->statusCallback = this->controllerService->subscribeModeTopic(this->modeInternalName, "status");
    this->datetimeCallback = this->controllerService->subscribeModeTopic(this->modeInternalName, "datetime");

    this->modeSubFun = this->controllerService->subscribeModeTopic(this->modeInternalName, "mode", "", payload_e::STRING, topic_e::PUB_SUB, std::function<void(String)>(std::bind(&LumiEsp::modeCallback, this, std::placeholders::_1)));
}

void LumiEsp::customLoop(int steps) {
    // do nothing
}

void LumiEsp::logMessage(String message) {
    this->logCallback(message);
}

void LumiEsp::logMessage(const char* message) {
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

void LumiEsp::modeCallback(String payload) {
    this->controllerService->setMode(payload);

    if (this->modeSubFun) {
        this->modeSubFun(payload);
    }
}