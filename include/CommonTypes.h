#ifndef TYPES_H
#define TYPES_H

#include <functional>
#include <Arduino.h>


enum payload_type_e {
    STRING,
    BYTE,
    INT,
    FLOAT,
    COLOR
};

struct boundaries_t {
    int min;
    int max;
};

struct mode_callback_t {
    String modeName;
    String localTopic;
    String globalPubTopic;
    String defaultPayload;
    boundaries_t boundaries;
    payload_type_e payloadType;
    std::function<void(String payload)> topicCallback;
};

#endif // TYPES_H