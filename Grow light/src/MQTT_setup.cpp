#include <Arduino.h> // if you aren't using PlaformIO remove this line
#include "definitions.h" // Global definitions for pins and baud rate
#include "MQTTsetup.h"
#include "secret.h" // Wifi SSID, Password, and broker address file
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// Update these with values suitable for your network.

const char* ssid = SECRET_SSID;
const char* password = SECRET_PASSWORD;
const char* mqtt_server = SECRET_BROKER;


WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE	(50)
char msg[MSG_BUFFER_SIZE];
int value = 0;

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  wifiStatus();
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  String temp_message;
  for (unsigned int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    temp_message += (char)payload[i];
  }
  Serial.println();
  /*if the garden/light topic is recieved we either turn the grow light as well as the status LED, on or 
  off based on the attached message

  */
  if (String(topic) == "garden/light"){
      if (temp_message == "on"){
        Serial.println("on");
        digitalWrite(relay, LOW);
        digitalWrite(status_led, LOW);
      }
      else if (temp_message == "off")
      {
        Serial.println("off");
          digitalWrite(relay, HIGH);
          digitalWrite(status_led, HIGH);
      }
       
  }
}

long lastReconnectAttempt = 0;

boolean reconnect() {
  if (client.connect("Growlamp")) {
    // Once connected, publish an announcement...
    client.publish("grow_light_out","hello world");
    // ... and resubscribe
    client.subscribe("garden/light");
  }
  return client.connected();
}

void setup() {
  pinMode(status_led, OUTPUT);     // Initialize the Status LED pin as an output
  pinMode(relay,OUTPUT); // Initialize the relay pin as output
  digitalWrite(relay,HIGH);
  digitalWrite(status_led, HIGH); // Turn off the status LED
  Serial.begin(9600);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  lastReconnectAttempt = 0;
}
//Simple wifi connection status function, flashes the status LED on 4 times
void wifiStatus(){
  for (int i = 0; i<4; i++){
    if (i % 2 == 0){
      digitalWrite(status_led,LOW);
    }
    else {
      digitalWrite(status_led, HIGH);
    }
    delay(1000);
  }
}

void loop()
{
  if (!client.connected()) {
    long now = millis();
    if (now - lastReconnectAttempt > 5000) {
      lastReconnectAttempt = now;
      // Attempt to reconnect
      if (reconnect()) {
        wifiStatus(); // flash the status led 4 times to signal connection
        lastReconnectAttempt = 0;
      }
    }
  } else {
    // Client connected

    client.loop();

  }

}