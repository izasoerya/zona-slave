#pragma once

#include <ModbusMaster.h>
#include "enum.h"

struct ModbusObject
{
    SensorType sensor;
    uint8_t id;
    uint16_t baudRate;
    uint16_t registerAddress;
    ModbusCommandType type;
};

class Modbus
{
private:
    static const uint8_t pinRE = 23;
    static const uint8_t pinDE = 25;
    ModbusObject *object[10];
    uint8_t objectLength;
    ModbusMaster modbus;

    static void preTransmission()
    {
        digitalWrite(pinRE, 1);
        digitalWrite(pinDE, 1);
        delay(5);
    }

    static void postTransmission()
    {
        digitalWrite(pinRE, 0);
        digitalWrite(pinDE, 0);
        delay(5);
    }

public:
    Modbus(ModbusObject **pObject, uint8_t length);
    ~Modbus();

    void begin();
    float readSingle(ModbusObject *object);
};

Modbus::Modbus(ModbusObject **pObject, uint8_t length)
{
    objectLength = length;
    for (uint8_t i = 0; i < length; ++i)
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

    modbus.preTransmission(preTransmission);
    modbus.postTransmission(postTransmission);

    for (int i = 0; i < objectLength; i++)
    {
        if (object[i] != nullptr)
        {
            Serial2.begin(object[i]->baudRate);
            break; // Exit after finding the first valid object
        }
    }
}

float Modbus::readSingle(ModbusObject *obj)
{
    if (obj == nullptr)
    {
        return -69; // prevent null pointer dereference
    }

    ModbusObject *selectedObject = nullptr;

    for (int i = 0; i < objectLength; i++)
    {
        if (object[i] == nullptr)
        {
            continue;
        }

        if (obj->sensor == object[i]->sensor)
        {
            selectedObject = object[i];
            break;
        }
    }

    if (selectedObject == nullptr)
    {
        return -69;
    }

    modbus.begin(selectedObject->id, Serial2);

    uint8_t result;
    if (selectedObject->type == ModbusCommandType::INPUT_REGISTER)
    {
        result = modbus.readInputRegisters(selectedObject->registerAddress, 1);
    }
    else if (selectedObject->type == ModbusCommandType::HOLDING_REGISTER)
    {
        result = modbus.readHoldingRegisters(selectedObject->registerAddress, 1);
    }
    else
    {
        return -68; // Unknown command type
    }

    if (result == modbus.ku8MBSuccess)
    {
        return modbus.getResponseBuffer(0);
    }
    else
    {
        return -70 - result; // return error indicator
    }
}
