# ESP8266 4WD Robot (NodeMCU + L298N)

## Files
- `ESP8266_4WD.ino` : Arduino sketch (main code)
- `README.md` : this file

## Wiring (match the blueprint)
Use a single L298N motor driver module. Each side of the L298N controls two motors wired in parallel (front+rear on each side).

**Pin mapping (NodeMCU / ESP8266):**
- ENA (Left motors PWM)  -> D1 (GPIO5)
- IN1 (Left motor A dir) -> D5 (GPIO14)
- IN2 (Left motor B dir) -> D6 (GPIO12)

- ENB (Right motors PWM) -> D2 (GPIO4)
- IN3 (Right motor A dir)-> D7 (GPIO13)
- IN4 (Right motor B dir)-> D8 (GPIO15)

**Power:**
- Motor power (12V or appropriate for your motors) -> L298N `VMS` / `VCC` (external battery pack)
- L298N `GND` -> Common ground with NodeMCU GND
- L298N `5V` (logic) -> If your L298N has onboard regulator and you power motors with >7V, you can use its 5V output to feed NodeMCU `VIN`. Otherwise power NodeMCU separately via USB or regulated 5V to VIN.
- NodeMCU `GND` -> Common ground with battery and L298N.

**Notes:**
- Wire the two left motors in parallel to the left output terminals of L298N, and the two right motors in parallel to the right output terminals. This matches a 4WD setup where left wheels move together and right wheels move together.
- Make sure grounds are common.
- Ensure the battery can supply sufficient current for 4 motors.

## Setup
1. Replace `YOUR_SSID` and `YOUR_PASSWORD` in `ESP8266_4WD.ino` with your Wi‑Fi credentials.
2. Open the Arduino IDE:
   - Install ESP8266 board support (if not already): Boards Manager -> search `esp8266` -> install `esp8266 by ESP8266 Community`.
   - Select the correct board (NodeMCU 1.0 (ESP-12E Module)) and the correct COM port.
3. Copy `ESP8266_4WD.ino` contents into a new sketch (or use provided file) and upload.
4. After boot, open the Serial Monitor at 115200 to find the IP address (if needed). The code tries to connect for 10s; if not connected it will still start the webserver in AP mode is not implemented — connect your NodeMCU to the same WiFi network.
5. Open a browser and go to `http://<NODE_IP>/` to access the control web page.

## Controls
- Use the webpage buttons to move: Forward / Back / Left / Right / Stop.
- Speed slider adjusts PWM from 0 (stop) to 255 (full speed).

## Troubleshooting
- If the ESP8266 does not boot after wiring, double-check pins used for boot mode (GPIO0, GPIO2, GPIO15). If needed you can change pin assignments in the sketch to avoid boot-critical pins.
- If motors jitter, check power and wiring; ensure grounds are connected.
- For best results use a separate power supply for motors and for the NodeMCU.