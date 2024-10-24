#ifndef _CJKIT_TEMPERATURE_H
#define _CJKIT_TEMPERATURE_H

#include "base.h"
#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>

static const unsigned long DS18B20_MAX_CONVERSION_TIMEOUT = 750; /* from library */

namespace CJKit
{
    class TemperatureSensorBus
    {
    private:
        OneWire _bus;
        DallasTemperature _sensors;
        unsigned long _lastMeasureReq = 0;

        void _blockTillConversionComplete()
        {
            if (_lastMeasureReq == 0)
            { // equivalent to completed conversion
                return;
            }

            while (!_sensors.isConversionComplete() && (millis() - _lastMeasureReq < DS18B20_MAX_CONVERSION_TIMEOUT))
            {
                // TODO: vary with resolution instead of fixed max timeout
                unsigned long rem = DS18B20_MAX_CONVERSION_TIMEOUT - (millis() - _lastMeasureReq);
                if (rem > XDELAY_MAX_INTERMEDIATE_DELAY_MS)
                {
                    xdelay(XDELAY_MAX_INTERMEDIATE_DELAY_MS);
                }
                else if (rem == 0)
                {
                    delay(10);
                }
                else
                {
                    xdelay(rem);
                }
            }

            _lastMeasureReq = 0;
        }

    public:
        TemperatureSensorBus(uint8_t busPin = DS18B20_BUS_PIN) : _bus(busPin),
                                                                 _sensors(&_bus)
        {
        }

        void begin()
        {
            _sensors.begin();
            _sensors.setWaitForConversion(false);
            setResolution(9);
        }

        void setResolution(uint8_t res)
        {
            _sensors.setResolution(res);
        }

        void requestTemperatures()
        {
            _sensors.requestTemperatures();
            _lastMeasureReq = millis();
        }

        float readTemperatureForIndex(uint8_t index)
        {
            _blockTillConversionComplete();
            return _sensors.getTempCByIndex(index);
        }

        uint8_t getDeviceCount()
        {
            return _sensors.getDeviceCount();
        }
    };
}

#endif
