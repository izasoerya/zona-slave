#ifndef SHT20_H
#define SHT20_H

#include <Arduino.h>
#include <ModbusMaster.h>
#include <pin_definition.h>

class SHT20
{
public:
    SHT20();
    ~SHT20();
    bool begin();
    void setEnable(bool enable);
    float readTemperature();
    float readHumidity();

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

#endif // SHT20_H