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
    float windSpeed;

    bool sht20Enable = true;
    bool micsEnable = true;
    bool inmpEnable = true;
    bool windEnable = true;
    bool lightEnable = true;

    void debugAll(HardwareSerial &Serial)
    {
        Serial.print("Temperature: ");
        Serial.println(temperature);
        Serial.print("Humidity: ");
        Serial.println(humidity);
        Serial.print("NH3: ");
        Serial.println(nh3);
        Serial.print("CO2: ");
        Serial.println(co2);
        Serial.print("NO2: ");
        Serial.println(no2);
        Serial.print("Wind Speed: ");
        Serial.println(windSpeed);
        Serial.print("Lux: ");
        Serial.println(lux);
        Serial.println("");
        // for (int i = 0; i < 64; i++)
        // {
        //     Serial.print(frequencyData.frequency[i]);
        //     Serial.print(" ");
        // }
    }
};