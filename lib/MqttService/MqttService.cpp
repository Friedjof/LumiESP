#include "MqttService.h"


MqttService::MqttService()
{
    // set mqtt callbacks
    mqttClient.onMessage(std::bind(&MqttService::onMessage, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5, std::placeholders::_6));
    
    mqttClient.setCleanSession(true);
}

void MqttService::setup()
{
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
        if (topic.second.topicType == topic_e::PUB_ONLY || topic.second.topicType == topic_e::PUB_SUB)
        {
            mqttClient.publish(topic.second.globalPubTopic.c_str(), 0, false, topic.second.defaultPayload.c_str());
        }
        
        if (topic.second.topicType == topic_e::SUB_ONLY || topic.second.topicType == topic_e::PUB_SUB)
        {
            mqttClient.publish(topic.first.c_str(), 0, false, topic.second.defaultPayload.c_str());
        }

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

void MqttService::connect()
{
    while (!mqttClient.connected())
    {
        if (mqttClient.connect())
        {
            Serial.println("Connected to MQTT broker");
            //this->publish(this->mqttStatusTopic(MQTT_STATUS_MSG_TOPIC).c_str(), "connected");
        }
        else
        {
            delay(5000);
        }
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
std::function<void(String payload)> MqttService::subscribeModeTopic(String modeName, String localTopic, String defaultPayload, boundaries_t boundaries, payload_e payloadType, topic_e topicType, std::function<void(String payload)> topicCallback) {
    char subTopic[64];
    char pubTopic[64];

    sprintf(subTopic, MQTT_MODE_TOPIC_SUB_STRING, modeName.c_str(), localTopic.c_str());
    sprintf(pubTopic, MQTT_MODE_TOPIC_PUB_STRING, modeName.c_str(), localTopic.c_str());

    String globalSubTopic = this->mqttGlobalTopic(subTopic);
    String globalPubTopic = this->mqttGlobalTopic(pubTopic);

    this->modeTopics[globalSubTopic] = {modeName, localTopic, globalPubTopic, defaultPayload, boundaries, payloadType, topicType, topicCallback};

    return [this, globalPubTopic](String payload) {
        this->publish(globalPubTopic, payload.c_str());
    };
}

std::function<void(String payload)> MqttService::subscribeModeTopic(String modeName, String localTopic, payload_e payloadType, topic_e topicType) {
    return this->subscribeModeTopic(modeName, localTopic, "", boundaries_t(), payloadType, topicType, nullptr);
}
std::function<void(String payload)> MqttService::subscribeModeTopic(String modeName, String localTopic) {
    return this->subscribeModeTopic(modeName, localTopic, "", boundaries_t(), payload_e::STRING, topic_e::PUB_ONLY, nullptr);
}
std::function<void(String payload)> MqttService::subscribeModeTopic(String modeName, String localTopic, const char* defaultPayload, payload_e payloadType, topic_e topicType, std::function<void(String payload)> topicCallback) {
    return this->subscribeModeTopic(modeName, localTopic, defaultPayload, boundaries_t(), payloadType, topicType, topicCallback);
}

std::function<void(String payload)> MqttService::subscribeModeTopic(String modeName, String localTopic, const char* defaultPayload, payload_e payloadType, std::function<void(String payload)> topicCallback) {
    return this->subscribeModeTopic(modeName, localTopic, defaultPayload, boundaries_t(), payloadType, topic_e::PUB_SUB, topicCallback);
}
std::function<void(String payload)> MqttService::subscribeModeTopic(String modeName, String localTopic, int defaultPayload, boundaries_t boundaries, payload_e payloadType, std::function<void(String payload)> topicCallback) {
    return this->subscribeModeTopic(modeName, localTopic, String(defaultPayload), boundaries, payloadType, topic_e::PUB_SUB, topicCallback);
}

// ------- PUBLISH TOPICS -------
void MqttService::publish(const char* subTopic, const char* message)
{
    if (this->initialized)
    {
        mqttClient.publish(this->mqttGlobalTopic(subTopic).c_str(), 0, false, message);
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

    if (it->second.topicCallback == nullptr) {
        return false;
    }

    payload_e payloadType = it->second.payloadType;
    boundaries_t boundaries = it->second.boundaries;

    bool isValid = false;

    switch (payloadType)
    {
        case payload_e::BYTE:
        {
            char* end;
            long value = strtol(payload.c_str(), &end, 10);
            isValid = (*end == '\0' && value >= boundaries.min && value <= boundaries.max);
            break;
        }
        case payload_e::INT:
        {
            char* end;
            long value = strtol(payload.c_str(), &end, 10);
            isValid = (*end == '\0' && value >= boundaries.min && value <= boundaries.max);
            break;
        }
        case payload_e::FLOAT:
        {
            char* end;
            float value = strtof(payload.c_str(), &end);
            isValid = (*end == '\0' && value >= boundaries.min && value <= boundaries.max);
            break;
        }
        case payload_e::COLOR:
        {
            isValid = isHexColor(payload);
            break;
        }
        case payload_e::STRING:
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
