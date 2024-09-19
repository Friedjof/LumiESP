# Variablen
PORT = /dev/ttyUSB0
BOARD = esp32dev
SPEED = 115200

# Befehle
BUILD_DIR = .pio/build/$(BOARD)
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

activate:
	source /home/friedjof/.platformio/penv/bin/activate

# Standardziel
.PHONY: all build upload monitor clean