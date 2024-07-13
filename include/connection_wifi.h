#ifndef WIFI_H
#define WIFI_H

// Include any necessary libraries here
#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include "pin_definition.h"

class WiFiConnection
{
public:
    WiFiConnection();
    ~WiFiConnection();

    void begin();
    void reconnect();

private:
    const char *ssid = "Subhanallah";
    const char *password = "muhammadnabiyullah";
};

#endif // WIFI_H