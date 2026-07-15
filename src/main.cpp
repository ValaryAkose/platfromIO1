// #include <Arduino.h>
// #include <WiFi.h>
// #include <WebServer.h>

// const char* ssid     = "ANITAB LAB";       
// const char* password = "Akirachix@2011";   

// #define STATUS_LED 2 

// WebServer server(80);

// int getSecurityWeight(wifi_auth_mode_t authMode) {
//     switch (authMode) {
//         case WIFI_AUTH_WPA3_PSK: return 6;
//         case WIFI_AUTH_WPA2_WPA3_PSK: return 5;
//         case WIFI_AUTH_WPA2_PSK: return 4;
//         case WIFI_AUTH_WPA_PSK: return 3;
//         case WIFI_AUTH_WEP: return 2;
//         case WIFI_AUTH_OPEN: return 1;
//         default: return 0;
//     }
// }

// String getSecurityDetails(wifi_auth_mode_t authMode, String &color) {
//     switch (authMode) {
//         case WIFI_AUTH_WPA3_PSK: color = "#2ecc71"; return "WPA3 (Strongest)";
//         case WIFI_AUTH_WPA2_WPA3_PSK: color = "#2ecc71"; return "WPA2/WPA3 Mixed";
//         case WIFI_AUTH_WPA2_PSK: color = "#3498db"; return "WPA2 (Standard)";
//         case WIFI_AUTH_WPA_PSK: color = "#e67e22"; return "WPA (Legacy/Weak)";
//         case WIFI_AUTH_WEP: color = "#e74c3c"; return "WEP (Vulnerable)";
//         case WIFI_AUTH_OPEN: color = "#e74c3c"; return "OPEN (No Password!)";
//         default: color = "#7f8c8d"; return "UNKNOWN";
//     }
// }

// void handleRoot() {
//     Serial.println("\n[Web Request] Client connected. Scanning airwaves...");

//     for (int i = 0; i < 4; i++) {
//         digitalWrite(STATUS_LED, HIGH); delay(75);
//         digitalWrite(STATUS_LED, LOW);  delay(75);
//     }

//     int n = WiFi.scanNetworks();
    
//     String html = "<html><head><meta name='viewport' content='width=device-width, initial-scale=1'>";
//     html += "<meta http-equiv='refresh' content='15'>"; 
//     html += "<style>";
//     html += "body { font-family: 'Segoe UI', Arial, sans-serif; background-color: #f4f6f9; color: #333; margin: 0; padding: 20px; display: flex; justify-content: center; }";
//     html += ".card { background: white; padding: 25px; border-radius: 12px; box-shadow: 0 4px 15px rgba(0,0,0,0.1); width: 100%; max-width: 550px; }";
//     html += "h2 { color: #2c3e50; text-align: center; margin-top: 0; border-bottom: 2px solid #eceff1; padding-bottom: 10px; }";
//     html += "table { width: 100%; border-collapse: collapse; margin-top: 15px; }";
//     html += "th, td { text-align: left; padding: 12px 8px; border-bottom: 1px solid #edf2f7; }";
//     html += "th { background-color: #f7fafc; color: #4a5568; font-weight: 600; }";
//     html += ".badge { padding: 5px 10px; border-radius: 20px; color: white; font-size: 11px; font-weight: bold; display: inline-block; }";
//     html += ".btn { display: block; width: 100%; padding: 12px; background-color: #3498db; color: white; text-align: center; border: none; border-radius: 6px; font-size: 16px; font-weight: bold; text-decoration: none; margin-top: 20px; cursor: pointer; }";
//     html += ".btn:hover { background-color: #2980b9; }";
//     html += "</style></head><body>";
    
//     html += "<div class='card'>";
//     html += "<h2>📶 Wi-Fi Security Auditor</h2>";
//     html += "<p style='text-align:center; color:#718096; margin-bottom:5px;'>Ranked from Strongest to Weakest</p>";
//     html += "<p style='text-align:center; font-size:12px; color:#a0aec0;'>Page auto-refreshes every 15s</p>";
//     html += "<table><tr><th>Network (SSID)</th><th>Signal</th><th>Security Strength</th></tr>";

//     if (n == 0) {
//         html += "<tr><td colspan='3' style='text-align:center;'>No networks detected nearby.</td></tr>";
//         digitalWrite(STATUS_LED, LOW);
//     } else {
//         struct NetInfo { String ssid; int32_t rssi; wifi_auth_mode_t auth; };
//         NetInfo* list = new NetInfo[n];
//         bool hasOpenNetwork = false;

//         for (int i = 0; i < n; i++) { 
//             list[i] = {WiFi.SSID(i), WiFi.RSSI(i), WiFi.encryptionType(i)}; 
//             if (list[i].auth == WIFI_AUTH_OPEN) {
//                 hasOpenNetwork = true;
//             }
//         }

//         for (int i = 0; i < n - 1; i++) {
//             for (int j = 0; j < n - i - 1; j++) {
//                 if (getSecurityWeight(list[j].auth) < getSecurityWeight(list[j + 1].auth)) {
//                     NetInfo temp = list[j]; list[j] = list[j + 1]; list[j + 1] = temp;
//                 }
//             }
//         }

//         for (int i = 0; i < n; i++) {
//             String badgeColor = "";
//             String securityText = getSecurityDetails(list[i].auth, badgeColor);
//             html += "<tr>";
//             html += "<td><strong>" + list[i].ssid + "</strong></td>";
//             html += "<td>" + String(list[i].rssi) + " dBm</td>";
//             html += "<td><span class='badge' style='background-color:" + badgeColor + ";'>" + securityText + "</span></td>";
//             html += "</tr>";
//         }
//         delete[] list;

//         if (hasOpenNetwork) {
//             digitalWrite(STATUS_LED, LOW);
//             Serial.println("[Alert] Unsecured networks detected in this area!");
//         } else {
//             digitalWrite(STATUS_LED, HIGH);
//         }
//     }

//     html += "</table>";
//     html += "<a href='/' class='btn'>🔄 Scan Airwaves Now</a>";
//     html += "</div></body></html>";
    
//     server.send(200, "text/html", html);
//     WiFi.scanDelete(); 
// }

// void setup() {
//     Serial.begin(115200);
//     pinMode(STATUS_LED, OUTPUT);
//     digitalWrite(STATUS_LED, LOW); 

//     WiFi.mode(WIFI_AP_STA); 
//     WiFi.begin(ssid, password);
    
//     Serial.print("Connecting to Wi-Fi network");
//     while (WiFi.status() != WL_CONNECTED) {
//         delay(500); 
//         Serial.print(".");
//     }
    
//     Serial.println("\n[Status] Connected Successfully!");
//     Serial.print("[Action] Open your browser and go to: http://");
//     Serial.println(WiFi.localIP());

//     server.on("/", handleRoot);
//     server.begin();
//     Serial.println("[Status] HTTP Server Active.");
// }

// void loop() {
//     server.handleClient(); 
// }





#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>

WebServer viona(80);
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
    <title>Viona ESP32 Control</title>
    <style>
        :root {
            --background-color: #F4F6F9;
            --card-bg: #FFFFFF;
            --border-color: #E1E4EA;
            --text-main: #2D3748;
            --text-muted: #4A5568;
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
    <h1>Viona Kyalo</h1>
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
  viona.send(200, "text/html", page(scannedWifiList));
}

void handleSetDelay() {
  if (viona.hasArg("ms")) {
    blinkDelay = viona.arg("ms").toInt();
    if (blinkDelay < 10) blinkDelay = 10;
  }
  viona.sendHeader("Location", "/");
  viona.send(303);
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
  
  viona.on("/", handleRoute);
  viona.on("/set", handleSetDelay);
  viona.begin();
}

void loop() {
  viona.handleClient();
  
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= (unsigned long)blinkDelay) {
    previousMillis = currentMillis;
    ledState = !ledState;
    digitalWrite(LED, ledState);
  }
}

