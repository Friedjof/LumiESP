#include "MqttService.h"


MqttService::MqttService() : mqttClient()
{
    // set mqtt callbacks
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

void MqttService::initTopics()
{
    // create all topics
    for (auto const& topic : this->modeTopics)
    {
        // publish the default payloads
        mqttClient.publish(topic.second.globalPubTopic.c_str(), 0, false, topic.second.defaultPayload.c_str());
        mqttClient.publish(topic.first.c_str(), 0, false, topic.second.defaultPayload.c_str());

        // subscribe to the topic
        this->subscribe(topic.first);
    }
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
            // TODO: implement connect here
            //this->publish(this->mqttStatusTopic(MQTT_STATUS_MSG_TOPIC).c_str(), "connected");
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
        // TODO: implement mqttStatusUpdate here
        //this->publish(this->mqttStatusTopic(MQTT_STATUS_MSG_TOPIC).c_str(), "alive");
    }
}

void MqttService::mqttDatetimeUpdate(const char* datetime)
{
    if (this->initialized)
    {
        // TODO: implement mqttDatetimeUpdate here
        //this->publish(this->mqttStatusTopic(MQTT_STATUS_DATETIME_TOPIC).c_str(), datetime);
    }
}

void MqttService::publish(const char* subTopic, const char* message)
{
    if (this->initialized)
    {
        mqttClient.publish(this->mqttGlobalTopic(subTopic).c_str(), 0, false, message);
    }
}

// ------- GENERIC SUBSCRIBE -------
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

// ------- SUBSCRIBE TOPICS -------
std::function<void(String payload)> MqttService::subscribeModeTopic(String modeName, String localTopic, String defaultPayload, boundaries_t boundaries, payload_type_e payloadType, std::function<void(String payload)> topicCallback) {
    // TODO: make the topics configurable in the config.h
    String globalSubTopic = this->mqttGlobalTopic("modes/" + modeName + "/sub/" + localTopic);
    String globalPubTopic = this->mqttGlobalTopic("modes/" + modeName + "/pub/" + localTopic);

    this->modeTopics[globalSubTopic] = {modeName, localTopic, globalPubTopic, defaultPayload, boundaries, payloadType, topicCallback};

    return [this, globalPubTopic](String payload) {
        this->publish(globalPubTopic, payload.c_str());
    };
}

std::function<void(String payload)> MqttService::subscribeModeTopic(String modeName, String localTopic, int defaultPayload, boundaries_t boundaries, payload_type_e payloadType, std::function<void(String payload)> topicCallback) {
    return this->subscribeModeTopic(modeName, localTopic, String(defaultPayload), boundaries, payloadType, topicCallback);
}

std::function<void(String payload)> MqttService::subscribeModeTopic(String modeName, String localTopic, int defaultPayload, boundaries_t boundaries, std::function<void(String payload)> topicCallback) {
    return this->subscribeModeTopic(modeName, localTopic, defaultPayload, boundaries, payload_type_e::INT, topicCallback);
}

std::function<void(String payload)> MqttService::subscribeModeTopic(String modeName, String localTopic, int defaultPayload, std::function<void(String payload)> topicCallback) {
    return this->subscribeModeTopic(modeName, localTopic, defaultPayload, boundaries_t(), payload_type_e::INT, topicCallback);
}

std::function<void(String payload)> MqttService::subscribeModeTopic(String modeName, String localTopic, float defaultPayload, std::function<void(String payload)> topicCallback) {
    return this->subscribeModeTopic(modeName, localTopic, String(defaultPayload), boundaries_t(), payload_type_e::FLOAT, topicCallback);
}

std::function<void(String payload)> MqttService::subscribeModeTopic(String modeName, String localTopic, String defaultPayload, payload_type_e payloadType, std::function<void(String payload)> topicCallback) {
    return this->subscribeModeTopic(modeName, localTopic, defaultPayload, boundaries_t(), payloadType, topicCallback);
}

std::function<void(String payload)> MqttService::subscribeModeTopic(String modeName, String localTopic, String defaultPayload, std::function<void(String payload)> topicCallback) {
    return this->subscribeModeTopic(modeName, localTopic, defaultPayload, boundaries_t(), payload_type_e::STRING, topicCallback);
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
void MqttService::onSubscribe(uint16_t packetId, const espMqttClientTypes::SubscribeReturncode* codes, size_t len)
{
    // TODO: implement onSubscribe here
}

void MqttService::onMessage(const espMqttClientTypes::MessageProperties& properties, const char* topic, const uint8_t* payload, size_t len, size_t index, size_t total)
{
    // TODO: implement onMessage here
}

bool MqttService::onMessageCallback(String topic, String payload)
{
    auto it = this->modeTopics.find(topic);
    if (it == this->modeTopics.end()) {
        return false;
    }

    payload_type_e payloadType = it->second.payloadType;
    boundaries_t boundaries = it->second.boundaries;

    bool isValid = false;

    switch (payloadType)
    {
        case payload_type_e::INT:
        {
            char* end;
            long value = strtol(payload.c_str(), &end, 10);
            isValid = (*end == '\0' && value >= boundaries.min && value <= boundaries.max);
            break;
        }
        case payload_type_e::FLOAT:
        {
            char* end;
            float value = strtof(payload.c_str(), &end);
            isValid = (*end == '\0' && value >= boundaries.min && value <= boundaries.max);
            break;
        }
        case payload_type_e::COLOR:
        {
            isValid = this->isValidHexColor(payload);
            break;
        }
        case payload_type_e::STRING:
        default:
            isValid = true;
            break;
    }

    if (isValid) {
        it->second.topicCallback(payload);
    }

    return isValid;
}

void MqttService::setOnMessageCallback(void (*callback)(const espMqttClientTypes::MessageProperties& properties, const char* topic, const uint8_t* payload, size_t len, size_t index, size_t total))
{
    mqttClient.onMessage(callback);
}

// ------- HELPER FUNCTIONS -------
// <device_name>/<sub_topics>
String MqttService::mqttGlobalTopic(const char* subTopic)
{
    if (String(subTopic).startsWith(DEVICE_NAME))
    {
        return String(subTopic);
    }

    char result[64];
    sprintf(result, MQTT_TOPIC_STRING, DEVICE_NAME, subTopic);

    return String(result);
}

String MqttService::mqttGlobalTopic(String subTopic)
{
    return this->mqttGlobalTopic(subTopic.c_str());
}

bool MqttService::isValidHexColor(String color) {
    // Check if the string starts with '#' and is either 4 or 7 characters long
    if ((color.length() != 4 && color.length() != 7) || color[0] != '#') {
        return false;
    }

    // Check if all characters after '#' are valid hexadecimal digits
    for (size_t i = 1; i < color.length(); ++i) {
        if (!std::isxdigit(color[i])) {
            return false;
        }
    }

    return true;
}
