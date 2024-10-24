#ifndef _CJKIT_GPS_H
#define _CJKIT_GPS_H

#include <TinyGPS++.h>

#define GPS_SERIAL Serial
#define GPS_SERIAL_BAUD 9600

class Gps
{
private:
    TinyGPSPlus _parser;
    unsigned long _lastNewMessage = 0;

public:
    void setup()
    {
        GPS_SERIAL.begin(GPS_SERIAL_BAUD);

        // try to get some info right at the start
        parsePending();
    }

    void parsePending()
    {
        while (GPS_SERIAL.available())
        {
            _parser.encode(GPS_SERIAL.read());
        }
    }

    double latitude()
    {
        return _parser.location.lat();
    }

    double longitude()
    {
        return _parser.location.lng();
    }

    unsigned long positionAge()
    {
        return _parser.location.age();
    }

    double courseDeg()
    {
        return _parser.course.deg();
    }

    unsigned long courseAge()
    {
        return _parser.course.age();
    }

    double speedMps()
    {
        return _parser.speed.mps();
    }

    unsigned long speedAge()
    {
        return _parser.speed.age();
    }

    double altitudeMeters()
    {
        return _parser.altitude.meters();
    }

    unsigned long altitudeAge()
    {
        return _parser.altitude.age();
    }
} gps;

#endif
