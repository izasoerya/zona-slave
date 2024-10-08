#include <Arduino.h>
#include "connection_wifi.h"
#include "pin_definition.h"
#include "sht20.h"
#include "anemometer.h"
#include "mics.h"
#include "inmp.h"
#include "ambient_light.h"
#include "models.h"

void taskFetchSensors(void *pvParameters);
TaskHandle_t *_handlerFetchSensors;

void sensorSHT();
void sensorMICS();
void sensorLight();
void sensorINMP();
void sensorAnemometer();

SHT20 sht;
Anemometer anemometer;
MICS6814 mics;
AmbientLight light;
INMP441 inmp;
WiFiConnection wifi;
SensorData data;

void setup()
{
  Serial.begin(9600);
  uint16_t baudModbus = data.anemometerEnable ? 4800 : 9600;
  Modbus.begin(baudModbus);
  wifi.begin();

  pinMode(RE_MODBUS, OUTPUT);
  pinMode(DE_MODBUS, OUTPUT);
  if (data.sht20Enable)
    sht.begin();
  if (data.anemometerEnable)
    anemometer.begin();
  if (data.lightEnable)
    light.begin();
  if (data.inmpEnable)
    inmp.begin();

  // Here i use RTOS just in case need multithreading
  // add more task if needed
  xTaskCreate(taskFetchSensors, "all sensor", 20000, NULL, 1, _handlerFetchSensors);
}

void taskFetchSensors(void *pvParameters)
{
  while (true)
  {
    wifi.reconnectMQTT();
    if (data.sht20Enable) //! ENABLE/DISABLE (default true) SENSOR
      sensorSHT();
    if (data.micsEnable) //! ENABLE/DISABLE (default true) SENSOR
      sensorMICS();
    if (data.lightEnable) //! ENABLE/DISABLE (default true) SENSOR
      sensorLight();
    if (data.inmpEnable) //! ENABLE/DISABLE (default true) SENSOR
      sensorINMP();
    if (data.anemometerEnable) //! ENABLE/DISABLE (default true) SENSOR
      sensorAnemometer();

    data.debugAll(Serial);
    wifi.publishMQTT(data);
    wifi.reconnect();

    vTaskDelay(SensorData().anemometerEnable ? 3000 / portTICK_PERIOD_MS : 3000 / portTICK_PERIOD_MS);
  }
}

void sensorSHT()
{
  data.temperature = sht.readTemperature();
  data.humidity = sht.readHumidity();
}

void sensorAnemometer()
{
  data.windSpeed = anemometer.readWindSpeed();
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
  data.frequencyData = inmp.read();
}

void loop() { vTaskDelete(NULL); }