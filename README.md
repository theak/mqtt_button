# mqtt_button
Use an ESP32 Arduino as a toggle button that communicates state over MQTT. Optionally, if a PIR motion sensor is connected, motion state can be reported as well.

Pressing a button toggles the light and the state of the binary sensor. If the light is toggled on, an "ON" message is sent. If the light is toggled off, an "OFF" message is sent. The state of the light can also be remotely controlled via the mqtt_sub_topic.

If motion is detected, then "ON" message is sent to mqtt_motion_topic.

To get started:
1. Copy config.h.template to config.h
2. Edit the values to match your setup
3. Open main/main.ino in Arduino IDE
4. Install the PubSubClient library (Sketch -> Include libraries -> Manage libraries)
5. Select the device type, compile, and run. If necessary, install support for the relevant device (e.g. ESP32).
