# LumiMQTT Library Services

This repository contains various services used for the LumiMQTT project. The services are defined in the `/lib` directory and provide different functionalities such as logging, task management, time synchronization, and MQTT communication.

## Table of Contents

- [ClockService](#clockservice)
- [LoggingService](#loggingservice)
- [TaskService](#taskservice)
- [MQTTService](#mqttservice)

## ClockService

The `ClockService` is responsible for time synchronization and retrieving the current time via NTP (Network Time Protocol).

### Functions

- **getDateTime(char* buffer)**: Retrieves the current date and time and stores it in the provided buffer.

### Configuration

The NTP configuration is done in the `config.h` file:

```cpp
// NTP Configuration
#define NTP_SERVER "time.nist.gov"
#define NTP_OFFSET 3600 * 2   // UTC+2
```

## LoggingService

The `LoggingService` provides functions for logging messages at various levels and modes.

### Functions

- **setup()**: Initializes the logging service.
- **setup(short logLevel)**: Initializes the logging service with a specific log level.
- **setup(ClockService clockService, TaskService taskService)**: Initializes the logging service with dependencies.
- **setup(ClockService clockService, TaskService taskService, short logLevel)**: Initializes the logging service with dependencies and a specific log level.
- **logMessage(short logLevel, short mode, char* message)**: Logs a message at the specified level and mode.
- **logMessage(short logLevel, char* message)**: Logs a message at the specified level.
- **logMessage(char* message)**: Logs a message.

### Configuration

The logging configuration is done in the `config.h` file:

```cpp
// Logging Configuration
#define LOG_LEVEL_ERROR 0x01
#define LOG_LEVEL_WARN  0x02
#define LOG_LEVEL_INFO  0x04
#define LOG_LEVEL_DEBUG 0x08

#define LOG_MODE_SERIAL 0x01
#define LOG_MODE_MQTT   0x02
#define LOG_MODE_ALL    0xFF

#define LOG_LEVEL LOG_LEVEL_DEBUG
#define LOG_MODE  LOG_MODE_SERIAL

#define LOG_SERIAL_SPEED 115200

// Example: "LumiMQTT (2020-01-01 00:00:00) [INFO] 'Message'"
#define LOG_STRING "%s (%s) [%s] '%s'"
```

## TaskService

The `TaskService` manages various tasks and ensures they are executed correctly.

### Functions

- **setup()**: Initializes the task service.
- **setup(LoggingService loggingService)**: Initializes the task service with a logging service.
- **initWiFi()**: Initializes the WiFi connection.
- **mqttStatusUpdate()**: Updates the MQTT status.

### Configuration

The WiFi and MQTT configuration is done in the `config.h` file:

```cpp
// WiFi Configuration
#define WIFI_SSID "your-ssid"
#define WIFI_PASSWORD "your-password"

// MQTT Configuration
#define MQTT_BROKER "your-mqtt-broker"
#define MQTT_PORT 1883
#define MQTT_STATUS_MSG_TOPIC "status/msg"
#define MQTT_STATUS_LOG_TOPIC "status/log"
#define MQTT_STATUS_DATETIME_TOPIC "status/datetime"
```

## MQTTService

The `MQTTService` is responsible for MQTT communication, including connecting to an MQTT broker and managing MQTT messages.

### Functions

- **setup()**: Initializes the MQTT service and connects to the MQTT broker.
- **loop()**: Runs the main loop of the MQTT client to receive and send messages.
- **publish(const char* topic, const char* payload)**: Publishes a message to a specific MQTT topic.
- **subscribe(const char* topic)**: Subscribes to a specific MQTT topic.

### Configuration

The MQTT configuration is done in the `config.h` file:

```cpp
// MQTT Configuration
#define MQTT_BROKER "your-mqtt-broker"
#define MQTT_PORT 1883
#define MQTT_STATUS_MSG_TOPIC "status/msg"
#define MQTT_STATUS_LOG_TOPIC "status/log"
#define MQTT_STATUS_DATETIME_TOPIC "status/datetime"
```

## License

This project is licensed under the MIT License. See the `LICENSE` file for more details.

## Contact

For questions or support, please contact [Your Name or Email Address].
