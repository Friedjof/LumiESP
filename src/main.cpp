#include <Arduino.h>

#include <cstdio>

#include <WiFi.h>
#include <PubSubClient.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

#include "../config/config.h"

// prototypes
void setup_wifi();
void sync_time();
void reconnect();
void logging(char* msg);

// WiFi and MQTT configuration
const char* ssid = WIFI_SSID;
const char* password = WIFI_PASSWORD;
const char* mqtt_server = MQTT_BROKER;
const int mqtt_port = MQTT_PORT;
const char* mqtt_user = MQTT_USERNAME;
const char* mqtt_password = MQTT_PASSWORD;
const char* mqtt_client_id = MQTT_CLIENT_ID;

// Create an instance of the WiFi and MQTT clients
WiFiClient espClient;
PubSubClient client(espClient);

// NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, NTP_SERVER);

unsigned long lastMsg = 0;


void setup() {
  Serial.begin(115200);

  setup_wifi();
  sync_time();

  client.setServer(mqtt_server, mqtt_port);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }

  client.loop();

  if (millis() - lastMsg > MQTT_DATETIME_UPDATE_INTERVAL) {
    lastMsg = millis();
    timeClient.update();
    char msg[100];
    snprintf(msg, 100, "%s", timeClient.getFormattedTime());
    client.publish(MQTT_DATETIME_TOPIC, msg);
  }
}


void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);
  WiFi.waitForConnectResult();

  #ifdef CUSTOM_DNS
  WiFi.config(WiFi.localIP(), WiFi.gatewayIP(), WiFi.subnetMask(), IPAddress(DNS_SERVER));
  #endif

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.print("DNS: ");
  Serial.println(WiFi.dnsIP());
}

void sync_time() {
  timeClient.begin();
  timeClient.setTimeOffset(NTP_OFFSET);

  Serial.println("Synchronizing time with NTP server...");
  while (!timeClient.update()) {
    Serial.println("Failed to get time, retrying...");
    timeClient.forceUpdate();
    delay(1000);
  }
  Serial.print("Time synchronized: ");
  Serial.println(timeClient.getFormattedTime());
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(mqtt_client_id, mqtt_user, mqtt_password)) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish(MQTT_STATUS_TOPIC, "connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void logging(char* msg) {
    if (Serial) {
        Serial.println(msg);
    } else {
        if (client.connected()) {
            client.publish(MQTT_LOG_TOPIC, msg);
        }
    }
}