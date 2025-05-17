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
    ModbusObject object[2];
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
    if (objectLength == 2)
    {
        memcpy(object, pObject, 2 * sizeof(ModbusObject));
    }
    else if (objectLength == 1)
    {
        memcpy(object, pObject, sizeof(ModbusMaster));
    }
}

Modbus::~Modbus() {}

void Modbus::begin()
{
    pinMode(pinRE, OUTPUT);
    pinMode(pinDE, OUTPUT);
    if (objectLength == 1)
    {
        Serial2.begin(object[0].baudRate);
        modbus.begin(object[0].id, Serial2);
    }
    else
    {
        if (object[0].id != object[1].id)
        {
            Serial.println("Baud Rate is different!");
            while (1)
                ;
        }
        Serial2.begin(object[0].baudRate);
        modbus.begin(object[0].id, Serial2);
    }
}

float Modbus::readSingle(ModbusObject *obj)
{
    ModbusObject *selectedObj = (obj == &object[0]) ? &object[0] : &object[1];

    uint8_t result = modbus.readInputRegisters(selectedObj->registerAddress, 1);
    if (result == modbus.ku8MBSuccess)
    {
        return float(modbus.getResponseBuffer(0));
    }
    return 0.0F;
}
