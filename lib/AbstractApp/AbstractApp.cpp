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
