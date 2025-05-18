#pragma once

#include <Arduino.h>

struct SensorData
{
    float temperature;
    float humidity;
    float nh3;
    uint16_t lux;
    uint16_t windSpeed;
};