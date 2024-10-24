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
     * to start a measurement on all sensors connected to the bus. Afterwards, they can call TemperatureSensorBus::readTemperatureForIndex
     * to read the temperature from a sensor.
     */
    class TemperatureSensorBus
    {
        /**
         * The longest time we will wait for a DS18B20 sensor to signal its measurement is ready.
         */
        const unsigned long DS18B20_MAX_CONVERSION_TIMEOUT = 750; /* from library */

    private:
        /// @brief OneWire bus where temperature sensors are connected
        OneWire _bus;

        /// @brief Wrapper around bus for easy sensor interaction.
        DallasTemperature _sensors;

        /// @brief Latest measurement request time
        unsigned long _lastMeasureReq = 0;

        /// @brief Blocks until the first sensor in the bus reports its measurement to be complete.
        void _blockTillConversionComplete(void);

    public:
        /**
         * Constructs a new temperature sensor bus.
         * Users must call begin exactly once before performing measurements.
         *
         * @param busPin - The digital pin used for the sensor bus.
         */
        TemperatureSensorBus(uint8_t busPin = DS18B20_BUS_PIN);

        /**
         * Initializes the sensor bus, enumerating the temperature sensors within it and setting their
         * bit resolution to 9.
         *
         * Must be called exactly once before any other methods (except constructor).
         * Sensors connected after calling begin will be ignored by all methods.
         */
        void begin(void);

        /**
         * Set bit resolution of all DS18B20 temperature sensors connected to the bus.
         *
         * Bit resolutions will be constrained to valid values (9 to 12).
         * This affects the measurement time: refer to the datasheet for more information.
         *
         * @param res - New bit resolution (9, 10, 11 or 12)
         * @return true if resolution was successfully updated on all connected sensors, false otherwise
         */
        bool setResolution(uint8_t res);

        /**
         * Starts a new temperature measurement on all sensors connected to the bus.
         *
         * Refer to the datasheet for measurement duration.
         * The temperature can be read from each sensor with TemperatureSensorBus::readTemperatureForIndex.
         */
        void requestTemperatures(void);

        /**
         * Read measured temperature from a sensor connected to the bus.
         * This method will block until measurement is complete (if not complete already).
         *
         * @param index The index of the desired sensor (between 0 and getDeviceCount())
         * @return measured temperature, or -127.0 if communication with the sensor failed.
         */
        float readTemperatureForIndex(uint8_t index);

        /**
         * Check if temperature measurement finished in the first connected sensor (index 0).
         *
         * @return true if last requested temperature measurement is complete, false otherwise
         */
        bool measurementComplete(void);

        /**
         * Number of temperature (DS18B20) sensors connected to the bus.
         * This number does not change after calling begin.
         *
         * @return Number of temperature sensors connected to the bus.
         */
        uint8_t getDeviceCount(void);
    };
}

#endif
