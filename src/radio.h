#ifndef _CJKIT_RADIO_H
#define _CJKIT_RADIO_H

#include "base.h"
#include "buffered_print.h"
#include <SPIFlash.h>
#include <RFM69.h>
#include <RFM69_ATC.h>

namespace CJKit
{
    template <uint8_t OWN_NODE_ID = 1, uint8_t DEST_NODE_ID = 2, uint8_t NET_ID = 100>
    class StreamedRadio : public BufferedPrint<MAX_BUFFER_SIZE>
    {
    public:
        static const uint8_t RADIO_FREQ_BAND = RF69_433MHZ;
        static const uint8_t MAX_BUFFER_SIZE = 61; // library limitation, DO NOT CHANGE

    private:
        RFM69_ATC _radio;

    protected:
        void write_unbuffered(uint8_t const *buf, int size);

    public:
        StreamedRadio(uint8_t slaveSelectPin = RADIO_SS_PIN, uint8_t interruptPin = RADIO_IRQ_PIN, bool isRFM69HW = false, SPIClass *spi = nullptr);

        /**
         *
         */
        bool begin(uint8_t freqBand = RF69_433MHZ);

        /**
         *
         */
        void setFrequency(uint32_t freq);
    };
}

#endif
