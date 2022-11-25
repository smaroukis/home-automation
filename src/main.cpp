
#if defined(ESP8266)
#include <ESP8266WiFi.h>
#else
#include <WiFi.h>
#endif
#if defined(ESP8266)
#include <ESP8266WebServer.h>
#else
#include <WebServer.h>
#endif
#include <PubSubClient.h>

// SECRET_SSID and SECRET_PASS
#include "secrets.h"

// Custom Headers
#include "sensorFunctions.h"

// Wifi 
WiFiClient espClient;
PubSubClient mqttClient(espClient); //lib required for mqtt

void callback(char* topic, byte* payload, unsigned int length) {   //callback includes topic and payload ( from which (topic) the payload is comming)

  // Print topic and payload
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
  }

  // If payload is "ON"
  if ((char)payload[0] == 'O' && (char)payload[1] == 'N') //on
  {
    digitalWrite(BUILTIN_LED, HIGH);
    Serial.println("on");
    mqttClient.publish("outTopic", "LED turned ON");
  }

  // If payload is "OFF"
  else if ((char)payload[0] == 'O' && (char)payload[1] == 'F' && (char)payload[2] == 'F') //off
  {
    digitalWrite(BUILTIN_LED, LOW);
    Serial.println(" off");
    mqttClient.publish("outTopic", "LED turned OFF");
  }

  // get temperature and publish to `status/temp` 

  Serial.println();
}


void reconnect() {
  while (!mqttClient.connected()) {
    Serial.println("Attempting MQTT connection...");
    if (mqttClient.connect("ESP32_clientID")) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      mqttClient.publish("outTopic", "Nodemcu connected to MQTT");
      // ... and resubscribe
      mqttClient.subscribe("inTopic");

    } else {
      Serial.print("failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}


void connectMqtt()
{
  mqttClient.connect("ESP32_clientID");  // ESP will connect to mqtt broker with clientID
  {
    Serial.println("connected to MQTT");
    mqttClient.subscribe("inTopic"); //topic=Demo
    mqttClient.publish("outTopic",  "connected to MQTT");

    if (!mqttClient.connected())
    {
      reconnect();
    }
  }
}

void setup()
{
  // Setup Serial Monitor and Hardware
  Serial.begin(115200);

  pinMode(BUILTIN_LED, OUTPUT);
  digitalWrite(BUILTIN_LED, LOW);

// Setup Wifi and MQTT
  WiFi.begin(SECRET_SSID, SECRET_PASS);
  Serial.println("Wifi connected");

  mqttClient.setServer(MQTT_SERVER, 1883);//connecting to mqtt server
  mqttClient.setCallback(callback);
  //delay(5000);
  connectMqtt();

// Setup Sensors
  dhtSetup();
}


void loop()
{
  // Handle WiFi connection
  if (!mqttClient.connected())
  {
    reconnect();
  }
  mqttClient.loop(); // MQTT keep alive, callbacks, etc
  
  // delay handled in sensorFunctions.h
  // if delay has not been met, -999 is returned
  float temperature = getTemperature();
  if ((temperature != -999) && (!isnan(temperature))) 
  {
    // TODO publish value as string to home/device/status/temp
    Serial.print(F("Temperature: "));
    Serial.print(temperature);
    Serial.println(F("°C"));
  }

}

/*  Debugging Checklist
1) Check pin numbers in code
2) Check breadboarding
3) 
*/