#ifndef ANEMOMETER_H
#define ANEMOMETER_H

#include <Arduino.h>
#include <ModbusMaster.h>
#include <pin_definition.h>

class Anemometer
{
public:
    Anemometer();
    ~Anemometer();
    bool begin();
    void setEnable(bool enable);
    uint16_t readWindSpeed();

private:
    ModbusMaster modbus;
    uint8_t modbusAddress = 1;

    static void preTransmission()
    {
        digitalWrite(RE_MODBUS, 1);
        digitalWrite(DE_MODBUS, 1);
    }

    static void postTransmission()
    {
        digitalWrite(RE_MODBUS, 0);
        digitalWrite(DE_MODBUS, 0);
    }
};

#endif // ANEMOMETER_H