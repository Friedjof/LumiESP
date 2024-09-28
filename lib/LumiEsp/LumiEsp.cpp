#include "LumiEsp.h"


LumiEsp::LumiEsp(ControllerService* controllerService) : AbstractApp(controllerService) {
    this->modeTitle = "Lumi ESP";
    this->modeDescription = "This app sends LumiESP status messages to MQTT";
    this->modeInternalName = "LumiEsp";
    this->modeAuthor = "Friedjof Noweck";
    this->modeContact = "programming@noweck.info";
    this->modeVersion = "0.1.0";
    this->modeLicense = "MIT";

    this->controllerService = controllerService;
}

void LumiEsp::customSetup() {
    this->pushLog = this->controllerService->subscribeModeTopic(this->modeInternalName, "log");
    this->pushLevel = this->controllerService->subscribeModeTopic(this->modeInternalName, "level");
    this->pushStatus = this->controllerService->subscribeModeTopic(this->modeInternalName, "status");
    this->pushDatetime = this->controllerService->subscribeModeTopic(this->modeInternalName, "datetime");

    this->pushMode = this->controllerService->subscribeModeTopic(this->modeInternalName, "mode", "", payload_e::STRING, topic_e::PUB_SUB, std::function<void(String)>(std::bind(&LumiEsp::modeCallback, this, std::placeholders::_1)));
}

void LumiEsp::customLoop(int steps) {
    // do nothing
}

void LumiEsp::logMessage(String message) {
    if (this->pushLog != nullptr) {
        this->pushLog(message);
    }
}

void LumiEsp::logMessage(const char* message) {
    if (this->pushLog != nullptr) {
        this->pushLog(message);
    }
}

void LumiEsp::logLevel(String level) {
    if (this->pushLevel != nullptr) {
        this->pushLevel(level);
    }
}

void LumiEsp::logStatus(String status) {
    if (this->pushStatus != nullptr) {
        this->pushStatus(status);
    }
}

void LumiEsp::logDatetime(String datetime) {
    if (this->pushDatetime != nullptr) {
        this->pushDatetime(datetime);
    }
}

void LumiEsp::modeCallback(String payload) {
    this->controllerService->setMode(payload);

    if (this->pushMode != nullptr) {
        this->pushMode(payload);
    }
}
