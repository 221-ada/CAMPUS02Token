# 02 – MQTT Bridge (PC Mosquitto Broker ↔ ESP32-S3 MQTT Client)

## The Goal
Test end-to-end MQTT connectivity **PC ↔ MQTT Broker ↔ ESP32-S3** before adding UART forwarding to STM32.

- PC runs **Mosquitto broker**
- ESP32 connects to Wi-Fi and subscribes to a command topic
- PC publishes messages and ESP32 prints received payloads over USB Serial

---

## Hardware / Software
- **PC (Windows)** with Mosquitto installed (can also be installed on USB stick)
- **ESP32-S3** (Freenove board)
- Arduino IDE + libraries:
  - `WiFi.h`
  - `MQTTClient` (Arduino MQTTClient library - i've installed the one from Joel Gaehwiler)

---

## Step 1 — Install Mosquitto (portable on USB)
1. Download and run the Mosquitto Windows installer.
2. Install to USB path, e.g.:
   - `D:\Apps\Mosquitto\`
3. Components:
   - Files - yes
   - Service - no (recommended OFF for USB/portable use)
   - Visual Studio Runtime optional, for me - yes

---

## Step 2 — Configure Mosquitto to allow LAN connections
Mosquitto often needs explicit listener config to accept connections from other devices on the LAN.

Edit / create:
- `D:\Apps\Mosquitto\mosquitto.conf`

Add / ensure these lines exist:

```conf
listener 1883 0.0.0.0
allow_anonymous true
