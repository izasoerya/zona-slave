#include "ambient_light.h"

AmbientLight::AmbientLight() : myLux(13) {}

AmbientLight::~AmbientLight() {}

void AmbientLight::begin()
{
    myLux.begin();
}

int AmbientLight::readLightLevel()
{
    return myLux.lightStrengthLux();
}