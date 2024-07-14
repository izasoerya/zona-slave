#include "connection_wifi.h"

WiFiConnection::WiFiConnection() : wifiClient(WiFiClientSecure()), mqttClient(PubSubClient(wifiClient)), jsonDoc(JsonDocument()) {}

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

    wifiClient.setInsecure();
    mqttClient.setServer(mqttServer, mqttPort);
    mqttClient.subscribe(mqttTopic);
}

String WiFiConnection::publishMQTT(SensorData sensorData)
{
    String finalData;
    jsonDoc["id"] = nodeID;
    jsonDoc["temp"] = String(sensorData.temperature, 2);
    jsonDoc["hum"] = String(sensorData.humidity, 2);
    jsonDoc["nh3"] = String(sensorData.nh3, 2);
    jsonDoc["lux"] = String(sensorData.lux);
    JsonArray freq = jsonDoc["freq"].to<JsonArray>();
    for (int i = 0; i < 64; i++)
        freq.add(sensorData.frequencyData.frequency[i]);

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
        Serial.println("Connecting to MQTT broker...");
        if (mqttClient.connect("esp", "device_1", "device_1_admin"))
        {
            Serial.println("Connected to MQTT broker");
        }
        else
        {
            Serial.print("Failed to connect to MQTT broker, rc=");
            Serial.print(mqttClient.state());
            Serial.println(" retrying in 5 seconds");
            delay(3000);
        }
    }
}