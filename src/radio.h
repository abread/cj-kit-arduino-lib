#ifndef _CJKIT_RADIO_H
#define _CJKIT_RADIO_H

#include <Arduino.h>
#include <SPIFlash.h>
#include <RFM69.h>
#include <RFM69_ATC.h>
#include "base.h"
#include "buffered_print.h"

#ifndef _CJKIT_RADIO_CLASS
#if CJKIT_VERSION == 0
#define _CJKIT_RADIO_CLASS RFM69_ATC
#elif CJKIT_VERSION <= 2
#define _CJKIT_RADIO_CLASS RFM69
#else
#error "unsupported kit version for radio"
#endif
#endif

namespace CJKit
{
    /// Maximum payload size in a radio packet.
    static const uint8_t RADIO_PAYLOAD_MAX_SIZE = 61;

    /**
     * CanSat Júnior's Radio driver with Print-like interface.
     *
     * This class allows [Print::print]-ing to the radio with the usual Arduino Print methods (same as in
     * Serial), by buffering output (see [BufferedPrint]) up to the maximum size of an RFM69HCW packet.
     * Users must call [begin] before any other method, and should call [setFrequency] with their assigned
     * telemtry frequency.
     *
     * To minimize the effects of interference, users should keep the size of each transmission under
     * [MAX_BUFFER_SIZE] and call [flush] between transmissions, ensuring either the full message is
     * received or nothing is received at all (corrupted messages are detected and discarded automatically
     * by the radio hardware).
     *
     * To support use cases more advanced than what this library allows while retaining the convenience
     * of Print methods, users create a new class extending [BufferedPrint] with the desired functionality.
     */
    template <uint8_t OWN_NODE_ID = 0, uint8_t DEST_NODE_ID = 1, uint8_t NET_ID = 100>
    class StreamedRadio : public BufferedPrint<RADIO_PAYLOAD_MAX_SIZE>
    {
    public:
        /// Radio encryption key size (in bytes).
        static const uint8_t ENCRYPTION_KEY_SIZE = 16; // RFM69HCW uses AES128, which demands 128-bit (16-byte) keys, DO NOT CHANGE

    private:
        _CJKIT_RADIO_CLASS _radio;
        static const uint8_t RADIO_FREQ_BAND = RF69_433MHZ;

    protected:
        void write_unbuffered(uint8_t const *buf, int size) final
        {
            // TODO: be smart about logging
            Serial.print("saída rádio: ");
            Serial.write(buf, size);
            Serial.println();
            _radio.send(DEST_NODE_ID, buf, size);
        }

    public:
        /**
         * Create radio driver.
         *
         * Default parameters correspond to standard kit configuration.
         *
         * @param slaveSelectPin - Arduino digital pin connected to radio's NSS pin.
         * @param interruptPin - Arduino digital pin with interrupt capabilities connected to radio's DIO0 pin.
         * @param isRFM69HW - True if radio is a RFM69HW, false otherwise.
         * @param spi - SPI interface where radio is connected. Use nullptr for default Arduino SPI interface.
         */
        StreamedRadio(uint8_t slaveSelectPin = RADIO_SS_PIN, uint8_t interruptPin = RADIO_IRQ_PIN, bool isRFM69HW = false, SPIClass *spi = nullptr) : _radio(slaveSelectPin, interruptPin, isRFM69HW, spi) {}

        /**
         * Initialize radio device.
         *
         * Default parameters correspond to standard kit configuration.
         *
         * @param freqBand - Radio frequency band (see RFM69 library documentation).
         * @returns True if initialization is successful, false otherwise.
         */
        bool begin(uint8_t freqBand = RF69_433MHZ)
        {
            if (!_radio.initialize(freqBand, OWN_NODE_ID, NET_ID))
            {
                // TODO: debug log
                return false;
            }
            _radio.setHighPower();
            _radio.encrypt(nullptr);
#if CJKIT_VERSION != 0 && CJKIT_VERSION <= 2
            _radio.setPowerDBm(5);
#endif

            return true;
        }

        /**
         * Set radio operating frequency (for transmission and reception).
         *
         * @param freq - Operating frequency in Hz.
         */
        void setFrequency(uint32_t freq)
        {
            _radio.setFrequency(freq);
        }

        /**
         * Set radio encryption key (use nullptr to disable encryption).
         *
         * The encryption key must be [ENCRYPTION_KEY_SIZE] bytes and should be generated with a
         * secure AES-128 key generator.
         *
         * @param key - Encryption key.
         */
        void setEncryptionKey(uint8_t const key[ENCRYPTION_KEY_SIZE])
        {
            _radio.encrypt((char const *)&key[0]);
        }

        /**
         * Underlying RFM69 radio device.
         * @deprecated Unstable interface. Use with caution.
         */
        _CJKIT_RADIO_CLASS &internalRadio()
        {
            return _radio;
        }
    };
}

#endif
