#include <Arduino.h>
#include "gps.h"

using namespace CJKit;

template <HardwareSerial &GpsSerial>
class Gps
{
public:
    void begin(unsigned long gpsBaudRate, unsigned long parsePendingTimeoutMs)
    {
        GpsSerial.begin(gpsBaudRate);

        // try to get some info right at the start
        parsePending(parsePendingTimeoutMs);
    }

    void parsePending(unsigned long parsePendingTimeoutMs)
    {
        unsigned long start = millis();
        while (millis() - start < parsePendingTimeoutMs)
        {
            for (uint8_t i = 0; i < PARSE_MAX_BATCH_SIZE && GpsSerial.available(); i++)
            {
                _parser.encode(GpsSerial.read());
            }
        }
    }

    double latitudeDeg()
    {
        return _parser.location.lat();
    }

    double longitudeDeg()
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

    double altitudeM()
    {
        return _parser.altitude.meters();
    }

    unsigned long altitudeAge()
    {
        return _parser.altitude.age();
    }
};
