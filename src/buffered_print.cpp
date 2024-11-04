#include <Arduino.h>
#include "buffered_print.h"

using namespace CJKit;

template <int BUFFER_SIZE>
class BufferedPrint : public Print
{
public:
    int bufferSpace(void) const
    {
        return BUFFER_SIZE - _buffer_len;
    }

    void flush(void)
    {
        if (_buffer_len == 0)
        {
            return;
        }

        write_unbuffered(_buffer, _buffer_len);
        _buffer_len = 0;
    }

    size_t write(uint8_t const *payload, int len)
    {
        int i = 0;
        while (i < len)
        {
            if (bufferSpace() == 0)
            {
                flush();
            }

            int sz = min(bufferSpace(), len - i);
            memcpy(_buffer + _buffer_len, payload + i, sz);
            i += sz;
            _buffer_len += sz;
        }

        return len;
    }

    size_t write(uint8_t c)
    {
        return write(&c, 1);
    }

    int availableForWrite(void) const
    {
        return bufferSpace();
    }

    // The following methods are (re)declared in BufferedPrint just to set a different default
    // precision. The logic is exactly the same as the original Print::print/println.
    size_t print(double d, int n)
    {
        return Print::print(d, n);
    }
    size_t println(double d, int n)
    {
        return Print::println(d, n);
    }
    size_t print(float f, int n)
    {
        return Print::print(f, n);
    }
    size_t println(float f, int n)
    {
        return Print::println(f, n);
    }
};
