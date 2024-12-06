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
    /// Temperature sensor (DS18B20) bus pin.
    const uint8_t DS18B20_BUS_PIN = 4;

    /// (Active) buzzer pin (positive terminal).
    const uint8_t BUZZER_PIN = 5;

    /// Radio SS pin.
    const uint8_t RADIO_SS_PIN = 10;

    // const uint8_t RADIO_IRQ_PIN = ?; // version dependent
    // HardwareSerial &GPS_SERIAL = Serial; // version/Arduino dependent

    /// GPS serial baud rate.
    const unsigned long GPS_BAUD_RATE = 9600;

#if CJKIT_VERSION == 0
    /// Radio receive interrupt pin.
    const uint8_t RADIO_IRQ_PIN = 5;
#elif CJKIT_VERSION == 1 || CJKIT_VERSION == 2
    /// Radio receive interrupt pin.
    const uint8_t RADIO_IRQ_PIN = 3;
#else
#error "LIBRARY BUG: unsupported kit version in RADIO_IRQ_PIN"
#endif

#ifdef ARDUINO_AVR_NANO_EVERY
    /// GPS Serial port.
    HardwareSerial &GPS_SERIAL = Serial1;
#else
#ifndef ARDUINO_AVR_NANO
#warning "unsupported Arduino board kind. setting same settings as if for Arduino Nano (ATmega328P)"
#endif
    /// GPS Serial port.
    HardwareSerial &GPS_SERIAL = Serial;
#endif

#if CJKIT_VERSION == 2
    /// "Arduino PCB" D1 LED pin.
    const uint8_t LED_D1_PIN = 6;

    /// "Arduino PCB" D2 LED pin.
    const uint8_t LED_D2_PIN = 7;

    /// "Arduino PCB" D3 LED pin.
    const uint8_t LED_D3_PIN = 8;

    /// "Arduino PCB" D4 LED pin.
    const uint8_t LED_D4_PIN = 9;
#endif

    /**
     * Idle task type for xdelay: a function that receives the remaining time in milliseconds and
     * returns nothing.
     *
     * The idle task is called during the delay, allowing for other tasks to be executed.
     * As such it must be fast, aiming to complete in less than the provided time, which is always
     * at least XDELAY_MIN_DELAY_IDLE_TASK_MS milliseconds.
     *
     * The idle task may be called any number of times during the delay.
     * In particular, it may not be called at all if the delay is under XDELAY_MIN_DELAY_IDLE_TASK_MS
     * milliseconds.
     */
    typedef void(IdleTask)(uint32_t);

    /// @private Idle task for xdelay.
    extern IdleTask *__xdelay_idleTask;

    /// Target delay between idle task calls.
    const unsigned long XDELAY_MAX_INTERMEDIATE_DELAY_MS = 250;

    /// Minimum delay for idle task execution.
    const unsigned long XDELAY_MIN_DELAY_IDLE_TASK_MS = 50;

    /**
     * Extended delay function that allows for idle task execution during the delay.
     *
     * @param duration - The duration of the delay in milliseconds.
     * @see IdleTask
     */
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

    /**
     * Get the current idle task for xdelay.
     * @see IdleTask
     * @return The current idle task (nullptr if not set).
     */
    IdleTask *getXdelayIdleTask(void)
    {
        return __xdelay_idleTask;
    }

    /**
     * Set the idle task for xdelay.
     *
     * Note that the previous idle task is no longer called after setting a new idle task.
     *
     * @see IdleTask
     * @param task - The new idle task.
     * @returns The previous idle task (nullptr if not set).
     */
    IdleTask *setXdelayIdleTask(IdleTask *task)
    {
        IdleTask *old = __xdelay_idleTask;
        __xdelay_idleTask = task;
        return old;
    }

    /**
     * Clear the idle task for xdelay.
     * @see IdleTask
     * @returns The previous idle task (nullptr if not set).
     */
    IdleTask *clearXdelayIdleTask(void)
    {
        return setXdelayIdleTask(nullptr);
    }
}

#endif
