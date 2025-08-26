enum SensorType
{
    TEMPERATURE,
    HUMIDITY,
    AMMONIA,
    LIGHT,
    ANEMOMETER,
};

enum ModbusCommandType
{
    HOLDING_REGISTER,
    INPUT_REGISTER,
};

inline const char *sensorTypeToString(SensorType type)
{
    switch (type)
    {
    case TEMPERATURE:
        return "TEMPERATURE";
    case HUMIDITY:
        return "HUMIDITY";
    case AMMONIA:
        return "AMMONIA";
    case LIGHT:
        return "LIGHT";
    case ANEMOMETER:
        return "ANEMOMETER";
    default:
        return "UNKNOWN";
    }
}

inline const char *modbusCommandTypeToString(ModbusCommandType type)
{
    switch (type)
    {
    case HOLDING_REGISTER:
        return "HOLDING_REGISTER";
    case INPUT_REGISTER:
        return "INPUT_REGISTER";
    default:
        return "UNKNOWN";
    }
}