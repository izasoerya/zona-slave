#include "connection_wifi.h"

WiFiConnection::WiFiConnection() : wifiClient(WiFiClient()), mqttClient(PubSubClient(wifiClient)), jsonDoc(JsonDocument())
{
    mqttTopic = SensorData().anemometerEnable ? mqttTopicAnemo : mqttTopicMain;
}

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
        jsonDoc["temperature"] = round(sensorData.temperature * 100) / 100.0;
        jsonDoc["humidity"] = round(sensorData.humidity * 100) / 100.0;
        jsonDoc["ammonia"] = round(sensorData.nh3 * 100) / 100.0;
        jsonDoc["light_intensity"] = round(sensorData.lux * 100) / 100.0;
    }
    mqttClient.beginPublish(mqttTopicMain, measureJson(jsonDoc), 0);
    serializeJson(jsonDoc, mqttClient);
    mqttClient.endPublish();
    Serial.println("Data published to MQTT");
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