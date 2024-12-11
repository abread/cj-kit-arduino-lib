#define CJKIT_VERSION 2
#include <CJKit.h>

CJKit::Pressure pressure;

// this will be run once at startup
void setup() {
  Serial.begin(9600);

  if (!pressure.begin()) {
    Serial.println("pressure: FAIL: begin");
  }
}

// this will be run repeatedly after setup
void loop() {
  int32_t p = pressure.readPressurePa();
  float tp = pressure.readTemperatureC();

  Serial.print("Pressure sensor measured pressure=");
  Serial.print(p);
  Serial.print("Pa and temperature=");
  Serial.println(tp);

  CJKit::xdelay(1000); // pause for 1s
}
