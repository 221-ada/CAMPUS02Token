# 01 – Basic UART Bridge (ESP32 ↔ STM32)

## Overview

The ESP32-S3 and STM32F3DISCOVERY exchange a byte over UART.

- **ESP32 role:** USB–UART bridge between the PC and STM32.
- **STM32 role:** Waits for bytes on USART2; when it receives `'1'`
  it toggles an on-board LED and prints a debug message over USB.

## Hardware

- STM32F3DISCOVERY (STM32F303)
- ESP32-S3 WROOM (Freenove board)

### Wiring (USART2 on STM32)

| STM32 Pin (USART2) | ESP32 Pin               | Notes                       |
|--------------------|-------------------------|-----------------------------|
| PA2 (USART2_TX)    | ESP_RX1 (RX - GPIO 18)  | STM32 → ESP32               |
| PA3 (USART2_RX)    | ESP_TX1 (TX - GPIO 17)  | ESP32 → STM32               |
| GND                | GND                     | Common reference            |

Baud rate: **115200**, 8 data bits, no parity, 1 stop bit.

## Firmware behavior

### ESP32 (`esp32/`)

- `Serial` (USB) ↔ PC at 115200.
- `HardwareSerial STM(1)` ↔ STM32 at 115200, pins `ESP_RX1` / `ESP_TX1`.
- In the main loop:
  - Forwards all bytes from PC (`Serial`) to STM (`STM`).
  - Forwards all bytes from STM (`STM`) to PC (`Serial`).

Typing `1` in the Arduino Serial Monitor and sending it to STM32.

### STM32 (`stm32/`)

- Initializes GPIO, USB, and USART2.
- Waits 5 seconds.
- Prints:
    (if it doesn't print press the reset button on the board)
  - `"STM32 started"`
  - `"Waiting for UART commands on USART2 ..."`
- In the main loop:
  - Calls `HAL_UART_Receive(&huart2, &rx, 1, 10)` to read one byte with a 10 ms timeout.
  - If `rx == '1'`:
    - Toggles LED (e.g. `GPIOE, LED_PINS[3]`).
    - Prints `"Got '1' -> LED toggled"`.
  - Otherwise prints `"Got byte: "` (for future debugging).

## How to test

1. Flash STM32 firmware.
2. Flash ESP32 sketch.
3. Connect both boards with the UART wiring and common GND.
4. Open serial terminal at 115200 with correct com port:
5. Type `1` in the ESP32 serial monitor and press **Send**.
   - STM32 log: `Got '1' -> LED toggled`.
   - One on-board LED toggles state.



two-way UART communication is working.
