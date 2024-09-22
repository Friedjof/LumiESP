#include <Arduino.h>

#include <PubSubClient.h>
#include <TaskScheduler.h>

#include "MqttService.h"
#include "ClockService.h"
#include "LoggingService.h"
#include "TaskService.h"

#include "../config/config.h"


WiFiClient wifiClient;

MqttService mqttService;
ClockService clockService;
LoggingService loggingService(&mqttService, &clockService);
//TaskService taskService(mqttService, clockService, loggingService);


long long t = millis();


void setup() {
    mqttService.setup();
    clockService.setup();
    loggingService.setup();

    loggingService.logMessage(LOG_LEVEL_INFO, LOG_MODE_SERIAL, "Setup completed");
}

void loop() {
    if (millis() - t > 10000) {
        t = millis();
        loggingService.logMessage(LOG_LEVEL_INFO, LOG_MODE_ALL, "MQTT and Serial logging test");
    }
}
