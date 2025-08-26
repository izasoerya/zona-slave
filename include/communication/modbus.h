#pragma once

#include <ModbusMaster.h>
#include "enum.h"

struct ModbusObject
{
    SensorType sensor;
    uint8_t slaveId;
    uint16_t baudRate;
    uint16_t registerAddress;
    uint8_t lengthAddress;
    ModbusCommandType type;
};

class Modbus
{
private:
    static const uint8_t pinRE = 23;
    static const uint8_t pinDE = 25;
    uint8_t objectLength;
    ModbusObject object[10];
    ModbusMaster node[10];

    static void preTransmission()
    {
        digitalWrite(pinRE, 1);
        digitalWrite(pinDE, 1);
        delay(1);
    }

    static void postTransmission()
    {
        digitalWrite(pinRE, 0);
        digitalWrite(pinDE, 0);
        delay(1);
    }

public:
    Modbus(ModbusObject pObject[], uint8_t length);
    ~Modbus();

    void begin();
    float readSingle(ModbusObject object);
};

Modbus::Modbus(ModbusObject pObject[], uint8_t length)
{
    objectLength = length;
    Serial.printf("Length: %d", objectLength);
    for (uint8_t i = 0; i < length; i++)
    {
        object[i] = pObject[i];
    }
}

Modbus::~Modbus() {}

void Modbus::begin()
{
    pinMode(pinRE, OUTPUT);
    pinMode(pinDE, OUTPUT);
    digitalWrite(pinDE, LOW);
    digitalWrite(pinRE, LOW);

    for (int i = 0; i < objectLength; i++)
    {
        node[i].preTransmission(preTransmission);
        node[i].postTransmission(postTransmission);
        node[i].begin(object[i].slaveId, Serial2);
        Serial.printf(
            "Device sensor: %s, slaveId: %d, baud: %d, reg: %d, len: %d, type: %d\n",
            sensorTypeToString(object[i].sensor),
            object[i].slaveId,
            object[i].baudRate,
            object[i].registerAddress,
            object[i].lengthAddress,
            object[i].type);
    }
}

float Modbus::readSingle(ModbusObject obj)
{
    uint8_t result;
    for (int i = 0; i < objectLength; i++)
    {
        if (obj.slaveId == (i + 1))
        {
            if (obj.type == ModbusCommandType::INPUT_REGISTER)
            {
                Serial.printf(
                    "Read Input: %s, slaveId: %d, baud: %d, reg: %d, len: %d, type: %d || ",
                    sensorTypeToString(object[i].sensor),
                    object[i].slaveId,
                    object[i].baudRate,
                    object[i].registerAddress,
                    object[i].lengthAddress,
                    object[i].type);

                result = node[i].readInputRegisters(obj.registerAddress, obj.lengthAddress);
            }
            else if (obj.type == ModbusCommandType::HOLDING_REGISTER)
            {
                Serial.printf(
                    "Read Holding: %s, slaveId: %d, baud: %d, reg: %d, len: %d, type: %d || ",
                    sensorTypeToString(object[i].sensor),
                    object[i].slaveId,
                    object[i].baudRate,
                    object[i].registerAddress,
                    object[i].lengthAddress,
                    object[i].type);

                result = node[i].readHoldingRegisters(obj.registerAddress, obj.lengthAddress);
            }
            else
            {
                Serial.printf(
                    "Error Read: %s, slaveId: %d, baud: %d, reg: %d, len: %d, type: %d || ",
                    sensorTypeToString(object[i].sensor),
                    object[i].slaveId,
                    object[i].baudRate,
                    object[i].registerAddress,
                    object[i].lengthAddress,
                    object[i].type);
                return -68; // Unknown command type
            }
            if (result == node[i].ku8MBSuccess)
            {
                Serial.printf("Success: %d\n", node[i].getResponseBuffer(0));
                delay(10);
                return node[i].getResponseBuffer(0);
            }
            Serial.println("Error: No Device Found, Check Wiring and Code!");
            return result;
        }
    }
    Serial.println("Check the code dude");
    return -405; // What even is this, check objectLengh and mapping node to pObject
}