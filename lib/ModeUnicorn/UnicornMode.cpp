#include "UnicornMode.h"


UnicornMode::UnicornMode(ControllerService* controllerService) : AbstractMode(controllerService) {
    this->modeTitle = "Unicorn Mode";
    this->modeDescription = "This mode sparkles like a unicorn.";
    this->modeInternalName = "UnicornMode";
    this->modeAuthor = "Friedjof Noweck";
    this->modeContact = "programming@noweck.info";
    this->modeVersion = "0.1.0";
    this->modeLicense = "MIT";
}

void UnicornMode::customSetup() {
}

void UnicornMode::customLoop(unsigned long long steps) {
    if (this->isFirstRun()) {
        this->controllerService->setBrightness(this->brightness);
        this->controllerService->setHexColor(this->hexColor);

        this->controllerService->confirmLedConfig();
    }
}
