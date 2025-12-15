// Part of this ESP32 code is created by esp32io.com


#include <WiFi.h>
#include <MQTTClient.h>
#include <ArduinoJson.h>

const char WIFI_SSID[] = "511";
const char WIFI_PASSWORD[] = "4414563";

const char MQTT_BROKER_ADDRESS[] = "192.168.0.103"; // the PC IP running mosquitto
const int MQTT_PORT = 1883;

//const char MQTT_CLIENT_ID[] = "Campus02-esp";
//const char MQTT_USERNAME[] = "";
//const char MQTT_PASSWORD[] = "";

const char PUBLISH_TOPIC[] = "campus02/status";
const char SUBSCRIBE_TOPIC[] = "campus02/cmd";

//const char PUBLISH_INTERVAL = 5000; // 5 sec

WiFiClient network;
MQTTClient mqtt = MQTTClient(256);

//unsigned long lastPublishedTime = 0;

String clientId;

//void messageHandler(String &topic, String &payload) {
//  Serial.println("ESP32 - received from MQTT:");
//  Serial.print("- topic: " + topic);
//  Serial.println("- payload:");
//  Serial.print(payload);
//}

void messageHandler(String &topic, String &payload) {
  Serial.print("MQTT RX topics = ");
  Serial.print(topic);
  Serial.print(" payload = ");
  Serial.println(payload);
}

void connectWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  Serial.println("ESP32 - Connecting to WiFi ");
  
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("WIFI OK, IP = "); 
  Serial.println(WiFi.localIP());
}


void connectMQTT() {
  // connect to the mqtt broker
  mqtt.begin(MQTT_BROKER_ADDRESS, MQTT_PORT, network);

  // create a handler for incoming messages 
  mqtt.onMessage(messageHandler);

  Serial.print("ESP32 - connecting to MQTT broker");

  while(!mqtt.connect(clientId.c_str())) {
    delay(500);
    Serial.print(".");
    delay(100);
  }
  Serial.println();

  if(!mqtt.connected()){
    Serial.println("ESP32 - MQTT broker timeout!");
    return;
  }

  // subscribe to a topic, the incoming messages are processed by messageHandler()
  if(mqtt.subscribe(SUBSCRIBE_TOPIC)){
    Serial.print("ESP32 - subscribed to the topic: ");
  } else {
    Serial.print("ESP32 - failed to subscribe to the topic : ");
  }

  Serial.println(SUBSCRIBE_TOPIC);
  Serial.println("ESP32 - MQTT broker connected!");

  mqtt.publish(PUBLISH_TOPIC, "ESP32 connected");
}


void setup() {
  Serial.begin(115200);

  clientId = "Campus02-esp";

  connectWiFi();
  connectMQTT();

}

void loop() {
  mqtt.loop();

//  if(millis() - lastPublishTime > PUBLISH_INTERVAL) {
//    sendToMQTT();
//    lastPublishedTime = millis();
//  }
  if(!mqtt.connected()){
    connectMQTT();
  }

  delay(10);
}

//void sendToMQTT() {
//  StaticJsonDocument<200> message;
//  message["timestamp"] = millis();
//  message["data"] = analogRead(0); // or other sensors
//  char messageBuffer[512];
//  serializeJson(message, messageBuffer);
//
//  mqtt.publish(PUBLISH_TOPIC, messageBuffer);
//
//  Serial.println("ESP32 - sent to MQTT:");
//  Serial.print("- topic: ");
//  Serial.println(PBLISH_TOPIC);
//  Serial.print("- payload:");
//  Serial.println(messageBuffer);
//}
