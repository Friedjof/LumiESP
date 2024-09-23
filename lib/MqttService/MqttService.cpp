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

void MqttService::createTopics()
{
    // create all topics
    const char* initMsg = "initialized";
    this->publish(this->mqttStatusTopic(MQTT_STATUS_MSG_TOPIC), initMsg);
    this->publish(this->mqttStatusTopic(MQTT_STATUS_LOG_TOPIC), initMsg);
    this->publish(this->mqttStatusTopic(MQTT_STATUS_DATETIME_TOPIC), initMsg);

    this->publish(this->mqttLedPubTopic(MQTT_LED_MODE_TOPIC), initMsg);
    this->publish(this->mqttLedPubTopic(MQTT_LED_STATE_TOPIC), initMsg);
    this->publish(this->mqttLedSubTopic(MQTT_LED_MODE_TOPIC), initMsg);
    this->publish(this->mqttLedSubTopic(MQTT_LED_STATE_TOPIC), initMsg);
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
    while (!mqttClient.connected())
    {
        if (mqttClient.connect(DEVICE_NAME, MQTT_USERNAME, MQTT_PASSWORD))
        {
            this->publish(this->mqttStatusTopic(MQTT_STATUS_MSG_TOPIC).c_str(), "connected");
        }
        else
        {
            delay(5000);
        }
    }
}

void MqttService::mqttStatusUpdate()
{
    if (this->initialized)
    {
        this->publish(this->mqttStatusTopic(MQTT_STATUS_MSG_TOPIC).c_str(), "alive");
    }
}

void MqttService::mqttDatetimeUpdate(const char* datetime)
{
    if (this->initialized)
    {
        this->publish(this->mqttStatusTopic(MQTT_STATUS_DATETIME_TOPIC).c_str(), datetime);
    }
}

void MqttService::publish(const char* subTopic, const char* message)
{
    if (this->initialized)
    {
        mqttClient.publish(this->mqttGlobalTopic(subTopic).c_str(), message);
    }
}

void MqttService::publish(String subTopic, const char* message)
{
    if (this->initialized)
    {
        this->publish(subTopic.c_str(), message);
    }
}

void MqttService::publish(String subTopic, String message)
{
    if (this->initialized)
    {
        this->publish(subTopic.c_str(), message.c_str());
    }
}

void MqttService::callback(char* topic, byte* payload, unsigned int length)
{
    // TODO: implement callback
}

void MqttService::setCallback(void (*callback)(char*, byte*, unsigned int))
{
    mqttClient.setCallback(callback);
}

// ------- HELPER FUNCTIONS -------
String MqttService::mqttGlobalTopic(const char* subTopic)
{
    char result[64];
    sprintf(result, MQTT_TOPIC_STRING, DEVICE_NAME, subTopic);

    return String(result);
}

String MqttService::mqttStatusTopic(const char* subTopic)
{
    char result[64];
    sprintf(result, MQTT_STATUS_TOPIC, subTopic);

    return String(result);
}

String MqttService::mqttLedPubTopic(const char* subTopic)
{
    char result[64];
    sprintf(result, MQTT_LED_PUB_TOPIC, subTopic);

    return String(result);
}

String MqttService::mqttLedSubTopic(const char* subTopic)
{
    char result[64];
    sprintf(result, MQTT_LED_SUB_TOPIC, subTopic);

    return String(result);
}

bool MqttService::isLedTopic(const char* topic)
{
    return String(topic).startsWith(String(DEVICE_NAME) + "/led/sub/");
}

bool MqttService::isLedTopic(String topic)
{
    return topic.startsWith(String(DEVICE_NAME) + "/led/sub/");
}
