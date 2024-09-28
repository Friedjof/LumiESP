#include "AbstractMode.h"


AbstractMode::AbstractMode(ControllerService* controllerService)
{
    this->controllerService = controllerService;
}

AbstractMode::~AbstractMode()
{
}

void AbstractMode::setup()
{
    this->controllerService->registerMode(this->modeInternalName, [this](int steps) {
        this->loop(steps);
    });
    
    // call custom setup
    this->customSetup();

    this->controllerService->logMessage(LOG_LEVEL_INFO, LOG_MODE_ALL, "Mode setup completed for: " + this->modeInternalName);
}

void AbstractMode::loop(unsigned long long steps)
{
    this->firstRun = steps > this->lastSteps + 1 || (steps == 0 && this->lastSteps == 0);

    this->customLoop(steps);

    this->lastSteps = steps;
}

// ------- MODE PROPERTIES -------
String AbstractMode::getModeTitle()
{
    return this->modeTitle;
}

String AbstractMode::getModeInternalName()
{
    return this->modeInternalName;
}

String AbstractMode::getModeDescription()
{
    return this->modeDescription;
}

String AbstractMode::getModeAuthor()
{
    return this->modeAuthor;
}

String AbstractMode::getModeContact()
{
    return this->modeContact;
}

String AbstractMode::getModeVersion()
{
    return this->modeVersion;
}

String AbstractMode::getModeLicense()
{
    return this->modeLicense;
}

// ------- PRIVATE METHODS -------
bool AbstractMode::isFirstRun()
{
    return this->firstRun;
}
