#pragma once

#include <Arduino.h>
#include "modbus.h"

const ModbusObject ammonia = {
    SensorType::AMMONIA,
    1, 9600, 0x07D1, 1,
    ModbusCommandType::HOLDING_REGISTER};
const ModbusObject anemoMeter = {
    SensorType::ANEMOMETER,
    2, 9600, 0x0000, 1,
    ModbusCommandType::HOLDING_REGISTER};
const ModbusObject temperature = {
    SensorType::TEMPERATURE,
    3, 9600, 0x0001, 1,
    ModbusCommandType::INPUT_REGISTER};
const ModbusObject humidity = {
    SensorType::HUMIDITY,
    3, 9600, 0x0002, 1,
    ModbusCommandType::INPUT_REGISTER};