#include "StatusApp.h"


StatusApp::StatusApp(MqttService* mqttService) : AbstractApp(mqttService) {
    this->modeTitle = "Status App";
    this->modeDescription = "This app sends LumiESP status messages to MQTT";
    this->modeInternalName = "Status";
    this->modeAuthor = "Friedjof Noweck";
    this->modeContact = "git@noweck.info";
    this->modeVersion = "0.1.0";
    this->modeLicense = "MIT";
}

void StatusApp::customSetup() {
    this->logCallback = this->mqttService->subscribeModeTopic(this->modeInternalName, "log");
    this->levelCallback = this->mqttService->subscribeModeTopic(this->modeInternalName, "level");
    this->statusCallback = this->mqttService->subscribeModeTopic(this->modeInternalName, "status");
    this->datetimeCallback = this->mqttService->subscribeModeTopic(this->modeInternalName, "datetime");

    //this->modeCallback = this->mqttService->subscribeModeTopic(this->modeInternalName, "mode", payload_e::STRING, topic_e::PUB_SUB, std::function<void(String)>(std::bind(&StatusApp::modeCallback, this, std::placeholders::_1)));
}

void StatusApp::customLoop(int steps) {
    // do nothing
}

void StatusApp::logMessage(String message) {
    this->logCallback(message);
}

void StatusApp::logLevel(String level) {
    this->levelCallback(level);
}

void StatusApp::logStatus(String status) {
    this->statusCallback(status);
}

void StatusApp::logDatetime(String datetime) {
    this->datetimeCallback(datetime);
}
