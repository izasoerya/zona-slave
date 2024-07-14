#include <Arduino.h>
#include "connection_wifi.h"
#include "pin_definition.h"
#include "sht20.h"
#include "mics.h"
#include "inmp.h"
#include "ambient_light.h"
#include "models.h"

void taskFetchSensors(void *pvParameters);
// void taskSampling(void *pvParameters);
TaskHandle_t *_handlerFetchSensors;
// TaskHandle_t *_handlerSampling;

void sensorSHT();
void sensorMICS();
void sensorLight();
void sensorINMP();

SHT20 sht;
MICS6814 mics;
AmbientLight light;
INMP441 inmp;
WiFiConnection wifi;
// WiFiClientSecure espClient;
// PubSubClient mqttClient(espClient);
SensorData data;

void setup()
{
  Serial.begin(9600);
  Modbus.begin(9600);
  wifi.begin();

  pinMode(RE_MODBUS, OUTPUT);
  pinMode(DE_MODBUS, OUTPUT);
  sht.begin();
  light.begin();
  inmp.begin();
  // espClient.setInsecure();
  // const char *mqttServer = "k91ed63b.ala.us-east-1.emqxsl.com";
  // mqttClient.setServer(mqttServer, 8883);
  // const char *mqttTopic = "sensor-data";
  // mqttClient.subscribe(mqttTopic);

  // Here i use RTOS just in case need multithreading
  // add more task if needed
  xTaskCreate(taskFetchSensors, "all sensor", 20000, NULL, 1, _handlerFetchSensors);
  // xTaskCreate(taskSampling, "inmp", 2048, NULL, 1, _handlerSampling);
}

// void reconnectMQTT()
// {
//   while (!mqttClient.connected())
//   {
//     Serial.println("Connecting to MQTT broker...");
//     if (mqttClient.connect("esp", "device_1", "device_1_admin"))
//     {
//       Serial.println("Connected to MQTT broker");
//     }
//     else
//     {
//       Serial.print("Failed to connect to MQTT broker, rc=");
//       Serial.print(mqttClient.state());
//       Serial.println(" retrying in 5 seconds");
//       delay(3000);
//     }
//   }
// }

void taskFetchSensors(void *pvParameters)
{
  while (true)
  {
    wifi.reconnectMQTT();
    if (data.sht20Enable)
      sensorSHT();
    if (data.micsEnable)
      sensorMICS();
    if (data.lightEnable)
      sensorLight();
    if (data.inmpEnable)
      sensorINMP();

    data.debugAll(Serial);
    wifi.publishMQTT(data);
    wifi.reconnect();

    vTaskDelay(1000);
  }
}

// void taskSampling(void *pvParameters)
// {
//   while (true)
//   {
//     if (data.inmpEnable)
//       sensorINMP();

//     vTaskDelay(1000);
//   }
// }

void sensorSHT()
{
  data.temperature = sht.readTemperature();
  data.humidity = sht.readHumidity();
}

void sensorMICS()
{
  data.no2 = mics.readNO2();
  data.nh3 = mics.readNH3();
  data.co2 = mics.readCO();
}

void sensorLight()
{
  data.lux = light.readLightLevel();
}

void sensorINMP()
{
  // data.frequencyLength = sizeof(inmp.read()) / sizeof(inmp.read()[0]);
  data.frequencyData = inmp.read();
}

void loop() { vTaskDelete(NULL); }