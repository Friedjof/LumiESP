# LumiESP
LumiESP is a project utilizing an ESP32 microcontroller to control an LED strip via MQTT. It is designed for easy customization and extension, enabling the creation of custom modes and settings. The included Makefile streamlines PlatformIO commands for building, uploading, and monitoring the code.

> **Note:** This project is still in development and may contain bugs or incomplete features. Please report any issues you encounter.

## What is the ESP32 and MQTT?
The ESP32 is a versatile, low-cost microcontroller with built-in Wi-Fi and Bluetooth capabilities, making it ideal for IoT (Internet of Things) projects. It allows devices to connect to the internet and communicate wirelessly. MQTT (Message Queuing Telemetry Transport) is a lightweight messaging protocol designed for small sensors and mobile devices. It enables efficient communication between devices by sending and receiving messages over a network.

## Features
- Control LED strips using the ESP32 over MQTT (e.g., LED strips like `WS2812`, `WS2812B`, `WS2813`, etc.)
- Customizable settings for LED strips, such as brightness and color and custom modes
- Simplified build, upload, and monitoring process using PlatformIO commands
- Ability to define custom modes and settings for the LED strips

## Requirements
- ESP32 microcontroller (e.g., ESP32 Dev Module)
- PlatformIO installed on your system or simply use this [nix-shell](/shell.nix)
- LED strip (e.g., WS2812, WS2812B, WS2813)
- MQTT broker (e.g., Mosquitto)

## Installation
### 1. Install PlatformIO
PlatformIO is a cross-platform code builder and the easiest way to manage your embedded software environment. To install PlatformIO, follow the official instructions found [here](https://docs.platformio.org/en/latest/core/installation/methods/installer-script.html).

> **Note:** If you are using NixOS, you can use the provided `shell.nix` file to enter a shell with PlatformIO installed.

### 2. Clone the Repository
Clone the LumiESP project repository to your local machine:

```bash
git clone https://github.com/friedjof/lumiESP.git
cd LumiESP
```

### 3. Build, Upload, and Monitor
The project includes a Makefile that simplifies the use of PlatformIO commands. Make sure to edit the variables in the Makefile according to your setup.

> **Note:** You have to copy the `config/config.h-template` to `config/config.h` and fill in the necessary information. You can also use the `setup.py` script to do this for you (run `python setup.py` or `make setup`).

#### Variables
- `PORT`: Serial port of your ESP32 (e.g., `/dev/ttyUSB0`).
- `BOARD`: The type of ESP32 board (e.g., `esp32dev`).
- `SPEED`: Baud rate for the serial monitor (e.g., `115200`).

### Development Environment
You can run a `mosquitto` MQTT broker locally on your machine for testing. To run the MQTT broker using Docker, follow these steps:

> **Note:** Make sure you have Docker installed on your system and you are in the root directory of this project.

Setup the username and password for the MQTT broker (replace `<username>` with your desired username e.g., `esp`):
```bash
docker run -it -v ./mosquitto/config:/mosquitto/config eclipse-mosquitto mosquitto_passwd -c /mosquitto/config/password.txt <username>
```

Start the MQTT broker
```bash
docker compose up -d
```

#### Makefile
- **Build the project:** `make build`
- **Upload the code to the ESP32:** `make upload`
- **Monitor the serial output:** `make monitor`
- **Clean the build files:** `make clean`
- **Setup the repository:** `make setup`
- **Clean, build, flash, and open the serial monitor:** `make flash`
- **print the help message:** `make help`

## MQTT Topics
```txt
LumiESP
├── LumiESP     // this defines the system topic
│   ├── sub
│   │   ├── status     // the system status
│   │   ├── datetime   // the current date and time
│   │   ├── log        // the log message
│   │   ├── level      // the log level
│   │   └── mode       // the current mode
│   └── pub
│       └── mode       // set the new mode
├── StaticMode  // this is a basic mode that sets the LED strip to a static color
│   ├── sub
│   │   ├── brightness // the current brightness
│   │   └── hex        // the current color in hex format
│   └── pub
│       ├── brightness // set the new brightness
│       └── hex        // set the new color in hex format
…
```

> **Note:** The system topic is used to publish and subscribe to system-related information, such as the current mode, status, and log messages. The other topics are used to control the LED strip and define custom modes.

## Define your own custom modes
You can define your custom modes very easily.
1. Create a new folder like `ModeCustom` in the `lib/` directory.
2. Create a new header `CustomMode.h` and a new source file `CustomMode.cpp` in the new folder.
3. Implement the `CustomMode` class in the header and source file. Use the `AbstractMode` class as a base class.

> `CustomMode.h`
```cpp
// ifndef is a preprocessor directive that checks if the given token has been #defined earlier in the file or in an included file
#ifndef CUSTOMMODE_H
#define CUSTOMMODE_H


#include <functional>

#include "AbstractMode.h"


class CustomMode : public AbstractMode {
private:
    // Some internal properties
    String customColor = "#424242";
    String newCustomColor = this->customColor;
    byte customBrightness = 255;
    byte newCustomBrightness = this->customBrightness;

    // This method is called when a new color is set via MQTT
    void customColorCallback(const String &color);
    void customBrightnessCallback(const byte &brightness);

    // The push-methods are used to publish the new values back to the MQTT broker to acknowledge the change
    std::function<void(String payload)> pushCustomColor = std::bind(&CustomMode::customColorCallback, this, std::placeholders::_1);
    std::function<void(String payload)> pushCustomBrightness = std::bind(&CustomMode::customBrightnessCallback, this, std::placeholders::_1);

    // Some helper methods to identify the changes
    bool isNewCustomColor();
    bool isNewCustomBrightness();

public:
    CustomMode(ControllerService* controllerService);

    // The custom setup and loop methods are used to define the behavior of the mode

    // The "customSetup" method is called once when the mode is initialized in the "main.cpp" file
    void customSetup() override;

    // The "customLoop" method is called in every loop iteration
    // The "steps" is a global counter that is increased in every loop iteration over every mode
    void customLoop(unsiged long long steps) override;
};

#endif
```

> `CustomMode.cpp`
```cpp
// include the header file of the custom mode
#include "CustomMode.h"


// Implement the constructor of the custom mode
// It is important to call the constructor of the base class "AbstractMode" with the "controllerService" parameter
CustomMode::CustomMode(ControllerService* controllerService) : AbstractMode(controllerService) {
    // This parameter is used to classify the mode in general. It is commen sense to discribe some basic information about the mode and the author
    this->modeTitle = "Custom Mode";
    this->modeDescription = "This is a custom mode that sets the LED strip to a custom color and brightness.";
    this->modeInternalName = "CustomMode"; // This title is used to identify the mode later. It is very important to use a **unique name**
    this->modeAuthor = "<Your Name>";      // You can use your name or your username
    this->modeContact = "<Your Email or an other contact information (optional)>";
    this->modeVersion = "1.0.0";           // You can define a version number
    this->modeLicense = "MIT";             // You can use any license you want
}

void CustomMode::customSetup() {
    // This method is called once when the mode is initialized
    // Here you can specify the MQTT topics you want to push and subscribe to

    // Note: The "controllerService" is a pointer to the "ControllerService" central instance that is used to manage the other servives
    //       See the diagram in the "Services and classes" section for more information
    //       The pointer is managed by the "AbstractMode" class and you can simply use it in your custom mode

    // >>> Topics: "<DeviceName>/<ModeInternalName>/sub/customcolor" and "<DeviceName>/<ModeInternalName>/pub/customcolor"
    // Parameters: modeInternalName, topicName, defaultValue, payloadType (e.g., COLOR, INT, BYTE, BOOL, STRING, etc.), callback
    this->pushCustomColor = this->controllerService->subscribeModeTopic(
        this->modeInternalName, "customcolor", this->customColor.c_str(), payload_e::COLOR,
        std::function<void(String)>(std::bind(&CustomMode::customColorCallback, this, std::placeholders::_1)));
    // The "customColorCallback" method is called when a new color is set via MQTT and you can use the "pushCustomColor" method to acknowledge the change

    // >>> Topics: <DeviceName>/<ModeInternalName>/sub/custombrightness and <DeviceName>/<ModeInternalName>/pub/custombrightness
    // Parameters: modeInternalName, topicName, defaultValue, boundaries (min, max), payloadType, callback
    this->pushCustomBrightness = this->controllerService->subscribeModeTopic(
        this->modeInternalName, "custombrightness", this->customBrightness, boundaries_t{0, 255},
        payload_e::BYTE, std::function<void(String)>(std::bind(&CustomMode::customBrightnessCallback, this, std::placeholders::_1)));
}

void CustomMode::customLoop(unsigned long long steps) {
    // This method is called in every loop iteration.
    // Here you can define the behavior of your mode.

    // This is a function of the "AbstractMode" class that is used to check if this is the first loop iteration after activating this mode
    if (this->isFirstRun()) {
        // Here you can update the LED strip because LEDs are in the state of the last mode
        // You can use the "setHexColor" method to set the color
        this->controllerService->setHexColor(this->customColor);
        // You can use the "setBrightness" method to set the brightness
        this->controllerService->setBrightness(this->customBrightness);

        // Do not forget to confirm the new configuration
        this->controllerService->confirmLedConfig();
    }

    // Note: It is common sense to use if statements to identify if a new configuration has been set via MQTT
    //       It helps to set only the new configuration and to confirm the change and decrease the time to execute the loop
    if (this->isNewCustomColor()) {
        // Do something with the new color
        this->customColor = this->newCustomColor;

        // You can use the "setHexColor" method to set the color
        this->controllerService->setHexColor(this->customColor);

        // You have to confirm every new configuration if it took place
        this->controllerService->confirmLedConfig();
    }

    if (this->isNewCustomBrightness()) {
        // Do something with the new brightness
        this->customBrightness = this->newCustomBrightness;

        // You can use the "setBrightness" method to set the brightness
        this->controllerService->setBrightness(this->brightness);

        // You have to confirm every new configuration if it took place
        this->controllerService->confirmLedConfig();
    }

    // Tipp: Use the "steps" parameter to create some animations.
    //       You can also define your own counter with can be updated in every x-th loop iteration
}

// This method is called when a new color is set via MQTT
void CustomMode::customColorCallback(const String &color) {
    // The new color is stored in the "newCustomColor" property
    this->newCustomColor = color;

    // You can use the "pushCustomColor" method to acknowledge the change
    // >>> This is common sense to confirm every new configuration and this helps other devices to synchronize with the new configuration
    this->pushCustomColor(this->newCustomColor);
}

// This method is called when a new brightness is set via MQTT
void CustomMode::customBrightnessCallback(const byte &brightness) {
    // The new brightness is stored in the "newCustomBrightness" property
    this->newCustomBrightness = brightness;

    // You can use the "pushCustomBrightness" method to acknowledge the change
    this->pushCustomBrightness(this->newCustomBrightness);
}

// This method is used to identify if the color has changed
bool CustomMode::isNewCustomColor() {
    return this->customColor != this->newCustomColor;
}

// This method is used to identify if the brightness has changed
bool CustomMode::isNewCustomBrightness() {
    return this->customBrightness != this->newCustomBrightness;
}
```

4. Include the new mode in the `main.cpp` file.
```cpp
//…

// custom modes
//…
#include "CustomMode.h"
//…

// This is the main setup method of the ESP32
void setup() {

    //…
    // ----> SETUP YOUR APP HERE <----
    //…
    // This is the place where you can define your custom modes
    AbstractMode* customMode = new CustomMode(&controllerService);
    //…

    // setup modes
    //…
    // This calls the "setup" method of the "AbstractMode" class and then your defined "customSetup" method
    customMode->setup();
    //…
    // <---- SETUP YOUR APP HERE ---->

    //…
}

//…
```
5. Build, upload, and monitor the code using the Makefile commands ([see above](#makefile)).

## Services and classes
- **ClockService**: Provides timekeeping functionality (including NTP synchronization).
- **LoggingService**: Logs messages to the serial monitor and MQTT.
- **MQTTService**: Handles MQTT communication with the broker.
- **LedService**: Controls the LED strip using PWM signals.
- **ControllerService**: Manages tasks and schedules for the ESP32.

![Services Overview](media/images/classes-overview.png)

## Troubleshooting
- Make sure the correct port is selected in the Makefile.
- Ensure PlatformIO is installed correctly by checking the installation instructions linked above.

## Acknowledgements
- PlatformIO for providing a versatile environment for embedded development.
- MQTT protocol for enabling efficient communication between devices.

## Author
- [Friedjof Noweck](https://github.com/Friedjof)
