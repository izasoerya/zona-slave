#include <Arduino.h>
#include "connection_wifi.h"
#include "ambient_light.h"
#include "models.h"
#include "modbus/modbus.h"

ModbusObject temperature = {SensorType::TEMPERATURE, 1, 9600, 0x0001, ModbusCommandType::INPUT_REGISTER};
ModbusObject humidity = {SensorType::HUMIDITY, 1, 9600, 0x0002, ModbusCommandType::INPUT_REGISTER};
ModbusObject anemoMeter = {SensorType::ANEMOMETER, 1, 4800, 0x0001, ModbusCommandType::HOLDING_REGISTER};
ModbusObject ammonia = {SensorType::AMMONIA, 1, 4800, 0x0001, ModbusCommandType::HOLDING_REGISTER};

Modbus *modbus = nullptr;
DFRobot_B_LUX_V30B light;
WiFiConnection wifi;

void setup()
{
	Serial.begin(9600);
	wifi.begin();

	static ModbusObject modbusObject[] = {temperature, humidity, ammonia};
	modbus = new Modbus(modbusObject, (sizeof(modbusObject) / sizeof(ModbusObject)));
	modbus->begin();
	light.begin();
}

void loop()
{
	wifi.reconnectMQTT();

	float temp = modbus->readSingle(&temperature);
	float hum = modbus->readSingle(&humidity);
	float nh3 = modbus->readSingle(&ammonia);
	uint16_t anemo = modbus->readSingle(&ammonia);
	uint16_t lux = light.lightStrengthLux();
	SensorData sensor = {temp, hum, nh3, lux, anemo};
	String payload = wifi.publishMQTT(sensor);
	wifi.reconnect();

	delay(5000);
}
