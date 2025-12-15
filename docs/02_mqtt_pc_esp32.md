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
  - `MQTTClient` (Arduino MQTTClient library - installed "MQTT" by Joel Gaehwiler)

---

## Step 1 — Install Mosquitto (portable on USB)
1. Download and run the Mosquitto Windows installer.
2. Install to USB path, e.g.:
   - `D:\Apps\Mosquitto\`
3. Components:
   - **Files** - yes
   - **Service** - no (OFF for USB/portable use, otherwise yes)
   - **Visual Studio Runtime** - optional (for me: yes)

---

## Step 2 — Configure Mosquitto to allow LAN connections
Mosquitto often needs explicit listener config to accept connections from other devices on the LAN.

Edit / create:
- `D:\Apps\Mosquitto\mosquitto.conf`

Add / ensure these lines exist:

```conf
listener 1883 0.0.0.0
allow_anonymous true

> `allow_anonymous true` is OK for a closed local network only.
> For real deployments, authentification needs to be configured (username, password)

---

## Step 3 — Test MQTT works PC ↔ PC (same device)
> should work correctly before using ESP32

### 3.1 Start the broker (leave running)
Open Command Promp:

```bat
cd /d D:\Apps\Mosquitto
mosquitto.exe -c mosquitto.conf -v
```

Expected log line:
```
Opening ipv4 listen socket on port 1883.
```

### 3.2 Test using localhost 
Open **two** terminals (PowerShell) in `D:\Apps\Mosquitto`.

**Terminal A (subscriber):**
```powershell
cd D:\Apps\Mosquitto
.\mosquitto_sub -h localhost -t campus02/test -v
```

**Terminal B (publisher):**
```powershell
cd D:\Apps\Mosquitto
.\mosquitto_pub -h localhost -t campus02/test -m "PC_TO_PC_LOCALHOST"
```

Expected output in Terminal A:
```
campus02/test PC_TO_PC_LOCALHOST
```

### 3.3 Test using LAN IP on the PC
> Purpose: prove Mosquitto is reachable on the **Wi-Fi interface**, not only localhost.

Find the PC Wi-Fi IPv4 address:
```powershell
ipconfig
```

Example Wi-Fi IPv4 (PC): `192.168.0.103`

Repeat pub/sub using the Wi-Fi IP:

**Terminal A (subscriber):**
```powershell
.\mosquitto_sub -h 192.168.0.103 -t campus02/test -v
```

**Terminal B (publisher):**
```powershell
.\mosquitto_pub -h 192.168.0.103 -t campus02/test -m "PC_TO_PC_LAN"
```

Expected output in Terminal A:
```
campus02/test PC_TO_PC_LAN
```

## Step 4 — ESP32 MQTT-only test (PC → Broker → ESP32)
Upload the MQTT sketch to the ESP32 and open Serial Monitor at **115200**.

Expected ESP32 output:
- Wi-Fi connects and prints ESP IP (example: `192.168.0.105`)
- MQTT connects and subscribes to `campus02/cmd`
- Incoming MQTT messages are printed by `messageHandler()`

### 4.1 Subscribe to ESP32 status topic on the PC
```powershell
cd D:\Apps\Mosquitto
.\mosquitto_sub -h 192.168.0.103 -t campus02/status -v
```

Expected message once ESP32 connects:
```
campus02/status ESP32 connected
```

### 4.2 Publish a command to ESP32
```powershell
cd D:\Apps\Mosquitto
.\mosquitto_pub -h 192.168.0.103 -t campus02/cmd -m "ADA1"
```

Expected on ESP32 Serial Monitor:
```
MQTT RX topics = campus02/cmd payload = ADA1
```

---
