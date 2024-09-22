#include "MqttService.h"

MqttService::MqttService() : mqttClient(wifiClient)
{
}

void MqttService::setup()
{
    this->connectToWiFi();

    mqttClient.setServer(MQTT_BROKER, MQTT_PORT);

    this->mqttReconnect();

    this->initialized = true;
}

void MqttService::loop()
{
    if (!this->initialized)
    {
        return;
    }

    if (!mqttClient.connected())
    {
        this->mqttReconnect();
    }

    mqttClient.loop();
}

void MqttService::connectToWiFi()
{
    delay(10);

    WiFi.setHostname(DEVICE_NAME);

    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    WiFi.waitForConnectResult();

    #ifdef CUSTOM_DNS
    WiFi.config(WiFi.localIP(), WiFi.gatewayIP(), WiFi.subnetMask(), IPAddress(DNS_SERVER));
    #endif

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
    }
}

void MqttService::mqttReconnect()
{
    Serial.println("Attempting MQTT connection...");
    while (!mqttClient.connected())
    {
        if (mqttClient.connect(DEVICE_NAME, MQTT_USERNAME, MQTT_PASSWORD))
        {
            mqttClient.publish(MQTT_STATUS_MSG_TOPIC, "connected");
        }
        else
        {
            delay(5000);
        }
    }
    Serial.println("MQTT connected");
}

void MqttService::mqttStatusUpdate()
{
    if (this->initialized)
    {
        mqttClient.publish(MQTT_STATUS_MSG_TOPIC, "alive");
    }
}