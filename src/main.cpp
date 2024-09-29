#include <Arduino.h>
#include <functional>
#include <Wire.h>

#include <TaskScheduler.h>

// services
#include "MqttService.h"
#include "ClockService.h"
#include "LoggingService.h"
#include "LedService.h"
#include "ControllerService.h"

// modes
#include "AbstractMode.h"

// custom modes
#include "FlashMode.h"
#include "OffMode.h"
#include "StaticMode.h"
#include "SnakeMode.h"
#include "RainbowMode.h"
#include "UnicornMode.h"

// apps
#include "AbstractApp.h"

// custom apps
#include "LumiEsp.h"

// config
#include "../config/config.h"


void connectToWiFi();

// task wrappers prototypes
void mqttServiceStatusUpdateWrapper();
void mqttServiceLoopWrapper();
void mqttServiceUpdateDateTimeWrapper();
void clockServiceTimeSyncWrapper();
void ledServiceLoopWrapper();
void mqttServiceCallbackWrapper(const espMqttClientTypes::MessageProperties& properties, const char* topic, const uint8_t* payload, size_t len, size_t index, size_t total);


// initialize scheduler
Scheduler scheduler;

// global services
MqttService mqttService;
ClockService clockService;
LoggingService loggingService;
LedService ledService;
ControllerService controllerService(&mqttService, &clockService, &loggingService, &ledService);


// scheduler tasks
Task mqttStatusUpdateTask(MQTT_DATETIME_UPDATE_STATUS_INTERVAL, TASK_FOREVER, &mqttServiceStatusUpdateWrapper);
Task mqttServiceUpdateDateTimeTask(MQTT_DATETIME_UPDATE_INTERVAL, TASK_FOREVER, &mqttServiceUpdateDateTimeWrapper);
Task timeSyncTask(TIME_SYNC_INTERVAL, TASK_FOREVER, &clockServiceTimeSyncWrapper);
Task mqttServiceLoopTask(2, TASK_FOREVER, &mqttServiceLoopWrapper);
Task ledServiceLoopTask(LED_MODE_CONFIG_SPEED, TASK_FOREVER, &ledServiceLoopWrapper);


void setup() {
    Serial.begin(LOG_SERIAL_SPEED);

    connectToWiFi();

    // ----> SETUP SERVICES <----
    mqttService.setup();
    clockService.setup();
    loggingService.setup();
    ledService.setup();
    controllerService.setup();

    loggingService.logMessage(LOG_LEVEL_DEBUG, LOG_MODE_SERIAL, "Services setup completed");



    // ----> SETUP YOUR MODE HERE <----
    FlashMode* flashMode = new FlashMode(&controllerService);

    AbstractMode* offMode = new OffMode(&controllerService);
    AbstractMode* staticMode = new StaticMode(&controllerService);
    AbstractMode* snakeMode = new SnakeMode(&controllerService);
    AbstractMode* rainbowMode = new RainbowMode(&controllerService);
    AbstractMode* unicornMode = new UnicornMode(&controllerService);

    // setup modes
    flashMode->setup();

    offMode->setup();
    staticMode->setup();
    snakeMode->setup();
    rainbowMode->setup();
    unicornMode->setup();

    loggingService.logMessage(LOG_LEVEL_DEBUG, LOG_MODE_SERIAL, "Modes setup completed");
    // <---- SETUP YOUR MODE HERE ---->



    // register functions for updating status messages over mqtt
    LumiEsp *statusApp = new LumiEsp(&controllerService);
    statusApp->setup();

    // register callback functions
    controllerService.registerPushLog([statusApp](const char* message) -> void {
        statusApp->logMessage(String(message));
    });
    controllerService.registerPushDateTime([statusApp](const char* message) -> void {
        statusApp->logDatetime(String(message));
    });
    controllerService.registerPushStatus([statusApp](const char* message) -> void {
        statusApp->logStatus(String(message));
    });
    controllerService.registerPushLevel([statusApp](const char* message) -> void {
        statusApp->logLevel(String(message));
    });
    controllerService.registerPushMode([statusApp](String mode) -> void {
        statusApp->modeCallback(mode);
    });

    loggingService.logMessage(LOG_LEVEL_DEBUG, LOG_MODE_SERIAL, "Status app setup completed");

    // create and subscribe to mqtt topics
    mqttService.initTopics();

    // ----> SETUP INITIAL MODE <----
    controllerService.setMode(flashMode->getModeInternalName());

    loggingService.logMessage(LOG_LEVEL_DEBUG, LOG_MODE_SERIAL, "MQTT topics initialized");

    // set mqtt callback
    mqttService.setOnMessageCallback(&mqttServiceCallbackWrapper);

    loggingService.logMessage(LOG_LEVEL_DEBUG, LOG_MODE_SERIAL, "MQTT callback set");

    // add tasks to scheduler
    scheduler.addTask(mqttStatusUpdateTask);
    scheduler.addTask(mqttServiceUpdateDateTimeTask);
    scheduler.addTask(timeSyncTask);
    scheduler.addTask(mqttServiceLoopTask);
    scheduler.addTask(ledServiceLoopTask);

    loggingService.logMessage(LOG_LEVEL_DEBUG, LOG_MODE_SERIAL, "Tasks added to scheduler");

    // enable all tasks
    mqttStatusUpdateTask.enable();
    mqttServiceUpdateDateTimeTask.enable();
    timeSyncTask.enable();
    mqttServiceLoopTask.enable();
    ledServiceLoopTask.enable();

    loggingService.logMessage(LOG_LEVEL_DEBUG, LOG_MODE_SERIAL, "Tasks enabled");

    // --- setup completed ---
    controllerService.setStatus("Setup completed");
    loggingService.logMessage(LOG_LEVEL_INFO, LOG_MODE_ALL, "Setup completed");

    controllerService.setStatus("alive");
}

void loop() {
    scheduler.execute();
    vTaskDelay(1);
}


// helper functions
void connectToWiFi() {
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    WiFi.waitForConnectResult();

    WiFi.persistent(false);
    WiFi.setAutoConnect(true);

    #ifdef CUSTOM_DNS
    WiFi.config(WiFi.localIP(), WiFi.gatewayIP(), WiFi.subnetMask(), IPAddress(DNS_SERVER));
    #endif

    Serial.print("[INFO] Connecting to WiFi");

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("Connected");
}

// task wrappers
void mqttServiceStatusUpdateWrapper() {
    controllerService.mqttServiceStatusUpdateWrapper();
}

void mqttServiceLoopWrapper() {
    controllerService.mqttServiceLoopWrapper();
}

void mqttServiceUpdateDateTimeWrapper() {
    controllerService.mqttServiceUpdateDateTimeWrapper();
}

void clockServiceTimeSyncWrapper() {
    controllerService.clockServiceTimeSyncWrapper();
}

void ledServiceLoopWrapper() {
    controllerService.ledServiceLoopWrapper();
}

void mqttServiceCallbackWrapper(const espMqttClientTypes::MessageProperties& properties, const char* topic, const uint8_t* payload, size_t len, size_t index, size_t total) {
    controllerService.mqttServiceCallbackWrapper(properties, topic, payload, len, index, total);
}
