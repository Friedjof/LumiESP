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
    // call custom setup
    this->customSetup();

    this->controllerService->logMessage(LOG_LEVEL_INFO, LOG_MODE_ALL, "Mode setup completed for: " + this->modeInternalName);
}

void AbstractMode::loop(const int steps)
{
    this->customLoop(steps);
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

// ------- CUSTOM METHODS -------