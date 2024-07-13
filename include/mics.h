#ifndef MICS_H
#define MICS_H

#include "pin_definition.h"

class MICS6814
{
public:
    MICS6814();
    ~MICS6814();

    void begin();
    float readNO2();
    float readNH3();
    float readCO();

private:
};

#endif