# ATmega328P 4x4 Keypad UART Interface

This project demonstrates how to interface a **4×4 matrix keypad** with an **ATmega328P microcontroller** and transmit the pressed keys over **UART** at **115200 baud**. The code is designed for use with **Proteus simulations** and real hardware setups.

## Features

- Detects key presses from a standard 4×4 keypad
- Sends the corresponding character via UART
- Supports all keys: 0-9, A-D, *, #
- Debouncing included to avoid multiple readings
- Works with Proteus simulation or real ATmega328P boards

## Wiring (Proteus)

| Pin | Connection |
|-----|------------|
| PB0-PB3 | Keypad Rows |
| PD4-PD7 | Keypad Columns |
| UART TX | Terminal or Serial Monitor |

## How to Use

1. Compile the project in **Atmel Studio** or your preferred AVR IDE.
2. Upload the code to the **ATmega328P**.
3. Open a UART terminal (Proteus or hardware) at **115200 baud**.
4. Press any key on the 4×4 keypad and observe the character on the terminal.

## Code Structure

- `main.cpp` – Initializes UART, scans keypad, and sends characters
- `read_keypad()` – Scans the keypad matrix and returns the pressed key index
- UART functions:
  - `UART_init()`
  - `UART_send_char()`
  - `UART_send_string()`
  - `UART_send_int()`

## Requirements

- ATmega328P microcontroller
- 4×4 Matrix Keypad
- Proteus simulation (optional)
- AVR IDE / Compiler

## Author

**Usman Malik**  
