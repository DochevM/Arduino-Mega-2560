## Mobile Application overview
![alt text](https://github.com/DochevM/Arduino-Mega-2560/blob/main/Documents/Home_screen.jpg)
![alt text](https://github.com/DochevM/Arduino-Mega-2560/blob/main/Documents/Buttons_Screen.jpg)
## Connections

This table provides the pin connections between the Arduino Mega and various modules used in the smart home system.

| **Arduino Mega Pin** | **Component**       | **Vcc Pin**         | **Data Pin** / **Other**  | **GND Pin**       |
|----------------------|---------------------|---------------------|---------------------------|-------------------|
| 5V                   | DHT11               | Vcc                 | D24 (Data)                 | GND               |
| 5V                   | PIR Sensor          | Vcc                 | D25 (Data)                 | GND               |
| 5V                   | Ultrasonic Sensor   | Vcc                 | D11 (Trig), 10 (Echo)       | GND               |
| 5V                   | RFID (MFRC522)      | Vcc                 | D53 (SS), D48 (RST), D52 (SCK), D51 (MOSI), D50 (MISO) | GND |
| 5V                   | OLED Display        | Vcc                 | D20 (SDA), D21 (SCL)         | GND               |
| 5V                   | Gas Sensor (MQ)     | Vcc                 | A0 (Analog Output)         | GND               |
| 5V                   | Bluetooth Module    | Vcc                 | RX1 (D19) (TX), TX1 (D18) (RX) | GND            |
| 5V                   | Keypad              | Vcc                 | (Multiple digital pins for rows/columns) | GND |

# Arduino Mega Pin Connections for 4x4 Keypad

This table provides the pin connections between the Arduino Mega and the 4x4 Keypad based on the code you provided.

| **Arduino Mega Pin** | **Keypad Pin**      | **Function**   |
|----------------------|---------------------|----------------|
| D9                   | Keypad Row 1        | Row 1          |
| D8                   | Keypad Row 2        | Row 2          |
| D7                   | Keypad Row 3        | Row 3          |
| D6                   | Keypad Row 4        | Row 4          |
| D5                   | Keypad Column 1     | Column 1       |
| D4                   | Keypad Column 2     | Column 2       |
| D3                   | Keypad Column 3     | Column 3       |
| D2                   | Keypad Column 4     | Column 4       |

### Notes:
- The keypad rows are connected to pins `9`, `8`, `7`, and `6` on the Arduino Mega.
- The keypad columns are connected to pins `5`, `4`, `3`, and `2`.


![alt text](https://github.com/DochevM/Arduino-Mega-2560/blob/main/Documents/Arduino_mega_pinnout.png)
![alt text](https://github.com/DochevM/Arduino-Mega-2560/blob/main/Documents/keypad_pinout.png)