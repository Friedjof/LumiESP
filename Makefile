# Variablen
PORT = /dev/ttyUSB0
BOARD = esp32dev
SPEED = 115200
PLATFORMIO = ~/.platformio/penv/bin/platformio


# Ziele
all: build

build:
	$(PLATFORMIO) run --environment $(BOARD)

upload:
	$(PLATFORMIO) run --environment $(BOARD) --target upload --upload-port $(PORT)

monitor:
	$(PLATFORMIO) device monitor --port $(PORT) --baud $(SPEED)

clean:
	$(PLATFORMIO) run --environment $(BOARD) --target clean

flash: upload monitor

setup:
	python ./setup.py

help:
	@echo "Available Makefile Commands:"
	@echo "  setup    - Sets up the development environment"
	@echo "  all      - Builds the project"
	@echo "  build    - Builds the project"
	@echo "  upload   - Uploads the project to the ESP32"
	@echo "  monitor  - Opens a terminal to the ESP32"
	@echo "  flash    - Uploads the project to the ESP32 and opens a terminal"
	@echo "  clean    - Cleans the project"
	@echo "  help     - Displays this help message"

# Standardziel
.PHONY: all build upload monitor clean setup help
