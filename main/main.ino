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

void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;

  for (int i = 0; i < length; i++) {
    Serial.print((char) message[i]);
    messageTemp += (char) message[i];
  }
  Serial.println();

  if (String(topic) == mqtt_sub_topic) {
    Serial.print("Changing output to ");
    if (messageTemp == "ON") {
      Serial.println("ON");
      currentState = true;
      digitalWrite(led_pin, HIGH);
    } else if (messageTemp == "OFF") {
      Serial.println("OFF");
      currentState = false;
      digitalWrite(led_pin, LOW);
    }
  }
}

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
  client.setCallback(callback);
  client.publish(mqtt_topic, "OFF");

  //Initialize pins
  pinMode(led_pin, OUTPUT);
  pinMode(button_pin, INPUT);
}

void toggle() {
  currentState = !currentState;
  Serial.println(currentState ? "ON" : "OFF");
  client.publish(mqtt_topic, currentState ? "ON" : "OFF");
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266Client")) {
      Serial.println("connected");
      // Subscribe
      client.subscribe("esp32/output");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
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
