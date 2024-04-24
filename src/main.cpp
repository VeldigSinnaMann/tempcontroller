// main.ino

#include "../lib/Joystick/joystick.h"
#include <PubSubClient.h>
#include <WiFi.h>
#include <Wire.h>
#include <ArduinoJson.h>

// Replace the next variables with your SSID/Password combination
const char* ssid = "Phone 2";
const char* password = "AgnesErHot21";

// Add your MQTT Broker IP address, example:
const char* mqtt_server = "192.168.226.41";  // Raspberry Pi IP address
WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

void setup_wifi() {
    delay(10);
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* message, unsigned int length) {
    Serial.print("Message arrived on topic: ");
    Serial.print(topic);
    Serial.print(". Message: ");
    String messageTemp;

    for (int i = 0; i < length; i++) {
        messageTemp += (char)message[i];
    }
    Serial.println(messageTemp);
}

void reconnect() {
    while (!client.connected()) {
        Serial.print("Attempting MQTT connection...");
        if (client.connect("ESP8266Client")) {
            Serial.println("connected");
            client.subscribe("esp32/output");
        } else {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println(" try again in 5 seconds");
            delay(5000);
        }
    }
}

void setup() {
    setupJoystick();
    setup_wifi();
    client.setServer(mqtt_server, 1883);
    client.setCallback(callback);
}

void loop() {
    if (!client.connected()) {
        reconnect();
    }
    client.loop();

    readAndUpdateJoystick(); // Update joystick values
    long now = millis();
    if (now - lastMsg > 100) {
        lastMsg = now;

        // Generate JSON document with joystick values
        DynamicJsonDocument doc(1024);
        doc["leftRight"] = DirLeftRightVal;
        doc["upDown"] = DirUpDownVal;

        String output;
        serializeJson(doc, output);

        // Publish to MQTT
        client.publish("esp32/kontrollerOutput", output.c_str(), true); // The true parameter may enable QoS 1 or 2 depending on the library implementation

    }
}
