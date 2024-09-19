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

flash: upload monitor

clean:
	$(PLATFORMIO) run --environment $(BOARD) --target clean

setup:
	python ./setup.py

help:
	@echo "Verfügbare Makefile Commands:"
	@echo "  setup    - Richtet die Entwicklungsumgebung ein"
	@echo "  all      - Baut das Projekt"
	@echo "  build    - Baut das Projekt"
	@echo "  upload   - Lädt das Projekt auf den ESP32"
	@echo "  monitor  - Öffnet ein Terminal zum ESP32"
	@echo "  clean    - Löscht das Projekt"
	@echo "  help     - Zeigt diese Hilfe an"

# Standardziel
.PHONY: all build upload monitor clean setup help
