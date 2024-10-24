#include "pressure.h"
#include <Wire.h>

using namespace CJKit;

bool Pressure::begin(uint8_t mode, TwoWire *wire)
{
    bool ok = _bmp.begin(mode, wire);
    if (!ok)
    {
        // TODO: log
    }

    return ok;
}

int32_t Pressure::readPressureHPa(void)
{
    _bmp.readTemperature(); // TODO: check if it works without this call (it should)
    return _bmp.readPressure();
}

float Pressure::readTemperatureC(void)
{
    return _bmp.readTemperature();
}
