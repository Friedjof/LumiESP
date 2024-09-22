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
TaskService taskService(&mqttService, &clockService, &loggingService);

// global scheduler
Scheduler scheduler;


// task wrappers
void mqttStatusUpdateWrapper() {
    taskService.mqttStatusUpdate();
}

void mqttServiceLoopWrapper() {
    // this is necessary to keep the mqtt connection alive
    mqttService.loop();
}

void mqttServiceUpdateDateTimeWrapper() {
    char datetime[128];
    clockService.getDateTime(datetime);

    mqttService.mqttDatetimeUpdate(datetime);
}

void timeSyncWrapper() {
    clockService.syncTime();
}


// tasks
Task mqttStatusUpdateTask(MQTT_DATETIME_UPDATE_STATUS_INTERVAL, TASK_FOREVER, &mqttStatusUpdateWrapper);
Task mqttServiceLoopTask(5, TASK_FOREVER, &mqttServiceLoopWrapper);
Task mqttServiceUpdateDateTimeTask(MQTT_DATETIME_UPDATE_INTERVAL, TASK_FOREVER, &mqttServiceUpdateDateTimeWrapper);
Task timeSyncTask(TIME_SYNC_INTERVAL, TASK_FOREVER, &timeSyncWrapper);


void setup() {
    // setup services
    mqttService.setup();
    clockService.setup();
    loggingService.setup();
    taskService.setup();
    loggingService.logMessage(LOG_LEVEL_DEBUG, LOG_MODE_SERIAL, "Services setup completed");

    // add tasks to scheduler
    scheduler.addTask(mqttStatusUpdateTask);
    scheduler.addTask(mqttServiceLoopTask);
    scheduler.addTask(mqttServiceUpdateDateTimeTask);
    scheduler.addTask(timeSyncTask);
    loggingService.logMessage(LOG_LEVEL_DEBUG, LOG_MODE_SERIAL, "Tasks added to scheduler");

    // enable all tasks
    scheduler.enableAll();
    loggingService.logMessage(LOG_LEVEL_DEBUG, LOG_MODE_SERIAL, "Tasks enabled");

    loggingService.logMessage(LOG_LEVEL_INFO, LOG_MODE_ALL, "Setup completed");
}

void loop() {
    scheduler.execute();
    vTaskDelay(1);
}
