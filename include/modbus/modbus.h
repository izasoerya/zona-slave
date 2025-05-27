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
    digitalWrite(pinDE, LOW);
    digitalWrite(pinDE, LOW);
    modbus.preTransmission(preTransmission);
    modbus.postTransmission(postTransmission);
    if (objectLength == 1)
    {
        Serial2.begin(object[0]->baudRate);
        // modbus.begin(object[0]->id, Serial2);
    }
    else
    {
        if (object[0]->baudRate != object[1]->baudRate)
        {
            Serial.println("Baud Rate is different!");
            while (1)
                ;
        }
        Serial2.begin(object[0]->baudRate);
    }
}

float Modbus::readSingle(ModbusObject *obj)
{
    ModbusObject *selectedObject = nullptr;

    for (int i = 0; i < objectLength; i++)
    {
        if (obj->sensor == object[i]->sensor)
        {
            selectedObject = object[i];
            break;
        }
    }
    if (selectedObject == nullptr)
    {
        return -69; // Handle Deref nullpointer
    }
    modbus.begin(selectedObject->id, Serial2);

    uint8_t result;
    if (selectedObject->type == ModbusCommandType::INPUT_REGISTER)
    {
        Serial.println("inputreg");
        result = modbus.readInputRegisters(selectedObject->registerAddress, 1);
        if (result == modbus.ku8MBSuccess)
        {
            return modbus.getResponseBuffer(0);
        }
        Serial.println(selectedObject->sensor);
        Serial.println(selectedObject->registerAddress);
    }
    else if (selectedObject->type == ModbusCommandType::HOLDING_REGISTER)
    {
        Serial.println("holdingreg");
        result = modbus.readHoldingRegisters(selectedObject->registerAddress, 1);
        if (result == modbus.ku8MBSuccess)
        {
            return modbus.getResponseBuffer(0);
        }
        else
        {
            return result;
        }
        Serial.println(selectedObject->id);
        Serial.println(selectedObject->sensor);
        Serial.println(selectedObject->registerAddress);
    }
    else
    {
        return -68; // Unsupported Modbus command type
    }
}
