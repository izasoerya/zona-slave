#include "connection_wifi.h"

WiFiConnection::WiFiConnection() : wifiClient(WiFiClientSecure()), mqttClient(PubSubClient(wifiClient)) {}

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
    mqttClient.setServer(mqttServer, mqttPort);
    mqttClient.subscribe("/sensor-data");
}

void WiFiConnection::reconnect()
{
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(1000);
        Serial.println("Connecting to WiFi..");
    }
}

void WiFiConnection::reconnectMQTT(PubSubClient &client)
{
    while (!client.connected())
    {
        Serial.println("Connecting to MQTT broker...");
        if (client.connect("ESP32Client"))
        {
            Serial.println("Connected to MQTT broker");
        }
        else
        {
            Serial.print("Failed to connect to MQTT broker, rc=");
            Serial.print(client.state());
            Serial.println(" retrying in 5 seconds");
            delay(3000);
        }
    }
}