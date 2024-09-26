#include "AbstractApp.h"


AbstractApp::AbstractApp(ControllerService* controllerService)
{
    this->controllerService = controllerService;
}

AbstractApp::~AbstractApp()
{
}

void AbstractApp::setup()
{
    // call custom setup
    this->customSetup();
}

void AbstractApp::loop(const int steps)
{
    this->customLoop(steps);
}

// ------- MODE PROPERTIES -------
String AbstractApp::getModeTitle()
{
    return this->modeTitle;
}

String AbstractApp::getModeInternalName()
{
    return this->modeInternalName;
}

String AbstractApp::getModeDescription()
{
    return this->modeDescription;
}

String AbstractApp::getModeAuthor()
{
    return this->modeAuthor;
}

String AbstractApp::getModeContact()
{
    return this->modeContact;
}

String AbstractApp::getModeVersion()
{
    return this->modeVersion;
}

String AbstractApp::getModeLicense()
{
    return this->modeLicense;
}

// ------- CUSTOM METHODS -------