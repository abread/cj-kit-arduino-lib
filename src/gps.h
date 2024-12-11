#ifndef _CJKIT_GPS_H
#define _CJKIT_GPS_H

// TODO: research if we can do this without macros (e.g. error out on GPS class
// use, but always have it defined)
#ifdef CJKIT_ENABLE_GPS

#include "base.h"
#include <Arduino.h>
#include <TinyGPS++.h>

namespace CJKit {
/**
 * Serial U(S)ART-based GPS device.
 *
 * WARNING: Unstable interface. Use with caution.
 *
 * Optionally used in CanSat Júnior, particularly in the context of an high
 * height launch. Users must call begin before any other method and periodically
 * call Gps::parsePending to process and parse incoming data from the Gps. Refer
 * to CJKit::xdelay to hook this function into your program's idle times. The
 * serial interface MUST not receive non-GPS data, and should not be used to
 * send data while the GPS is connected to not risk changing its configuration
 * accidentally. Accessor methods report the latest processed information from
 * the GPS device.
 *
 * This library supports all U(S)ART-based GPS devices supported by the
 * TinyGPS++ library.
 */
class Gps {
private:
  /// Stream of incoming NMEA messages from GPS (e.g. Serial U(S)ART).
  Stream &_nmeaStream;

  /// Internal instance of NMEA message parser.
  TinyGPSPlus _parser;

public:
  /// Maximum bytes processed per batch in Gps::parsePending.
  const uint8_t PARSE_MAX_BATCH_SIZE = 128;

  /**
   * Construct a new Gps interface from an existing stream of incoming NMEA
   * messages.
   * @param nmeaStream Stream of incoming NMEA messages, defaults to the GPS
   * device to be used with your kit (which is NOT INITIALIZED BY THIS METHOD).
   */
  Gps(Stream &nmeaStream = GPS_SERIAL) : _nmeaStream(nmeaStream) {}

  /**
   * Accept and parse incoming GPS data within a configurable soft deadline.
   * The deadline mechanism is not precise ("soft"): this method processes data
   * in batches of Gps::PARSE_MAX_BATCH_SIZE bytes and the deadline only
   * prevents the next batch from being processed.
   *
   * @param parsePendingTimeoutMs Deadline in ms, defaults to 250ms after
   * millis()
   */
  void parsePending(unsigned long parsePendingDeadlineMs) {
    while (millis() < parsePendingDeadlineMs) {
      for (uint8_t i = 0; i < PARSE_MAX_BATCH_SIZE && _nmeaStream.available();
           i++) {
        _parser.encode(_nmeaStream.read());
      }
    }
  }
  void parsePending() { parsePending(millis() + 250); }

  /**
   * Last received latitude in degrees.
   */
  double latitudeDeg(void) { return _parser.location.lat(); }

  /**
   * Last received longitude in degrees.
   */
  double longitudeDeg(void) { return _parser.location.lng(); }

  /**
   * UTC time of the last received latitude and longitude.
   */
  uint32_t positionAge(void) { return _parser.location.age(); }

  /**
   * Last received course in degrees.
   */
  double courseDeg(void) { return _parser.course.deg(); }

  /**
   * UTC time of the last received course.
   */
  uint32_t courseAge(void) { return _parser.course.age(); }

  /**
   * Last received speed in meters per second.
   */
  double speedMps(void) { return _parser.speed.mps(); }

  /**
   * UTC time of the last received speed.
   */
  uint32_t speedAge(void) { return _parser.speed.age(); }

  /**
   * Last received altitude in meters.
   */
  double altitudeM(void) { return _parser.altitude.meters(); }

  /**
   * UTC time of the last received altitude.
   */
  uint32_t altitudeAge(void) { return _parser.altitude.age(); }

  /**
   * Read-only reference to the internal parsing library instance.
   *
   * Internal parsing library may be changed without notice.
   *
   * @deprecated Unstable interface. Use with caution.
   * @return const-reference to the internal parsing library.
   */
  TinyGPSPlus const &internalParser(void) const { return _parser; }
};
} // namespace CJKit

#endif
#endif
