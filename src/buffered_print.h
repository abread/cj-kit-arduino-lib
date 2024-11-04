#ifndef _CJKIT_BUFFERED_PRINT_H
#define _CJKIT_BUFFERED_PRINT_H

#include <Arduino.h>

namespace CJKit
{
    /**
     *
     */
    template <int BUFFER_SIZE>
    class BufferedPrint : public Print
    {
    private:
        uint8_t _buffer[BUFFER_SIZE] = {0};
        uint8_t _buffer_len = 0;

    protected:
        /**
         *
         */
        virtual void write_unbuffered(uint8_t const *buf, int len);

    public:
        /**
         *
         */
        int bufferSpace(void) const;

        /**
         *
         */
        void flush(void);

        /**
         *
         */
        size_t write(uint8_t const *payload, int len);

        /**
         *
         */
        size_t write(uint8_t c);

        /**
         *
         */
        int availableForWrite(void) const;

        // extra decimal places for floating point
        // the next declarations will shadow Print::print and Print::println, so we must bring them
        // to the derived class to keep the other overloaded original print/println methods.
        using Print::print;
        using Print::println;

        size_t print(double d, int n = 5);
        size_t println(double d, int n = 5);
        size_t print(float f, int n = 5);
        size_t println(float f, int n = 5);
    };
}

#endif
