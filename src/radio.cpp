#include "radio.h"

using namespace CJKit;

template <uint8_t OWN_NODE_ID, uint8_t DEST_NODE_ID, uint8_t NET_ID>
class StreamedRadio : public BufferedPrint<RADIO_PAYLOAD_MAX_SIZE>
{
    StreamedRadio(uint8_t slaveSelectPin, uint8_t interruptPin, bool isRFM69HW, SPIClass *spi) : _radio(slaveSelectPin, interruptPin, isRFM69HW, spi) {}

    bool begin(uint8_t freqBand)
    {
        if (!_radio.initialize(RF69_433MHZ, _nodeId, _netId))
        {
            // TODO: debug log
            return false;
        }
        _radio.setHighPower();

        // TODO: investigate whether to disable ATC, and whether to change the default to the library default (-90).
        _radio.enableAutoPower(-80);
        return true;
    }

    void setFrequency(uint32_t freq)
    {
        _radio.setFrequency(freq);
    }

    void setEncryptionKey(uint8_t const key[ENCRYPTION_KEY_SIZE])
    {
        _radio.encrypt(key);
    }

    void write_unbuffered(uint8_t const *buf, int size)
    {
        // TODO: be smart about logging
        Serial.print("saída rádio: ");
        Serial.write(_buffer, _buffer_len);
        Serial.println();
        _radio.send(_destId, _buffer, _buffer_len);
    }
};
