#include <Arduino.h>
#include "connection_wifi.h"
#include "ambient_light.h"
#include "models.h"
#include "modbus/modbus.h"

ModbusObject temperature = {SensorType::TEMPERATURE, 1, 9600, 0x0001, ModbusCommandType::INPUT_REGISTER};
ModbusObject humidity = {SensorType::HUMIDITY, 1, 9600, 0x0002, ModbusCommandType::INPUT_REGISTER};
ModbusObject anemoMeter = {SensorType::ANEMOMETER, 1, 4800, 0x0001, ModbusCommandType::HOLDING_REGISTER};
ModbusObject ammonia = {SensorType::AMMONIA, 1, 4800, 0x0001, ModbusCommandType::HOLDING_REGISTER};

AmbientLight light;
WiFiConnection wifi;
SensorData data;
Modbus *modbus = nullptr;

void setup()
{
  Serial.begin(9600);
  wifi.begin();
  ModbusObject modbusObject[] = {temperature, humidity, ammonia};
  modbus = new Modbus(modbusObject, (sizeof(modbusObject) / sizeof(ModbusObject)));
  modbus->begin();

  if (data.lightEnable)
    light.begin();
}

void loop()
{
  wifi.reconnectMQTT();

  float temp = modbus->readSingle(&temperature);

  wifi.publishMQTT(data);
  wifi.reconnect();

  delay(5000);
}
