#ifndef _CJKIT_BASE_H
#define _CJKIT_BASE_H

#include <Arduino.h>

#ifndef CJKIT_VERSION
#define CJKIT_VERSION 2
#endif

#if CJKIT_VERSION == 0

#ifndef ARDUINO_AVR_NANO_EVERY
#error "CJ Kit v0 requires Arduino Nano Every"
#endif

#elif CJKIT_VERSION == 1

#ifndef ARDUINO_AVR_NANO
#warning "CJ Kit v1 requires Arduino Nano"
#endif

#elif CJKIT_VERSION == 2

#ifndef ARDUINO_AVR_NANO
#warning "CJ Kit v2 requires Arduino Nano"
#endif

#else
#error "Unknown CJKIT_VERSION (" CJKIT_VERSION "). Only 0, 1 and 2 are supported."
#endif

namespace CJKit
{
    const uint8_t DS18B20_BUS_PIN = 4;
    const uint8_t BUZZER_PIN = 5;
    const uint8_t RADIO_SS_PIN = 10;
    // const uint8_t RADIO_IRQ_PIN = ?; // version dependent
    const HardwareSerial &GPS_SERIAL = Serial;

#if CJKIT_VERSION == 0
    const uint8_t RADIO_IRQ_PIN = 5;
#elif CJKIT_VERSION == 1 || CJKIT_VERSION == 2
    const uint8_t RADIO_IRQ_PIN = 3;
#else
#error "LIBRARY BUG: unsupported kit version in RADIO_IRQ_PIN"
#endif

    const uint8_t RADIO_IRQ_PIN = 3;

#if CJKIT_VERSION == 2
    const uint8_t LED_D1_PIN = 6;
    const uint8_t LED_D2_PIN = 7;
    const uint8_t LED_D3_PIN = 8;
    const uint8_t LED_D4_PIN = 9;
#endif

    void (*_xdelay_idleTask)() = nullptr;
    const unsigned long XDELAY_MAX_INTERMEDIATE_DELAY_MS = 250;
    const unsigned long XDELAY_MIN_DELAY_IDLE_TASK_MS = 50;

    inline void xdelay(unsigned long duration)
    {
        if (_xdelay_idleTask == nullptr || duration < XDELAY_MIN_DELAY_IDLE_TASK_MS)
        {
            return delay(duration);
        }

        unsigned long startTime = millis();
        unsigned long elapsed = 0;
        while (elapsed < duration)
        {
            _xdelay_idleTask();

            elapsed = millis() - startTime;

            if (elapsed >= duration)
                break;

            unsigned long rem = duration - elapsed;
            if (rem < XDELAY_MAX_INTERMEDIATE_DELAY_MS)
            {
                delay(rem);
            }
            else
            {
                delay(XDELAY_MAX_INTERMEDIATE_DELAY_MS);
            }

            elapsed = millis() - startTime;
        }
    }
}

#endif
