#define CJKIT_VERSION 2
#include <CJKit.h>

// this will be run once at startup
void setup() {
  // Arduino Nano has a built-in LED, always
  pinMode(LED_BUILTIN, OUTPUT); // configure the LED pin as an output
 
#if CJKIT_VERSION >= 2 // this ensures the code within is only active when CJKIT_VERSION is defined to a value >= 2
  // The v2 kit also has 4 extra LEDs in the "Arduino PCB"
  pinMode(CJKit::LED_D1_PIN, OUTPUT); // LED marked as D1
  pinMode(CJKit::LED_D2_PIN, OUTPUT); // LED marked as D2
  pinMode(CJKit::LED_D3_PIN, OUTPUT); // etc.
  pinMode(CJKit::LED_D4_PIN, OUTPUT);
#endif
}

// this will be run repeatedly after setup
void loop() {
  // blink LEDs!
  
  // turn them all off
  digitalWrite(LED_BUILTIN, LOW);
#if CJKIT_VERSION >= 2
  digitalWrite(CJKit::LED_D1_PIN, LOW);
  digitalWrite(CJKit::LED_D2_PIN, LOW);
  digitalWrite(CJKit::LED_D3_PIN, LOW);
  digitalWrite(CJKit::LED_D4_PIN, LOW);
#endif

  CJKit::xdelay(1000); // leave them off for 1s

  // turn them all on
  digitalWrite(LED_BUILTIN, HIGH);
#if CJKIT_VERSION >= 2
  digitalWrite(CJKit::LED_D1_PIN, HIGH);
  digitalWrite(CJKit::LED_D2_PIN, HIGH);
  digitalWrite(CJKit::LED_D3_PIN, HIGH);
  digitalWrite(CJKit::LED_D4_PIN, HIGH);
#endif

  CJKit::xdelay(1000); // leave them on for 1s
}

