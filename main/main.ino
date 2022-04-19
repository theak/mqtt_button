#include <WiFi.h>
#include <PubSubClient.h>

#include "config.h"

int buttonState = 0;
int lastButtonState = 0;
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;

bool currentState = false;

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(9600);

  //Initialize Wi-Fi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.println(WiFi.localIP());

  //Initialize MQTT
  client.setServer(mqtt_server, mqtt_port);

  client.publish(mqtt_topic, "OFF");

  //Initialize pins
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(button_pin, INPUT);
}

void toggle() {
  currentState = !currentState;
  Serial.println(currentState ? "ON" : "OFF");
  client.publish(mqtt_topic, currentState ? "ON" : "OFF");
}

void loop() {
  // put your main code here, to run repeatedly:
  int reading = digitalRead(button_pin);
  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }
  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != buttonState) {
      buttonState = reading;
      if (buttonState == LOW) {
        toggle();
      }
    }
  }
  lastButtonState = reading;
}
