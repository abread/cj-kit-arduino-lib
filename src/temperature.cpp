#include "temperature.h"

#include "base.h"
#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>

using namespace CJKit;

TemperatureSensorBus::TemperatureSensorBus(uint8_t busPin = DS18B20_BUS_PIN) : _bus(busPin),
                                                                               _sensors(&_bus)
{
}

void TemperatureSensorBus::_blockTillConversionComplete(void)
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

void TemperatureSensorBus::begin(void)
{
    _sensors.begin();
    _sensors.setWaitForConversion(false);
    setResolution(9);
}

bool TemperatureSensorBus::setResolution(uint8_t res)
{
    bool ok = true;

    res = constrain(res, 9, 12);
    DeviceAddress deviceAddress;
    for (uint8_t i = 0; i < _sensors.getDeviceCount(); i++)
    {
        if (_sensors.getAddress(deviceAddress, i))
        {
            if (!_sensors.setResolution(deviceAddress, res, true))
            {
                ok = false;
                // TODO: log
            }
        }
        else
        {
            ok = false;
            // TODO: log
        }
    }

    return ok;
}

void TemperatureSensorBus::requestTemperatures(void)
{
    _sensors.requestTemperatures();
    _lastMeasureReq = millis();
}

float TemperatureSensorBus::readTemperatureCForIndex(uint8_t index)
{
    _blockTillConversionComplete();
    return _sensors.getTempCByIndex(index);
}

bool TemperatureSensorBus::isMeasurementComplete(void)
{
    return _sensors.isConversionComplete();
}

uint8_t TemperatureSensorBus::getDeviceCount(void)
{
    return _sensors.getDeviceCount();
}
