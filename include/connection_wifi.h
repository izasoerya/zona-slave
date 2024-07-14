#ifndef WIFI_H
#define WIFI_H

// Include any necessary libraries here
#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include "pin_definition.h"
#include <PubSubClient.h>

class WiFiConnection
{
public:
    WiFiConnection();
    ~WiFiConnection();

    void begin();
    void reconnect();
    void reconnectMQTT(PubSubClient &client);

private:
    const char *ssid = "Subhanallah";
    const char *password = "muhammadnabiyullah";
    const char *mqttServer = "mqtt.eclipse.org";
    uint16_t mqttPort = 8883;
    WiFiClientSecure wifiClient;
    PubSubClient mqttClient;
};

#endif // WIFI_H