#include "pressure.h"
#include <Wire.h>

using namespace CJKit;

bool Pressure::begin(uint8_t mode, TwoWire *wire)
{
    if (!_bmp.begin(mode, wire))
    {
        // TODO: log
        return false;
    }

    return true;
}

int32_t Pressure::pressurePa(void)
{
    _bmp.readTemperature(); // TODO: check if it works without this call (it should)
    return _bmp.readPressure();
}

float Pressure::temperatureC(void)
{
    return _bmp.readTemperature();
}
