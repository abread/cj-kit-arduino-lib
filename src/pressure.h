#ifndef _CJKIT_PRESSURE_H
#define _CJKIT_PRESSURE_H

#include <Adafruit_BMP085.h>
#include <Wire.h>

namespace CJKit {

/**
 * Pressure sensor used in the CanSat Júnior Kit.
 * All kit versions use the BMP085 sensor.
 *
 * This sensor continuously measures ambient pressure, and the lateste
 * measurement can be requested with Pressure::read. Users must call
 * Pressure::begin exactly once before any other method.
 */
class Pressure {
private:
  Adafruit_BMP085 _bmp;

public:
  Pressure(void) {}

  /**
   * Connects to and initializes pressure sensor.
   * Must be called exactly once before any other method.
   *
   * Refer to sensor datasheet for the meaning of the various modes.
   *
   * @param mode Mode to set, ultra high-res by default.
   * @param wire The I2C interface to use, defaults to Wire.
   * @return Returns true if successful, false otherwise.
   */
  bool begin(uint8_t mode = BMP085_ULTRAHIGHRES, TwoWire *wire = &Wire) {
    if (!_bmp.begin(mode, wire)) {
      // TODO: log
      return false;
    }

    return true;
  }

  /**
   * Read latest measured pressure from sensor in Pa.
   *
   * @returns Latest measured pressure in Pa.
   */
  int32_t readPressurePa(void) {
    _bmp.readTemperature(); // TODO: check if it works without this call (it
                            // should)
    return _bmp.readPressure();
  }

  /**
   * Read latest measured temperature from sensor in ºC.
   *
   * @returns Latest measured temperature in ºC.
   */
  float readTemperatureC(void) { return _bmp.readTemperature(); }

  /**
   * Internal Adafruit_BMP085 object.
   * @deprecated Unstable interface. Use with caution.
   */
  Adafruit_BMP085 &internalBmp() { return _bmp; }
};

} // namespace CJKit

#endif
