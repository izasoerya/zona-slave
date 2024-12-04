#include <Arduino.h>
#include "connection_wifi.h"
#include "pin_definition.h"
#include "sht20.h"
#include "anemometer.h"
#include "mics.h"
#include "inmp.h"
#include "ambient_light.h"
#include "models.h"

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
  randomSeed(analogRead(0));
}

void loop()
{
  wifi.reconnectMQTT();

  // Fetch data from sensors if they are enabled
  if (data.sht20Enable)
    sensorSHT();
  if (data.micsEnable)
    sensorMICS();
  if (data.lightEnable)
    sensorLight();
  if (data.inmpEnable)
    sensorINMP();
  if (data.anemometerEnable)
    sensorAnemometer();

  // Debug and publish sensor data
  data.debugAll(Serial);
  wifi.publishMQTT(data);
  wifi.reconnect();

  // Delay to match the original task's interval (3 seconds)
  delay(3000);
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
