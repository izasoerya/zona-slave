#include "mics.h"

MICS6814::MICS6814() {}

MICS6814::~MICS6814() {}

void MICS6814::begin() {}

float MICS6814::readNO2()
{
    return analogRead(NO2);
}

float MICS6814::readNH3()
{
    return analogRead(NH3);
}

float MICS6814::readCO()
{
    return analogRead(CO);
}