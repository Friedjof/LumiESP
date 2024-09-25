#ifndef TYPES_H
#define TYPES_H

#include <functional>
#include <Arduino.h>

// --- enums ---
enum payload_e {
    STRING,
    BYTE,
    INT,
    FLOAT,
    COLOR
};

enum topic_e {
    PUB_SUB,
    PUB_ONLY,
    SUB_ONLY,
    NONE
};

// --- structs ---
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
    payload_e payloadType;
    topic_e topicType;
    std::function<void(String payload)> topicCallback;
};

#endif // TYPES_H