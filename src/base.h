#ifndef _CJKIT_BASE_H
#define _CJKIT_BASE_H

#ifndef CJKIT_VERSION
#warning "CJKIT_VERSION not defined. Assuming kit version 2."
#define CJKIT_VERSION 2
#endif

#include <Arduino.h>

#if CJKIT_VERSION == 0

#ifndef ARDUINO_AVR_NANO_EVERY
#warning "CJ Kit v0 is designed for Arduino Nano Every. Radio will likely NOT WORK for reception."
#endif

#elif CJKIT_VERSION == 1

#ifndef ARDUINO_AVR_NANO
#warning "CJ Kit v1 is designed for Arduino Nano"
#endif

#elif CJKIT_VERSION == 2

#ifndef ARDUINO_AVR_NANO
#warning "CJ Kit v2 is designed for Arduino Nano"
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
    // HardwareSerial &GPS_SERIAL = Serial; // version/Arduino dependent
    const unsigned long GPS_BAUD_RATE = 9600;

#if CJKIT_VERSION == 0
    const uint8_t RADIO_IRQ_PIN = 5;
#elif CJKIT_VERSION == 1 || CJKIT_VERSION == 2
    const uint8_t RADIO_IRQ_PIN = 3;
#else
#error "LIBRARY BUG: unsupported kit version in RADIO_IRQ_PIN"
#endif

#ifdef ARDUINO_AVR_NANO_EVERY
    HardwareSerial &GPS_SERIAL = Serial1;
#else
#ifndef ARDUINO_AVR_NANO
#warning "unsupported Arduino board kind. setting same settings as if for Arduino Nano (ATmega328P)"
#endif
    HardwareSerial &GPS_SERIAL = Serial;
#endif

#if CJKIT_VERSION == 2
    const uint8_t LED_D1_PIN = 6;
    const uint8_t LED_D2_PIN = 7;
    const uint8_t LED_D3_PIN = 8;
    const uint8_t LED_D4_PIN = 9;

    const uint8_t LED_PINS[4] = {LED_D1_PIN, LED_D2_PIN, LED_D3_PIN, LED_D4_PIN};
#endif

    typedef void(IdleTask)(uint32_t);
    extern IdleTask *__xdelay_idleTask;

    const unsigned long XDELAY_MAX_INTERMEDIATE_DELAY_MS = 250;
    const unsigned long XDELAY_MIN_DELAY_IDLE_TASK_MS = 50;

    void xdelay(unsigned long duration)
    {
        if (__xdelay_idleTask == nullptr || duration < XDELAY_MIN_DELAY_IDLE_TASK_MS)
        {
            return delay(duration);
        }

        unsigned long startTime = millis();
        unsigned long elapsed = 0;
        while (elapsed < duration)
        {
            __xdelay_idleTask(duration - elapsed);

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

    IdleTask *getXdelayIdleTask(void)
    {
        return __xdelay_idleTask;
    }

    IdleTask *setXdelayIdleTask(IdleTask *task)
    {
        IdleTask *old = __xdelay_idleTask;
        __xdelay_idleTask = task;
        return old;
    }

    IdleTask *clearXdelayIdleTask(void)
    {
        return setXdelayIdleTask(nullptr);
    }
}

#endif
