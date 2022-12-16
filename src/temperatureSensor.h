// TODO refactor to .cpp file when done

#ifndef temperatureSensor_h
#define temperatureSensor_h

#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

// Custom Headers
#include "sensorStd.h" // for RETURN_NULL

#define DHTPIN 13

// Uncomment the type of sensor in use:
#define DHTTYPE    DHT11     // DHT 11

DHT_Unified dht(DHTPIN, DHTTYPE);

// Sensor Delays Here
unsigned long tempDelay_ms = 4000;
unsigned long prevTemp_ms;

void dhtSetup() {
  Serial.begin(9600);
  // Initialize device.
  dht.begin();
  Serial.println(F("DHTxx Unified Sensor Example"));
  sensor_t sensor;
}

void dhtLoop() {
  // delay(tempDelay_ms);
  sensors_event_t event;
  dht.temperature().getEvent(&event);
  if (isnan(event.temperature)) {
    Serial.println(F("Error reading temperature!"));
  }
  else {
    Serial.print(F("Temperature: ")); Serial.print(event.temperature); Serial.println(F("°C"));
  }
  // Get humidity event and print its value.
  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity)) {
    Serial.println(F("Error reading humidity!"));
  }
  else {
    Serial.print(F("Humidity: ")); Serial.print(event.relative_humidity); Serial.println(F("%"));
  }
}

// Custom Get Temp Wrapper
// float getTemperature(){
//   // requires: DHT object and sensor event from DHT_unified
//   // modifies: prints to the serial monitor
//   // returns: temperature as float or NAN if delay has been met, else RETURN_NULL

//   if (millis() - prevTemp_ms >= tempDelay_ms) {
//     sensors_event_t event;
//     dht.temperature().getEvent(&event);
//     float temperature = event.temperature;

//     if (isnan(temperature)) {
//       Serial.println(F("Error reading temperature from Sensor! (NAN)"));
//       temperature = NAN;
//     } 

//     prevTemp_ms = millis(); // delay time is more important than constant cycle time
//     Serial.print(F("Temperature: "));
//     Serial.print(temperature);
//     Serial.println(F("°C"));

//     return temperature;
//   }

//   else return RETURN_NULL;
// }

// REMOVE AFTER TESTING
float getTemperature(){
    if (millis() - prevTemp_ms >= tempDelay_ms) {
        prevTemp_ms= millis();
        return 42.00f;
    }
    return RETURN_NULL;
}

#endif


/*  EXAMPLE USAGE IN MAIN LOOP (main.cpp:main)
  // delay handled in temperatureSensor.cpp
  // if delay has not been met, RETURN_NULL is returned
  auto temperature = getTemperature();
  if ((temperature != RETURN_NULL) && (!isnan(temperature))) 
  {
    // publishSensorVal returns mqttClient.publish() which is false if failed
   if (!publishSensorVal(mqttClient, "temperature", temperature))
      { Serial.println("Error publishing temperature"); }
  }
*/