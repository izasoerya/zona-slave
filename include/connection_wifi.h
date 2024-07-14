#ifndef WIFI_H
#define WIFI_H

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "pin_definition.h"
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
    const char *ssid = "faizareborn";
    const char *password = "herbowo221977";
    const char *mqttServer = "k91ed63b.ala.us-east-1.emqxsl.com";
    const char *mqttTopic = "/sensor-data";
    const uint8_t nodeID = 0;
    uint16_t mqttPort = 8883;
    WiFiClientSecure wifiClient;
    PubSubClient mqttClient;
    JsonDocument jsonDoc;
};

#endif // WIFI_H