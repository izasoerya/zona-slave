#include "anemometer.h"

Anemometer::Anemometer() {}

Anemometer::~Anemometer() {}

bool Anemometer::begin()
{
    digitalWrite(RE_MODBUS, 0);
    digitalWrite(DE_MODBUS, 0);
    modbus.begin(modbusAddress, Modbus);
    modbus.preTransmission(preTransmission);
    modbus.postTransmission(postTransmission);
    return true;
}

void Anemometer::setEnable(bool enable)
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

uint16_t Anemometer::readWindSpeed()
{
    uint8_t result = modbus.readHoldingRegisters(0, 1);
    if (result == modbus.ku8MBSuccess)
    {
        return uint16_t(modbus.getResponseBuffer(0));
    }
    return 60;
}
