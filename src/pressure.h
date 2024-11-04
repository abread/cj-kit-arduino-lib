#ifndef _CJKIT_PRESSURE_H
#define _CJKIT_PRESSURE_H

#include <Wire.h>
#include <Adafruit_BMP085.h>

namespace CJKit
{

    /**
     * Pressure sensor used in the CanSat Júnior Kit.
     * All kit versions use the BMP085 sensor.
     *
     * This sensor continuously measures ambient pressure, and the lateste measurement can be
     * requested with Pressure::read.
     * Users must call Pressure::begin exactly once before any other method.
     */
    class Pressure
    {
    private:
        Adafruit_BMP085 _bmp;

    public:
        Pressure(void);

        /**
         * @brief Connects to and initializes pressure sensor.
         * Must be called exactly once before any other method.
         *
         * Refer to sensor datasheet for the meaning of the various modes.
         *
         * @param mode Mode to set, ultra high-res by default.
         * @param wire The I2C interface to use, defaults to Wire.
         * @return Returns true if successful, false otherwise.
         */
        bool begin(uint8_t mode = BMP085_ULTRAHIGHRES, TwoWire *wire = &Wire);

        /**
         * @brief Read latest measured pressure from sensor in hPa.
         *
         * @returns Latest measured pressure in hPa.
         */
        int32_t pressureHPa(void);

        /**
         * @brief Read latest measured temperature from sensor in ºC.
         *
         * @returns Latest measured temperature in ºC.
         */
        float temperatureC(void);
    };

}


#endif
