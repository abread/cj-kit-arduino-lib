#define CJKIT_VERSION 2
#include <CJKit.h>

const uint32_t RADIO_FREQUENCY = 433000000; // Hz

CJKit::StreamedRadio<> radio;

// this will be run once at startup
void setup() {
  Serial.begin(9600);

  #if CJKIT_VERSION == 2
  // v2 kit's level shifter requires a slower SPI bus (still way faster than anything we would transmit).
  SPI.setClockDivider(SPI_CLOCK_DIV16);
  #endif

  if (radio.begin()) {
    radio.setFrequency(RADIO_FREQUENCY);
    Serial.println("radio: init OK");
  } else {
    Serial.println("radio: init FAIL");
  }
}

// this will be run repeatedly after setup
void loop() {
  unsigned long now = millis();

  radio.print("I have been awake for ");
  radio.print(now);
  radio.println("ms.");
  radio.flush(); // ensure the full message is sent out now (otherwise it may be delayed until the internal buffer fills up)

  CJKit::xdelay(1000); // pause for 1s
}

