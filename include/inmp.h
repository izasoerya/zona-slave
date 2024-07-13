#ifndef INMP_H
#define INMP_H

#include <Arduino.h>
#include <driver/i2s.h>
#include "pin_definition.h"
#include "models.h"

class INMP441
{
public:
    INMP441();
    ~INMP441();
    bool begin();
    FrequencyData read();

private:
};

#endif // INMP_H