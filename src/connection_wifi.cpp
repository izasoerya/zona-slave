#include "connection_wifi.h"

WiFiConnection::WiFiConnection()
    : wifiClient(WiFiClient()), mqttClient(PubSubClient(wifiClient)), jsonDoc(JsonDocument()) {}

WiFiConnection::~WiFiConnection() {}

void WiFiConnection::begin()
{
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    _counterReset = 0;
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        _counterReset++;
        if (_counterReset > 30)
        {
            Serial.println("Failed to connect to WiFi, resetting ESP32...");
            ESP.restart();
        }
        Serial.println("Connecting to WiFi..");
    }
    Serial.print("Connected to WiFi with IP: ");
    _counterReset = 0;
    Serial.println(WiFi.localIP());
    mqttClient.setServer(mqttServer, mqttPort);
}

String WiFiConnection::publishMQTT(SensorData data)
{
    String finalData;
    if (data.windSpeed != 226)
    {
        jsonDoc["wind_speed"] = (data.windSpeed);
        mqttClient.beginPublish(mqttTopicAnemo, measureJson(jsonDoc), 0);
        serializeJson(jsonDoc, mqttClient);
        mqttClient.endPublish();

        String payload;
        serializeJson(jsonDoc, payload);
        Serial.printf("\nData published to MQTT: %s\n", payload.c_str());
    }
    else
    {
        jsonDoc["temperature"] = round(data.temperature * 10) / 10.0;
        jsonDoc["humidity"] = round(data.humidity * 10) / 10.0;
        jsonDoc["ammonia"] = round(data.nh3 * 10) / 10.0;
        jsonDoc["light_intensity"] = (data.lux);
        mqttClient.beginPublish(mqttTopicMain, measureJson(jsonDoc), 0);
        serializeJson(jsonDoc, mqttClient);
        mqttClient.endPublish();

        String payload;
        serializeJson(jsonDoc, payload);
        Serial.printf("\nData published to MQTT: %s\n", payload.c_str());
    }
    return finalData;
}

void WiFiConnection::reconnect()
{
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(1000);
        Serial.println("Connecting to WiFi..");
    }
}

void WiFiConnection::reconnectMQTT()
{
    while (!mqttClient.connected())
    {
        Serial.print("Attempting MQTT connection...");
        if (mqttClient.connect(mqttTopicMain))
        {
            _counterReset = 0; // Reset the counter on successful connection
            Serial.println("connected");
        }
        else
        {
            if (_counterReset > 5)
            {
                Serial.println("Failed to connect to MQTT broker, resetting ESP32...");
                ESP.restart();
            }
            _counterReset++;
            Serial.print("failed, rc=");
            Serial.print(mqttClient.state());
            Serial.println(" try again in 5 seconds");
            delay(5000);
        }
    }
}