#pragma once

#include <ModbusMaster.h>
#include "enum.h"

struct ModbusObject
{
    SensorType sensor;
    uint8_t id;
    uint16_t baudRate;
    uint8_t registerAddress;
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
    }

    static void postTransmission()
    {
        digitalWrite(pinRE, 0);
        digitalWrite(pinDE, 0);
    }

public:
    Modbus(ModbusObject *pObject, uint8_t length);
    ~Modbus();

    void begin();
    float readSingle(ModbusObject *object);
};

Modbus::Modbus(ModbusObject *pObject, uint8_t length)
{
    objectLength = length;
    for (uint8_t i = 0; i < length; ++i)
    {
        object[i] = &pObject[i];
    }
}

Modbus::~Modbus() {}

void Modbus::begin()
{
    pinMode(pinRE, OUTPUT);
    pinMode(pinDE, OUTPUT);
    if (objectLength == 1)
    {
        Serial2.begin(object[0]->baudRate);
        modbus.begin(object[0]->id, Serial2);
    }
    else
    {
        if (object[0]->id != object[1]->id)
        {
            Serial.println("Baud Rate is different!");
            while (1)
                ;
        }
        Serial2.begin(object[0]->baudRate);
        modbus.begin(object[0]->id, Serial2);
    }
}

float Modbus::readSingle(ModbusObject *obj)
{
    ModbusObject *selectedObject = nullptr;
    for (int i = 0; i < objectLength; i++)
    {
        if (obj == object[i])
        {
            selectedObject = object[i];
            break;
        }
    }
    if (selectedObject == nullptr)
    {
        return 0.1F; // Handle Deref nullpointer
    }

    uint8_t result = modbus.readInputRegisters(selectedObject->registerAddress, 1);
    if (result == modbus.ku8MBSuccess)
    {
        return float(modbus.getResponseBuffer(0));
    }
    return 0.0F; // No response
}
