#define CJKIT_VERSION 2
#include <CJKit.h>

// this will be run once at startup
void setup() {
  // configure pin where the (positive terminal of the) buzzer is attached as a
  // digital output
  pinMode(CJKit::BUZZER_PIN, OUTPUT);
}

// this will be run repeatedly after setup
void loop() {
  // beep the buzzer!

  // turn it off
  digitalWrite(CJKit::BUZZER_PIN, LOW);

  CJKit::xdelay(1000); // leave it off for 1s

  // turn it on
  digitalWrite(CJKit::BUZZER_PIN, HIGH);

  CJKit::xdelay(1000); // leave it on for 1s
}
