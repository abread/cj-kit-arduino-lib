#ifndef _CJKIT_BUFFERED_PRINT_H
#define _CJKIT_BUFFERED_PRINT_H

#include <Arduino.h>

namespace CJKit
{
    /**
     * Buffered version of Print. This class is meant to be extended by another which overrides
     * the BufferedPrint::write_unbuffered method. Calls to regular Print methods will be buffered
     * until the buffer is full or BufferedPrint::flush is called. At that point, buffer contents
     * will be written using the overriden BufferedPrint::write_unbuffered method.
     */
    template <int BUFFER_SIZE>
    class BufferedPrint : public Print
    {
    private:
        uint8_t _buffer[BUFFER_SIZE] = {0};
        uint8_t _buffer_len = 0;

    protected:
        /**
         * Write buffer contents to underlying output (e.g. radio).
         *
         * @param buffer - Pointer to memory region to be written.
         * @param len - Size of the memory region to be written.
         */
        virtual void write_unbuffered(uint8_t const *buffer, int len);

    public:
        /**
         * @brief Remaining free space in the buffer.
         */
        int bufferSpace(void) const;

        /**
         * @brief Flushes buffer contents (if not empty), writing them to the underlying output and clearing
         * the buffer.
         */
        void flush(void);

        // TODO: do I need the override keyword?
        override size_t write(uint8_t const *payload, int len);
        override size_t write(uint8_t c);
        override int availableForWrite(void) const;

        // extra decimal places for floating point
        // the next declarations will shadow Print::print and Print::println, so we must bring them
        // to the derived class to keep the other overloaded original print/println methods.
        using Print::print;
        using Print::println;

        override size_t print(double d, int n = 5);
        override size_t println(double d, int n = 5);
        override size_t print(float f, int n = 5);
        override size_t println(float f, int n = 5);
    };
}

#endif
