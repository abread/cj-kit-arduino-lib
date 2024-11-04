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
    // HardwareSerial & const GPS_SERIAL = Serial; // Arduino dependent
    const unsigned long GPS_BAUD_RATE = 9600;

#if CJKIT_VERSION == 0
    const uint8_t RADIO_IRQ_PIN = 5;
#elif CJKIT_VERSION == 1 || CJKIT_VERSION == 2
    const uint8_t RADIO_IRQ_PIN = 3;
#else
#error "LIBRARY BUG: unsupported kit version in RADIO_IRQ_PIN"
#endif

#ifdef ARDUINO_AVR_NANO_EVERY
    HardwareSerial &const GPS_SERIAL = Serial1;
#else
#ifndef ARDUINO_AVR_NANO
#warning "unsupported Arduino board kind. setting same settings as if for Arduino Nano (ATmega328P)"
#endif
    HardwareSerial &const GPS_SERIAL = Serial;
#endif

#if CJKIT_VERSION == 2
    const uint8_t LED_D1_PIN = 6;
    const uint8_t LED_D2_PIN = 7;
    const uint8_t LED_D3_PIN = 8;
    const uint8_t LED_D4_PIN = 9;
#endif

    typedef void(IdleTask)(void);

    IdleTask *getXdelayIdleTask(void);
    IdleTask *clearXdelayIdleTask(void);
    IdleTask *setXdelayIdleTask(IdleTask *task);

    const unsigned long XDELAY_MAX_INTERMEDIATE_DELAY_MS = 250;
    const unsigned long XDELAY_MIN_DELAY_IDLE_TASK_MS = 50;

    void xdelay(unsigned long duration);
}

#endif
