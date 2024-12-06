#ifndef _CJKIT_TEMPERATURE_H
#define _CJKIT_TEMPERATURE_H

#include "base.h"
#include <stdint.h>
#include <OneWire.h>
#include <DallasTemperature.h>

namespace CJKit
{

    /**
     * Temperature sensor bus used in the CanSat Júnior Kit.
     *
     * A temperature sensor bus may be connected to one or more sensors.
     * All kit versions use the DS18B20 sensor.
     *
     * Users must call the TemperatureSensorBus::begin exactly once before any other methods.
     * Temperature measurements are performed asynchronously: users must call TemperatureSensorBus::requestTemperatures
     * to start a measurement on all sensors connected to the bus. Afterwards, they can call TemperatureSensorBus::temperatureCForIndex
     * to read the temperature from a sensor.
     */
    class TemperatureSensorBus
    {
    private:
        /// OneWire bus where temperature sensors are connected
        OneWire _bus;

        /// Wrapper around bus for easy sensor interaction.
        DallasTemperature _sensors;

        /// Latest measurement request time
        unsigned long _lastMeasureReq = 0;

        /// Blocks until the first sensor in the bus reports its measurement to be complete.
        void _blockTillConversionComplete(void)
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
                    delay(1); // always too small for xdelay anyway
                }
                else
                {
                    xdelay(rem);
                }
            }

            _lastMeasureReq = 0;
        }

    public:
        /**
         * The longest time we will wait for a DS18B20 sensor to signal its measurement is ready.
         */
        const unsigned long DS18B20_MAX_CONVERSION_TIMEOUT = 750; /* from library */

        /**
         * Constructs a new temperature sensor bus.
         * Users must call begin exactly once before performing measurements.
         *
         * @param busPin - The digital pin used for the sensor bus.
         */
        TemperatureSensorBus(uint8_t busPin = DS18B20_BUS_PIN) : _bus(busPin), _sensors(&_bus) {}
        /**
         * Initializes the sensor bus, enumerating the temperature sensors within it and setting their
         * bit resolution to 9.
         *
         * Must be called exactly once before any other methods (except constructor).
         * Sensors connected after calling begin will be ignored by all methods.
         */
        void begin(void)
        {
            _sensors.begin();
            _sensors.setWaitForConversion(false);
            setResolution(9);
        }

        /**
         * Set bit resolution of all DS18B20 temperature sensors connected to the bus.
         *
         * Bit resolutions will be constrained to valid values (9 to 12).
         * This affects the measurement time: refer to the datasheet for more information.
         *
         * @param res - New bit resolution (9, 10, 11 or 12)
         * @return true if resolution was successfully updated on all connected sensors, false otherwise
         */
        bool setResolution(uint8_t res)
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

        /**
         * Starts a new temperature measurement on all sensors connected to the bus.
         *
         * Refer to the datasheet for measurement duration.
         * The temperature can be read from each sensor with TemperatureSensorBus::temperatureCForIndex.
         */
        void requestTemperatures(void)
        {
            _sensors.requestTemperatures();
            _lastMeasureReq = millis();
        }

        /**
         * Read measured temperature from a sensor connected to the bus.
         * This method will block until measurement is complete (if not complete already).
         *
         * @param index The index of the desired sensor (between 0 and deviceCount())
         * @return measured temperature, or -127.0 if communication with the sensor failed.
         */
        float temperatureCForIndex(uint8_t index)
        {
            _blockTillConversionComplete();
            return _sensors.getTempCByIndex(index);
        }

        /**
         * Check if temperature measurement finished in the first connected sensor (index 0).
         *
         * @return true if last requested temperature measurement is complete, false otherwise
         */
        bool isMeasurementComplete(void)
        {
            return _sensors.isConversionComplete();
        }

        /**
         * Number of temperature (DS18B20) sensors connected to the bus.
         * This number does not change after calling begin.
         *
         * @return Number of temperature sensors connected to the bus.
         */
        uint8_t deviceCount(void)
        {
            return _sensors.getDeviceCount();
        }

        /**
         * Underlying OneWire bus object.
         * @deprecated Unstable interface. Use with caution.
         */
        OneWire &internalBus()
        {
            return _bus;
        }

        /**
         * Underlying DallasTemperature sensors object.
         * @deprecated Unstable interface. Use with caution.
         */
        DallasTemperature &internalSensors()
        {
            return _sensors;
        }
    };
}

#endif
