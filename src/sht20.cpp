#include "sht20.h"

SHT20::SHT20() {}

SHT20::~SHT20() {}

bool SHT20::begin()
{
    digitalWrite(RE_MODBUS, 0);
    digitalWrite(DE_MODBUS, 0);
    modbus.begin(modbusAddress, Modbus);
    modbus.preTransmission(preTransmission);
    modbus.postTransmission(postTransmission);
    return true;
}

void SHT20::setEnable(bool enable)
{
    if (enable)
    {
        digitalWrite(RE_MODBUS, 1);
        digitalWrite(DE_MODBUS, 1);
    }
    else
    {
        digitalWrite(RE_MODBUS, 0);
        digitalWrite(DE_MODBUS, 0);
    }
}

float SHT20::readTemperature()
{
    uint8_t result = modbus.readInputRegisters(0x0001, 2);
    if (result == modbus.ku8MBSuccess)
    {
        return float(modbus.getResponseBuffer(0) / 10.0F);
    }
    return 60;
}

float SHT20::readHumidity()
{
    uint8_t result = modbus.readInputRegisters(0x0001, 2);
    if (result == modbus.ku8MBSuccess)
    {
        return float(modbus.getResponseBuffer(1) / 10.0F);
    }
    return 60;
}
