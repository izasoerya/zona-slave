#include <Arduino.h>
#include "connection_wifi.h"
#include "ambient_light.h"
#include "models.h"
#include "modbus/modbus.h"

ModbusObject temperature = {SensorType::TEMPERATURE, 3, 9600, 0x0001, ModbusCommandType::INPUT_REGISTER};
ModbusObject humidity = {SensorType::HUMIDITY, 3, 9600, 0x0002, ModbusCommandType::INPUT_REGISTER};
ModbusObject anemoMeter = {SensorType::ANEMOMETER, 1, 4800, 0x0001, ModbusCommandType::HOLDING_REGISTER};
ModbusObject ammonia = {SensorType::AMMONIA, 1, 9600, 0x07D1, ModbusCommandType::HOLDING_REGISTER};

Modbus *modbus = nullptr;
DFRobot_B_LUX_V30B light(13);
WiFiConnection wifi;
ModbusObject modbusObject[] = {temperature, humidity};

void setup()
{
	Serial.begin(115200);
	wifi.begin();

	modbus = new Modbus(modbusObject, (sizeof(modbusObject) / sizeof(ModbusObject)));
	modbus->begin();
	light.begin();
}

void loop()
{
	wifi.reconnectMQTT();

	float temp = modbus->readSingle(&modbusObject[1]) / 10.0F;
	float hum = modbus->readSingle(&modbusObject[2]) / 10.0F;
	float nh3 = modbus->readSingle(&modbusObject[0]);
	uint16_t anemo = 0; // modbus->readSingle(&anemoMeter);
	uint16_t lux = light.lightStrengthLux();
	SensorData sensor = {temp, hum, nh3, lux, anemo};
	String payload = wifi.publishMQTT(sensor);
	wifi.reconnect();

	delay(5000);
}

//
// #include <Arduino.h>
// // Optional: define RS485 DE/RE control pin if your module requires it
// #define RS485_DIR 4 // Change to your DE/RE pin if needed

// uint16_t modbusCRC(uint8_t *buf, uint8_t len);

// void setup()
// {
// 	Serial.begin(115200); // Debug output
// 	Serial2.begin(4800);  // RS485 Modbus line

// 	pinMode(RS485_DIR, OUTPUT);
// 	digitalWrite(RS485_DIR, LOW); // Initially receive mode

// 	delay(1000); // Allow slave to boot

// 	// Build Modbus RTU frame: [SlaveID][Function][AddrHi][AddrLo][ValHi][ValLo][CRCLo][CRCHi]
// 	uint8_t frame[8];
// 	frame[0] = 0x01; // Slave address
// 	frame[1] = 0x06; // Function code: Write Single Register
// 	frame[2] = 0x07; // Register high byte
// 	frame[3] = 0xD1; // Register low byte (0x0066)
// 	frame[4] = 0x00; // Value high byte
// 	frame[5] = 0x02; // Value low byte (new slave ID = 3)

// 	uint16_t crc = modbusCRC(frame, 6);
// 	frame[6] = crc & 0xFF;		  // CRC low byte
// 	frame[7] = (crc >> 8) & 0xFF; // CRC high byte

// 	// Switch RS485 to transmit
// 	digitalWrite(RS485_DIR, HIGH);
// 	delay(2); // RS485 driver settle time

// 	for (int i = 0; i < 8; i++)
// 	{
// 		Serial2.write(frame[i]);
// 		Serial.print("0x");
// 		Serial.print(frame[i], HEX);
// 		Serial.print(" ");
// 	}

// 	Serial.println("\nSent Modbus request to change address to 3");

// 	Serial2.flush(); // Ensure all bytes sent
// 	delay(2);		 // Allow slave to finish receiving

// 	digitalWrite(RS485_DIR, LOW); // Switch RS485 back to receive
// }

// void loop()
// {
// 	// Nothing in loop for now
// }

// // CRC calculation (Modbus RTU CRC-16, LSB first)
// uint16_t modbusCRC(uint8_t *buf, uint8_t len)
// {
// 	uint16_t crc = 0xFFFF;
// 	for (int pos = 0; pos < len; pos++)
// 	{
// 		crc ^= (uint16_t)buf[pos];
// 		for (int i = 0; i < 8; i++)
// 		{
// 			if ((crc & 0x0001) != 0)
// 			{
// 				crc >>= 1;
// 				crc ^= 0xA001;
// 			}
// 			else
// 			{
// 				crc >>= 1;
// 			}
// 		}
// 	}
// 	return crc;
// }
