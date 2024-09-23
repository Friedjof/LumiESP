#include "MqttService.h"


MqttService::MqttService() : mqttClient()
{
    // set mqtt callbacks
    mqttClient.onConnect(std::bind(&MqttService::onConnect, this, std::placeholders::_1));
    mqttClient.onSubscribe(std::bind(&MqttService::onSubscribe, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
    mqttClient.onMessage(std::bind(&MqttService::onMessage, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5, std::placeholders::_6));
    
    mqttClient.setCleanSession(true);
}

void MqttService::setup()
{
    this->connectToWiFi();

    // set mqtt client settings
    mqttClient.setServer(MQTT_BROKER, MQTT_PORT);
    mqttClient.setCredentials(MQTT_USERNAME, MQTT_PASSWORD);
    mqttClient.setClientId(MQTT_CLIENT_ID);

    this->connect();

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
        this->connect();
    }

    mqttClient.loop();
}

void MqttService::connectToWiFi()
{
    delay(10);

    WiFi.setHostname(DEVICE_NAME);

    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    WiFi.waitForConnectResult();

    WiFi.persistent(false);
    WiFi.setAutoConnect(true);

    #ifdef CUSTOM_DNS
    WiFi.config(WiFi.localIP(), WiFi.gatewayIP(), WiFi.subnetMask(), IPAddress(DNS_SERVER));
    #endif

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
    }
}

void MqttService::connect()
{
    while (!mqttClient.connected())
    {
        if (mqttClient.connect())
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
        mqttClient.publish(this->mqttGlobalTopic(subTopic).c_str(), 0, false, message);
    }
}

void MqttService::subscribe()
{
    this->subscribe(this->mqttLedSubTopic(MQTT_LED_MODE_TOPIC));
}

void MqttService::subscribe(const char* subTopic)
{
    if (this->initialized)
    {
        mqttClient.subscribe(this->mqttGlobalTopic(subTopic).c_str(), 0);
    }
}

void MqttService::subscribe(String subTopic)
{
    if (this->initialized)
    {
        this->subscribe(subTopic.c_str());
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


// ------- MQTT CALLBACKS -------
void MqttService::onConnect(bool sessionPresent)
{
    this->createTopics();
    this->mqttStatusUpdate();
}

void MqttService::onSubscribe(uint16_t packetId, const espMqttClientTypes::SubscribeReturncode* codes, size_t len)
{
    // TODO: implement onSubscribe here
}

void MqttService::onMessage(const espMqttClientTypes::MessageProperties& properties, const char* topic, const uint8_t* payload, size_t len, size_t index, size_t total)
{
    // TODO: implement onMessage here
}

void MqttService::setCallback(void (*callback)(const espMqttClientTypes::MessageProperties& properties, const char* topic, const uint8_t* payload, size_t len, size_t index, size_t total))
{
    mqttClient.onMessage(callback);
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

bool MqttService::isLedModeSubTopic(String topic)
{
    return topic == this->mqttGlobalTopic(this->mqttLedSubTopic(MQTT_LED_MODE_TOPIC).c_str());
}