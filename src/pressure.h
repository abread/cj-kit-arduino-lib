#ifndef _CJKIT_PRESSURE_H
#define _CJKIT_PRESSURE_H

#include <Wire.h>
#include <Adafruit_BMP085.h>
class Pressure
{
    Adafruit_BMP085 _bmp;

public:
    void setup()
    {
        if (!_bmp.begin())
        {
            Serial.println("begin fail");
            radio.println("!!! BAD: Sensor de pressao nao encontrado. Verifica as tuas ligacoes.");
            // beep(2000);
            err = true;
        }
        else
        {
            // beep(100);
            Serial.println("begin ok");
            radio.println("OK: sensor press init");
        }
    }

    double read()
    {
        _bmp.readTemperature(); // won't work without it, TODO: check if library takes care of this for us
        return _bmp.readPressure();
    }
} pressure;

#endif
