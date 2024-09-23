#include <Arduino.h>

#include <TaskScheduler.h>

// services
#include "MqttService.h"
#include "ClockService.h"
#include "LoggingService.h"
#include "LedService.h"
#include "TaskService.h"

// config
#include "../config/config.h"


// task wrappers prototypes
void mqttServiceStatusUpdateWrapper();
void mqttServiceLoopWrapper();
void mqttServiceUpdateDateTimeWrapper();
void clockServiceTimeSyncWrapper();
void ledServiceLoopWrapper();
void mqttServiceCallbackWrapper(const espMqttClientTypes::MessageProperties& properties, const char* topic, const uint8_t* payload, size_t len, size_t index, size_t total);


// global objects
WiFiClient wifiClient;
Scheduler scheduler;

// global services
MqttService mqttService;
ClockService clockService;
LoggingService loggingService(&mqttService, &clockService);
LedService ledService(&loggingService, &mqttService);
TaskService taskService(&mqttService, &clockService, &loggingService, &ledService);


// tasks
Task mqttStatusUpdateTask(MQTT_DATETIME_UPDATE_STATUS_INTERVAL, TASK_FOREVER, &mqttServiceStatusUpdateWrapper);
Task mqttServiceUpdateDateTimeTask(MQTT_DATETIME_UPDATE_INTERVAL, TASK_FOREVER, &mqttServiceUpdateDateTimeWrapper);
Task timeSyncTask(TIME_SYNC_INTERVAL, TASK_FOREVER, &clockServiceTimeSyncWrapper);
Task mqttServiceLoopTask(2, TASK_FOREVER, &mqttServiceLoopWrapper);
Task ledServiceLoopTask(LED_MODE_CONFIG_SPEED, TASK_FOREVER, &ledServiceLoopWrapper);


void setup() {
    // setup services
    mqttService.setup();
    clockService.setup();
    loggingService.setup();
    ledService.setup();
    taskService.setup();

    loggingService.logMessage(LOG_LEVEL_DEBUG, LOG_MODE_SERIAL, "Services setup completed");

    // create mqtt topics
    mqttService.createTopics();

    loggingService.logMessage(LOG_LEVEL_DEBUG, LOG_MODE_SERIAL, "MQTT topics created");

    // set mqtt callback
    mqttService.setCallback(&mqttServiceCallbackWrapper);

    loggingService.logMessage(LOG_LEVEL_DEBUG, LOG_MODE_SERIAL, "MQTT callback set");

    // subscribe to mqtt topics
    mqttService.subscribe();

    loggingService.logMessage(LOG_LEVEL_DEBUG, LOG_MODE_SERIAL, "MQTT subscribed");

    // add tasks to scheduler
    scheduler.addTask(mqttStatusUpdateTask);
    scheduler.addTask(mqttServiceUpdateDateTimeTask);
    scheduler.addTask(timeSyncTask);
    scheduler.addTask(mqttServiceLoopTask);
    scheduler.addTask(ledServiceLoopTask);

    loggingService.logMessage(LOG_LEVEL_DEBUG, LOG_MODE_SERIAL, "Tasks added to scheduler");

    // enable all tasks
    scheduler.enableAll();

    loggingService.logMessage(LOG_LEVEL_DEBUG, LOG_MODE_SERIAL, "Tasks enabled");

    // --- setup completed ---
    loggingService.logMessage(LOG_LEVEL_INFO, LOG_MODE_ALL, "Setup completed");
}

void loop() {
    scheduler.execute();
    vTaskDelay(1);
}


// task wrappers
void mqttServiceStatusUpdateWrapper() {
    taskService.mqttServiceStatusUpdateWrapper();
}

void mqttServiceLoopWrapper() {
    taskService.mqttServiceLoopWrapper();
}

void mqttServiceUpdateDateTimeWrapper() {
    taskService.mqttServiceUpdateDateTimeWrapper();
}

void clockServiceTimeSyncWrapper() {
    taskService.clockServiceTimeSyncWrapper();
}

void ledServiceLoopWrapper() {
    taskService.ledServiceLoopWrapper();
}

void mqttServiceCallbackWrapper(const espMqttClientTypes::MessageProperties& properties, const char* topic, const uint8_t* payload, size_t len, size_t index, size_t total) {
    taskService.mqttServiceCallbackWrapper(properties, topic, payload, len, index, total);
}
