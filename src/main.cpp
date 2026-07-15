#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>

WebServer Akira(50);
const int LED = 2;
const char* SSID = "akirachix";
const char* Pass = "akirachix@2011";

int blinkDelay = 300;
unsigned long previousMillis = 0;
bool ledState = LOW;
String scannedWifiList = "";

String page(const String& wifiList) {
  String html = R"raw(
<!DOCTYPE html>
<html lang='en'>
<head>
    <meta charset='UTF-8'>
    <meta name='viewport' content='width=device-width, initial-scale=1.0'>
    <title>Akira ESP32 Control</title>
    <style>
        :root {
            --background-color: #3b3b3b;
            --card-bg: #FFFFFF;
            --border-color: #E1E4EA;
            --text-main: #c5dbff;
            --text-muted: #ffffff;
            --primary-navy: #020A5E;
            --accent-green: #0E382A;
            --accent-green-hover: #059669;
        }
        * { box-sizing: border-box; margin: 0; padding: 0; }
        body {
            background-color: var(--background-color);
            color: var(--text-main);
            font-family: sans-serif;
            display: flex;
            flex-direction: column;
            align-items: center;
            padding: 40px 20px;
        }
        h1 { color: var(--primary-navy); margin-bottom: 30px; }
        .container { display: flex; flex-wrap: wrap; justify-content: center; gap: 24px; max-width: 1000px; width: 100%; }
        .card {
            background-color: var(--card-bg);
            border: 1px solid var(--border-color);
            border-radius: 12px;
            padding: 24px;
            width: 100%;
            max-width: 360px;
            box-shadow: 0 4px 6px rgba(0,0,0,0.05);
        }
        h2 { font-size: 1.25rem; margin-bottom: 16px; border-bottom: 2px solid var(--background-color); padding-bottom: 8px; }
        .card-navy h2 { color: var(--primary-navy); }
        .card-green h2 { color: var(--accent-green); }
        ul { list-style: none; max-height: 220px; overflow-y: auto; }
        li { padding: 10px 12px; background: var(--background-color); border-radius: 6px; margin-bottom: 8px; font-size: 0.9rem; }
        .form-group { margin-bottom: 16px; }
        label { display: block; font-size: 0.85rem; color: var(--text-muted); margin-bottom: 6px; }
        input[type='number'] {
            width: 100%; padding: 10px 12px; background-color: var(--background-color);
            border: 1px solid var(--border-color); border-radius: 6px; outline: none;
        }
        button {
            width: 100%; padding: 12px; background-color: var(--accent-green); color: #FFFFFF;
            border: none; border-radius: 6px; font-weight: 600; cursor: pointer;
        }
        button:hover { background-color: var(--accent-green-hover); }
        .status-text { margin-top: 18px; text-align: center; background: var(--background-color); padding: 10px; border-radius: 6px; }
        .status-text b { color: var(--primary-navy); }
    </style>
</head>
<body>
    
    <div class='container'>
        <div class='card card-navy'>
            <h2>Scanned Wi-Fi</h2>
            <ul>
  )raw";
  html += wifiList;
  html += R"raw(
            </ul>
        </div>
        <div class='card card-green'>
            <h2>Blink Control</h2>
            <form action='/set' method='GET'>
                <div class='form-group'>
                    <label>Delay Configuration (ms)</label>
                    <input type='number' name='ms' required>
                </div>
                <button type='submit'>Change Delay</button>
            </form>
  )raw";
  html += "<div class='status-text'>Current delay: <b>" + String(blinkDelay) + " ms</b></div>";
  html += R"raw(
        </div>
    </div>
</body>
</html>
  )raw";
  return html;
}

void handleRoute() {
  Akira.send(200, "text/html", page(scannedWifiList));
}

void handleSetDelay() {
  if (Akira.hasArg("ms")) {
    blinkDelay = Akira.arg("ms").toInt();
    if (blinkDelay < 10) blinkDelay = 10;
  }
  Akira.sendHeader("Location", "/");
  Akira.send(303);
}

void setup() {
  Serial.begin(115200);
  pinMode(LED, OUTPUT);
  
  WiFi.mode(WIFI_AP_STA);
  WiFi.disconnect();
  delay(100);
  
  int n = WiFi.scanNetworks();
  if (n == 0) {
    scannedWifiList = "<li>No networks found</li>";
  } else {
    for (int i = 0; i < n; ++i) {
      scannedWifiList += "<li>" + WiFi.SSID(i) + " (" + String(WiFi.RSSI(i)) + " dBm)</li>";
    }
  }
  
  WiFi.begin(SSID, Pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  
  Akira.on("/", handleRoute);
  Akira.on("/set", handleSetDelay);
  Akira.begin();
}

void loop() {
  Akira.handleClient();
  
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= (unsigned long)blinkDelay) {
    previousMillis = currentMillis;
    ledState = !ledState;
    digitalWrite(LED, ledState);
  }
}

