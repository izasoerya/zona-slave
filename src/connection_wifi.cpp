#include "connection_wifi.h"

WiFiConnection::WiFiConnection() {}

WiFiConnection::~WiFiConnection() {}

void WiFiConnection::begin()
{
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(1000);
        Serial.println("Connecting to WiFi..");
    }
    Serial.print("Connected to WiFi with IP: ");
    Serial.println(WiFi.localIP());
    WiFiClientSecure client;
    client.setInsecure(); // use setCACert() if you have a certificate
}

void WiFiConnection::reconnect()
{
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(1000);
        Serial.println("Connecting to WiFi..");
    }
}