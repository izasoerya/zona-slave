#include "connection_wifi.h"

WiFiConnection::WiFiConnection() : wifiClient(WiFiClient()), mqttClient(PubSubClient(wifiClient)), jsonDoc(JsonDocument()) {}

WiFiConnection::~WiFiConnection() {}

void WiFiConnection::begin()
{
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(1000);
        Serial.println("Connecting to WiFi..");
    }
    Serial.print("Connected to WiFi with IP: ");
    Serial.println(WiFi.localIP());
    mqttClient.setServer(mqttServer, mqttPort);
}

String WiFiConnection::publishMQTT(SensorData sensorData)
{
    String finalData;
    if (sensorData.anemometerEnable)
    {
        jsonDoc["wind_speed"] = (sensorData.windSpeed);
    }
    else
    {
        jsonDoc["temperature"] = (sensorData.temperature);
        jsonDoc["humidity"] = (sensorData.humidity);
        jsonDoc["ammonia"] = (sensorData.nh3);
        jsonDoc["light_intensity"] = (sensorData.lux);
    }
    mqttClient.beginPublish(mqttTopic, measureJson(jsonDoc), 0);
    serializeJson(jsonDoc, mqttClient);
    mqttClient.endPublish();
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
        if (mqttClient.connect("/farm/3a99e243-2c69-447d-ba7d-30d2d126724c/sensor"))
        {
            Serial.println("connected");
        }
        else
        {
            Serial.print("failed, rc=");
            Serial.print(mqttClient.state());
            Serial.println(" try again in 5 seconds");
            delay(5000);
        }
    }
}