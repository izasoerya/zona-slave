#ifndef WIFI_H
#define WIFI_H

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "pin_definition.h"
#include "env.h"
#include "models.h"

class WiFiConnection
{
public:
    WiFiConnection();
    ~WiFiConnection();

    void begin();
    void reconnect();
    void reconnectMQTT();
    String publishMQTT(SensorData data);

private:
    const char *ssid = env_ssid;
    const char *password = env_password;
    const char *mqttServer = env_mqttServer;
    const char *mqttTopic;
    const char *mqttTopicMain = "device/3a99e243-2c69-447d-ba7d-30d2d126724c/sensor/create";
    const char *mqttTopicAnemo = "device/e8499855-ae64-473a-85bb-024c9177287e/sensor/create";
    const uint8_t nodeID = 0;
    uint16_t mqttPort = 1883;
    WiFiClient wifiClient;
    PubSubClient mqttClient;
    JsonDocument jsonDoc;
};

#endif // WIFI_H