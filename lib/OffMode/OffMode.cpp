#include "OffMode.h"


OffMode::OffMode(ControllerService* controllerService) : AbstractMode(controllerService) {
    this->modeTitle = "Off Mode";
    this->modeDescription = "This mode turns off the LEDs.";
    this->modeInternalName = "OffMode";
    this->modeAuthor = "Friedjof Noweck";
    this->modeContact = "programming@noweck.info";
    this->modeVersion = "0.1.0";
    this->modeLicense = "MIT";
}

void OffMode::customSetup() {
}

void OffMode::customLoop(unsigned long long steps) {
    if (this->isFirstRun()) {
        this->controllerService->setBrightness(this->brightness);
        this->controllerService->setHexColor(this->hexColor);

        this->controllerService->confirmLedConfig();
    }
}
