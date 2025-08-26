#include <Arduino.h>
#include <Wire.h>

#include "connection_wifi.h"
#include "ambient_light.h"
#include "models.h"
#include "communication/modbus.h"
#include "communication/object_declaration.h"

ModbusObject modbusObject[] = {ammonia, anemoMeter, temperature, humidity};
Modbus modbus(modbusObject, (sizeof(modbusObject) / sizeof(ModbusObject)));
DFRobot_B_LUX_V30B light(13);
WiFiConnection wifi;
bool lightPresent = false;

void setup()
{
	Serial.begin(115200);
	Serial2.begin(9600);
	wifi.begin();

	modbus.begin();
	Wire.begin();
	Wire.beginTransmission(0x94 >> 1); // 0x94 is 8-bit, shift right for 7-bit address
	if (Wire.endTransmission() == 0)
	{
		light.begin();
		lightPresent = true;
		Serial.println("Light sensor found.");
	}
	else
	{
		Serial.println("Light sensor NOT found at 0x94.");
	}
	Serial.println("Setup Completed\n");
}

void loop()
{
	wifi.reconnectMQTT();

	uint16_t anemoMeterValue = modbus.readSingle(anemoMeter);
	float ammoniaValue = modbus.readSingle(ammonia) / 10.0F;
	float temperatureValue = modbus.readSingle(temperature) / 10.0F;
	float humidityValue = modbus.readSingle(humidity) / 10.0F;
	uint16_t lux = lightPresent ? light.lightStrengthLux() : 226;

	SensorData sensor = {temperatureValue, humidityValue, ammoniaValue, lux, anemoMeterValue};
	Serial.printf("Data: %s\n", sensor.toString().c_str());
	String payload = wifi.publishMQTT(sensor);
	wifi.reconnect();

	delay(15000);
}

/**
 * @brief
 * USE THIS FOR SETTING MODBUS DEVICE,
 * SUCH AS BAUDRATE, SLAVEID, ETC.
 */

// #include <Arduino.h>
// #include <ModbusMaster.h>

// ModbusMaster node;

// #define RS485_DIR 23
// #define RS485_DID 25

// void preTransmission();
// void postTransmission();

// void setup()
// {
// 	Serial.begin(115200); // Debug
// 	Serial2.begin(9600);  // RS485 bus

// 	pinMode(RS485_DIR, OUTPUT);
// 	pinMode(RS485_DID, OUTPUT);
// 	digitalWrite(RS485_DIR, LOW);
// 	digitalWrite(RS485_DID, LOW);

// 	node.begin(2, Serial2); // 2 = slave address (update accordingly)
// 	node.preTransmission(preTransmission);
// 	node.postTransmission(postTransmission);

// 	delay(1000); // Allow slave to boot
// }

// void loop()
// {
// 	uint8_t result, resultWrite;
// 	uint16_t data;

// 	// resultWrite = node.writeSingleRegister(0x07D1, 2);
// 	// if (resultWrite == node.ku8MBSuccess)
// 	// {
// 	// 	data = node.getResponseBuffer(0x00); // first register
// 	// 	Serial.print("Register 0x07D1 = ");
// 	// 	Serial.println(data);
// 	// }
// 	// else
// 	// {
// 	// 	Serial.print("Read failed, error: ");
// 	// 	Serial.println(resultWrite, HEX);
// 	// }

// 	result = node.readHoldingRegisters(0x000, 1);

// 	if (result == node.ku8MBSuccess)
// 	{
// 		data = node.getResponseBuffer(0x00); // first register
// 		Serial.print("Register 0x07D1 = ");
// 		Serial.println(data);
// 	}
// 	else
// 	{
// 		Serial.print("Read failed, error: ");
// 		Serial.println(result, HEX);
// 	}

// 	delay(3000);
// }

// void preTransmission()
// {
// 	digitalWrite(RS485_DIR, HIGH); // TX mode
// 	digitalWrite(RS485_DID, HIGH); // TX mode
// }

// void postTransmission()
// {
// 	digitalWrite(RS485_DIR, LOW); // RX mode
// 	digitalWrite(RS485_DID, LOW); // RX mode
// }
