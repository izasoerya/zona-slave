#pragma once

#include <Arduino.h>

struct SensorData
{
    float temperature;
    float humidity;
    float nh3;
    uint16_t lux;
    uint16_t windSpeed;

    String toString()
    {
        return "Temperature: " + String(temperature, 2) +
               ", Humidity: " + String(humidity, 2) +
               ", NH3: " + String(nh3, 2) +
               ", Lux: " + String(lux) +
               ", WindSpeed: " + String(windSpeed);
    }
};