#pragma once

#include <Arduino.h>

struct FrequencyData
{
    int32_t frequency[64];
};

struct SensorData
{
    float temperature;
    float humidity;
    float nh3;
    float co2;
    float no2;
    uint16_t lux;
    FrequencyData frequencyData;
    uint16_t windSpeed;

    bool sht20Enable = true;
    bool micsEnable = true;
    bool inmpEnable = false;
    bool anemometerEnable = false;
    bool lightEnable = true;

    void debugAll(HardwareSerial &Serial)
    {
        if (sht20Enable)
        {
            Serial.print("Temperature: ");
            Serial.println(temperature);
            Serial.print("Humidity: ");
            Serial.println(humidity);
        }
        if (micsEnable)
        {
            Serial.print("NH3: ");
            Serial.println(nh3);
            Serial.print("CO2: ");
            Serial.println(co2);
            Serial.print("NO2: ");
            Serial.println(no2);
        }
        if (inmpEnable)
        {
            for (int i = 0; i < 64; i++)
            {
                Serial.print(frequencyData.frequency[i]);
                Serial.print(" ");
            }
        }
        if (anemometerEnable)
        {
            Serial.print("Wind Speed: ");
            Serial.print(windSpeed);
            Serial.println(" m/s");
        }
        if (lightEnable)
        {
            Serial.print("Lux: ");
            Serial.println(lux);
        }
        Serial.println("");
    }
};