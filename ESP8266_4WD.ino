#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

// --- WIFI ---
const char* ssid = "YOUR_SSID";
const char* password = "YOUR_PASSWORD";

// --- PIN MAP (NodeMCU ESP8266) ---
// Motor A (Left pair)
const uint8_t ENA = D1; // PWM speed control
const uint8_t IN1 = D5;
const uint8_t IN2 = D6;

// Motor B (Right pair)
const uint8_t ENB = D2; // PWM speed control
const uint8_t IN3 = D7;
const uint8_t IN4 = D8;

ESP8266WebServer server(80);

uint8_t speedVal = 200; // 0-255

void stopMotors() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}

void forwardMove() {
  // Left forward
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  // Right forward
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, speedVal);
  analogWrite(ENB, speedVal);
}

void backMove() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENA, speedVal);
  analogWrite(ENB, speedVal);
}

void leftMove() {
  // Turn left: left motors slower/reverse, right motors forward
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH); // left backwards
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);  // right forward
  analogWrite(ENA, speedVal);
  analogWrite(ENB, speedVal);
}

void rightMove() {
  // Turn right: right motors backwards, left motors forward
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);  // left forward
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH); // right backwards
  analogWrite(ENA, speedVal);
  analogWrite(ENB, speedVal);
}

String webpage = R"rawliteral(
<!DOCTYPE html>
<html>
  <head>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <title>ESP8266 4WD Control</title>
    <style>
      body { font-family: Arial, Helvetica, sans-serif; text-align:center; margin-top:30px; }
      button { width:120px; height:60px; margin:8px; font-size:18px; }
      input[type=range] { width:80%; }
    </style>
  </head>
  <body>
    <h2>ESP8266 4WD Robot</h2>
    <div>
      <button onclick="action('forward')">Forward</button><br>
      <button onclick="action('left')">Left</button>
      <button onclick="action('stop')">Stop</button>
      <button onclick="action('right')">Right</button><br>
      <button onclick="action('back')">Back</button>
    </div>
    <p>Speed: <span id="speedVal">200</span></p>
    <input type="range" id="speed" min="0" max="255" value="200" oninput="updateSpeed(this.value)">
    <script>
      function action(a){
        fetch('/cmd?a='+a).catch(()=>{});
      }
      function updateSpeed(v){
        document.getElementById('speedVal').innerText = v;
        fetch('/speed?v='+v).catch(()=>{});
      }
    </script>
  </body>
</html>
)rawliteral";

void handleRoot() {
  server.send(200, "text/html", webpage);
}

void handleCmd() {
  if (!server.hasArg("a")) { server.send(400, "text/plain", "Missing"); return; }
  String a = server.arg("a");
  if (a == "forward") forwardMove();
  else if (a == "back") backMove();
  else if (a == "left") leftMove();
  else if (a == "right") rightMove();
  else stopMotors();
  server.send(200, "text/plain", "OK");
}

void handleSpeed() {
  if (!server.hasArg("v")) { server.send(400, "text/plain", "Missing"); return; }
  int v = server.arg("v").toInt();
  if (v < 0) v = 0;
  if (v > 255) v = 255;
  speedVal = v;
  analogWrite(ENA, speedVal);
  analogWrite(ENB, speedVal);
  server.send(200, "text/plain", "OK");
}

void setup() {
  // pins
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  // safe stop
  stopMotors();

  // WiFi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  // Wait for connection (brief)
  unsigned long start = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - start < 10000) {
    delay(200);
  }

  // Start web server
  server.on("/", handleRoot);
  server.on("/cmd", handleCmd);
  server.on("/speed", handleSpeed);
  server.begin();
}

void loop() {
  server.handleClient();
}