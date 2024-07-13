#ifndef AMBIENT_LIGHT_H
#define AMBIENT_LIGHT_H

#include <DFRobot_B_LUX_V30B.h>

class AmbientLight
{
public:
    AmbientLight();
    ~AmbientLight();

    void begin();

    int readLightLevel();

private:
    DFRobot_B_LUX_V30B myLux;
};

#endif // AMBIENT_LIGHT_H