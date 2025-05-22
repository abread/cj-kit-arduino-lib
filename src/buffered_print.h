#ifndef _CJKIT_BUFFERED_PRINT_H
#define _CJKIT_BUFFERED_PRINT_H

#include <Arduino.h>

namespace CJKit {
/**
 * Buffered version of Print. This class is meant to be extended by another
 * which overrides the BufferedPrint::write_unbuffered method. Calls to regular
 * Print methods will be buffered until the buffer is full or
 * BufferedPrint::flush is called. At that point, buffer contents will be
 * written using the overriden BufferedPrint::write_unbuffered method.
 */
template <size_t BUFFER_SIZE> class BufferedPrint : public Print {
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
   * Remaining free space in the buffer.
   */
  size_t bufferSpace(void) const { return BUFFER_SIZE - _buffer_len; }

  /**
   * Flushes buffer contents (if not empty), writing them to the underlying
   * output and clearing the buffer.
   */
  void flush(void) {
    if (_buffer_len == 0) {
      return;
    }

    write_unbuffered(_buffer, _buffer_len);
    _buffer_len = 0;
  }

  size_t write(uint8_t const *buffer, size_t size) final {
    size_t i = 0;
    while (i < size) {
      if (bufferSpace() == 0) {
        flush();
      }

      size_t sz = min(bufferSpace(), size - i);
      memcpy(_buffer + _buffer_len, buffer + i, sz);
      i += sz;
      _buffer_len += sz;
    }

    return size;
  }
  size_t write(uint8_t b) final { return write(&b, 1); }
  int availableForWrite(void) final { return bufferSpace(); }

  // extra decimal places for floating point
  // the next declarations will shadow Print::print and Print::println, so we
  // must bring them to the derived class to keep the other overloaded original
  // print/println methods.
  using Print::print;
  using Print::println;

  size_t print(double d, int n = 5) { return Print::print(d, n); }
  size_t println(double d, int n = 5) { return Print::println(d, n); }
  size_t print(float f, int n = 5) { return Print::print(f, n); }
  size_t println(float f, int n = 5) { return Print::println(f, n); }
};
} // namespace CJKit

#endif
