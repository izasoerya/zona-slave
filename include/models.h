#include <Arduino.h>

struct SensorData
{
    float temperature;
    float humidity;
    float nh3;
    float co2;
    float no2;
    float frequency;
    float windSpeed;
    uint16_t lux;

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
        Serial.print("Frequency: ");
        Serial.println(frequency);
        Serial.print("Wind Speed: ");
        Serial.println(windSpeed);
        Serial.print("Lux: ");
        Serial.println(lux);
    }
};