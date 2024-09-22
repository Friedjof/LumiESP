#include "TaskService.h"


TaskService::TaskService(MqttService mqttService, ClockService clockService, LoggingService loggingService) : mqttClient(wifiClient), clockService(clockService), loggingService(loggingService)
{
}

void TaskService::setup()
{
    this->initWiFi();
    this->clockService.syncTime();
    this->initialized = true;
}

void TaskService::mqttStatusUpdate()
{
    char datetime[100];
    clockService.getDateTime(datetime);

    char log_msg[120];
    snprintf(log_msg, 100, "(%s-%s) status update sent", DEVICE_NAME, datetime);

    this->mqttClient.publish(MQTT_STATUS_MSG_TOPIC, "alive");
    this->mqttClient.publish(MQTT_STATUS_LOG_TOPIC, log_msg);
    this->mqttClient.publish(MQTT_STATUS_DATETIME_TOPIC, datetime);
}

void TaskService::initWiFi()
{
    delay(10);
    #if DEBUG
    Serial.println("------------------------------------");
    Serial.print("Connecting to ");
    Serial.println(WIFI_SSID);
    #endif

    // Set the hostname
    if (!WiFi.setHostname(DEVICE_NAME)) {
        #if DEBUG
        Serial.println("Failed to set hostname");
        #endif
    }

    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    WiFi.waitForConnectResult();

    #ifdef CUSTOM_DNS
    WiFi.config(WiFi.localIP(), WiFi.gatewayIP(), WiFi.subnetMask(), IPAddress(DNS_SERVER));
    #endif

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        #if DEBUG
        Serial.print(".");
        #endif
    }

    #if DEBUG
    Serial.println("");

    Serial.println("WiFi connected");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    Serial.print("DNS: ");
    Serial.println(WiFi.dnsIP());

    Serial.print("Hostname: ");
    Serial.println(WiFi.getHostname());
    #endif

    this->isWiFiConnected = true;
}

void TaskService::mqttReconnect()
{
    // Loop until we're reconnected
    while (!this->mqttClient.connected()) {
        #if DEBUG
        Serial.print("Attempting MQTT connection...");
        #endif

        // Attempt to connect
        if (this->mqttClient.connect(MQTT_CLIENT_ID, MQTT_USERNAME, MQTT_PASSWORD)) {
            #if DEBUG
            Serial.println("connected");
            #endif

            // Once connected, publish an announcement...
            this->mqttClient.publish(MQTT_STATUS_MSG_TOPIC, "connected");
        } else {
            #if DEBUG
            Serial.print("failed, rc=");
            Serial.print(this->mqttClient.state());
            Serial.println(" try again in 5 seconds");
            #endif
            
            // Wait 5 seconds before retrying
            delay(5000);
        }
    }
}

void TaskService::loop()
{
    if (!this->isWiFiConnected) {
        this->initWiFi();
    }
    if (!this->mqttClient.connected()) {
        this->mqttReconnect();
    }

    this->mqttLoop();
}