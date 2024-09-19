# LumiMQTT
LumiMQTT is a project that uses an ESP32 microcontroller to control a 12V LED strip via MQTT. The project is designed to provide flexible and remote control over lighting, allowing integration into home automation systems.

## Features
- Control 12V LED strip using ESP32 over MQTT
- Customizable settings for port, board, and speed
- Easy build, upload, and monitor using PlatformIO commands

## Requirements
- ESP32 microcontroller (e.g., ESP32 Dev Module)
- PlatformIO installed on your system
- LED strip (12V)
- MQTT broker (e.g., Mosquitto)

## Installation
### 1. Install PlatformIO
PlatformIO is a cross-platform code builder and the easiest way to manage your embedded software environment. To install PlatformIO, follow the official instructions found [here](https://docs.platformio.org/en/latest/core/installation/methods/installer-script.html).

### 2. Clone the Repository
Clone the LumiMQTT project repository to your local machine:

```bash
git clone https://github.com/yourusername/LumiMQTT.git
cd LumiMQTT
```

### 3. Build, Upload, and Monitor
The project includes a Makefile that simplifies the use of PlatformIO commands. Make sure to edit the variables in the Makefile according to your setup.

#### Variables
- `PORT`: Serial port of your ESP32 (e.g., `/dev/ttyUSB0`).
- `BOARD`: The type of ESP32 board (e.g., `esp32dev`).
- `SPEED`: Baud rate for the serial monitor (e.g., `115200`).

#### Commands
- **Build the project:**

  ```bash
  make build
  ```

- **Upload the code to the ESP32:**

  ```bash
  make upload
  ```

- **Monitor the serial output:**

  ```bash
  make monitor
  ```

- **Clean the build files:**

  ```bash
  make clean
  ```

## Troubleshooting
- Make sure the correct port is selected in the Makefile.
- Ensure PlatformIO is installed correctly by checking the installation instructions linked above.

## Acknowledgements
- PlatformIO for providing a versatile environment for embedded development.
- MQTT protocol for enabling efficient communication between devices.