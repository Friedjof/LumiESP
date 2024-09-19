#include <Arduino.h>

#include "config/config.h"


void setup() {
  Serial.begin(115200);

}

void loop() {
  Serial.println("Hello World NEW");
  delay(1000);
}
