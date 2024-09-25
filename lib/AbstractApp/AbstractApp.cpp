#include "AbstractApp.h"


AbstractApp::AbstractApp(MqttService* mqttService)
{
    this->mqttService = mqttService;
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
