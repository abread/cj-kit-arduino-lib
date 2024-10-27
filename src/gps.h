#ifndef _CJKIT_GPS_H
#define _CJKIT_GPS_H

#include <Arduino.h>
#include <TinyGPS++.h>
#include "base.h"

namespace CJKit
{
    /**
     * Serial U(S)ART-based GPS device.
     *
     * Optionally used in CanSat Júnior, particularly in the context of an high height launch.
     * Users must call begin before any other method and periodically call Gps::parsePending to
     * process and parse incoming data from the Gps. Refer to CJKit::xdelay to hook this function
     * into your program's idle times.
     * The serial interface MUST not receive non-GPS data, and should not be used to send data while
     * the GPS is connected to not risk changing its configuration accidentally.
     * Accessor methods report the latest processed information from the GPS device.
     *
     * This library supports all UART-based GPS devices supported by the TinyGPS++ library.
     *
     * @param GpsSerial GPS Serial interface, defaults to the one available in your kit.
     */
    template <HardwareSerial &GpsSerial = GPS_SERIAL>
    class Gps
    {
    private:
        /// @brief Internal instance of NMEA message parser.
        TinyGPSPlus _parser;

    public:
        /// @brief Maximum bytes processed per batch in Gps::parsePending.
        const uint8_t PARSE_MAX_BATCH_SIZE = 128;

        /**
         * @brief Initializes GPS Serial/U(S)ART interface and processes a limited number of messages.
         * Must be called exactly once before any other method.
         *
         * Refer to parsePending for the limitations of the timeout mechanism.
         *
         * @param baudRate GPS Serial interface baud rate, defaults to the GPS device to be used with your kit
         * @param parsePendingTimeoutMs Maximum time to spend processing incoming messages before returning (in ms), defaults to 1000ms.
         */
        void begin(unsigned long baudRate = GPS_BAUD_RATE, unsigned long parsePendingTimeoutMs = 1000);

        /**
         * @brief Accept and parse incoming GPS data within a configurable timeout.
         * The timeout mechanism is not precise: this method processes data in batches of Gps::PARSE_MAX_BATCH_SIZE
         * bytes and the timeout only prevents the next batch from being processed.
         *
         * @param parsePendingTimeoutMs Timeout in ms, defaults to 250ms
         */
        void parsePending(unsigned long parsePendingTimeoutMs = 250);

        /**
         * @brief Last received latitude in degrees.
         */
        double latitudeDeg();

        /**
         * @brief Last received longitude in degrees.
         */
        double longitudeDeg();

        /**
         * @brief UTC time of the last received latitude and longitude.
         */
        uint32_t positionAge();

        /**
         * @brief Last received course in degrees.
         */
        double courseDeg();

        /**
         * @brief UTC time of the last received course.
         */
        uint32_t courseAge();

        /**
         * @brief Last received speed in meters per second.
         */
        double speedMps();

        /**
         * @brief UTC time of the last received speed.
         */
        uint32_t speedAge();

        /**
         * @brief Last received altitude in meters.
         */
        double altitudeM();

        /**
         * @brief UTC time of the last received altitude.
         */
        uint32_t altitudeAge();

        /**
         * @brief UNSTABLE INTERFACE: Read-only reference to the internal parsing library instance.
         *
         * Internal parsing library may be changed without notice.
         *
         * @return const-reference to the internal parsing library.
         */
        TinyGPSPlus const &internalParser() const;
    };
}

#endif
