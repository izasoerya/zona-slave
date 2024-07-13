#ifndef INMP_H
#define INMP_H

#include <Arduino.h>
#include <driver/i2s.h>
#include "pin_definition.h"

class INMP441
{
public:
    INMP441();
    ~INMP441();
    bool begin();
    int32_t read();

private:
};

#endif // INMP_H