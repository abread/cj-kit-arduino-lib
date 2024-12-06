#define CJKIT_VERSION 2
#include <CJKit.h>

CJKit::TemperatureSensorBus temperatureBus;

// this will be run once at startup
void setup() {
  Serial.begin(9600);

  temperatureBus.begin();
  if (temperatureBus.deviceCount() != 1) {
    Serial.print("temperature: FAIL: sensor count ");
    Serial.print(temperatureBus.deviceCount());
    Serial.println(" != 1");
  } else if (! temperatureBus.setResolution(9)) {
    Serial.println("temperature: FAIL: setResolution");
  }
  
  temperatureBus.requestTemperatures();
}

// this will be run repeatedly after setup
void loop() {
  float t = temperatureBus.temperatureCForIndex(0);

  Serial.print("Temperature (ºC): ");
  Serial.println(t);

  // request new measurement before pausing the MCU
  // this way the sensor will perform its duties while we wait instead of stalling us.
  temperatureBus.requestTemperatures();
 
  CJKit::xdelay(1000); // sleep for 1s
}

