#include "radio.h"

using namespace CJKit;

template <uint8_t OWN_NODE_ID = 1, uint8_t DEST_NODE_ID = 2, uint8_t NET_ID = 100>
class StreamedRadio : public BufferedPrint<MAX_BUFFER_SIZE>
{
public:
    StreamedRadio(uint8_t slaveSelectPin = RADIO_SS_PIN, uint8_t interruptPin = RADIO_IRQ_PIN, bool isRFM69HW = false, SPIClass *spi = nullptr) : _radio(slaveSelectPin, interruptPin, isRFM69HW, spi) {}

    /**
     *
     */
    bool begin(uint8_t freqBand = RF69_433MHZ)
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

    /**
     *
     */
    void setFrequency(uint32_t freq)
    {
        _radio.setFrequency(freq);
    }

    /**
     *
     */
    void write_unbuffered(uint8_t const *buf, int size)
    {
        // TODO: be smart about logging
        Serial.print("saída rádio: ");
        Serial.write(_buffer, _buffer_len);
        Serial.println();
        _radio.send(_destId, _buffer, _buffer_len);
    }
};
