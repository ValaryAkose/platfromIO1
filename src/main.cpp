#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>

const char* ssid     = "ANITAB LAB";       
const char* password = "Akirachix@2011";   

#define STATUS_LED 2 

WebServer server(80);

int getSecurityWeight(wifi_auth_mode_t authMode) {
    switch (authMode) {
        case WIFI_AUTH_WPA3_PSK: return 6;
        case WIFI_AUTH_WPA2_WPA3_PSK: return 5;
        case WIFI_AUTH_WPA2_PSK: return 4;
        case WIFI_AUTH_WPA_PSK: return 3;
        case WIFI_AUTH_WEP: return 2;
        case WIFI_AUTH_OPEN: return 1;
        default: return 0;
    }
}

String getSecurityDetails(wifi_auth_mode_t authMode, String &color) {
    switch (authMode) {
        case WIFI_AUTH_WPA3_PSK: color = "#2ecc71"; return "WPA3 (Strongest)";
        case WIFI_AUTH_WPA2_WPA3_PSK: color = "#2ecc71"; return "WPA2/WPA3 Mixed";
        case WIFI_AUTH_WPA2_PSK: color = "#3498db"; return "WPA2 (Standard)";
        case WIFI_AUTH_WPA_PSK: color = "#e67e22"; return "WPA (Legacy/Weak)";
        case WIFI_AUTH_WEP: color = "#e74c3c"; return "WEP (Vulnerable)";
        case WIFI_AUTH_OPEN: color = "#e74c3c"; return "OPEN (No Password!)";
        default: color = "#7f8c8d"; return "UNKNOWN";
    }
}

void handleRoot() {
    Serial.println("\n[Web Request] Client connected. Scanning airwaves...");

    for (int i = 0; i < 4; i++) {
        digitalWrite(STATUS_LED, HIGH); delay(75);
        digitalWrite(STATUS_LED, LOW);  delay(75);
    }

    int n = WiFi.scanNetworks();
    
    String html = "<html><head><meta name='viewport' content='width=device-width, initial-scale=1'>";
    html += "<meta http-equiv='refresh' content='15'>"; 
    html += "<style>";
    html += "body { font-family: 'Segoe UI', Arial, sans-serif; background-color: #f4f6f9; color: #333; margin: 0; padding: 20px; display: flex; justify-content: center; }";
    html += ".card { background: white; padding: 25px; border-radius: 12px; box-shadow: 0 4px 15px rgba(0,0,0,0.1); width: 100%; max-width: 550px; }";
    html += "h2 { color: #2c3e50; text-align: center; margin-top: 0; border-bottom: 2px solid #eceff1; padding-bottom: 10px; }";
    html += "table { width: 100%; border-collapse: collapse; margin-top: 15px; }";
    html += "th, td { text-align: left; padding: 12px 8px; border-bottom: 1px solid #edf2f7; }";
    html += "th { background-color: #f7fafc; color: #4a5568; font-weight: 600; }";
    html += ".badge { padding: 5px 10px; border-radius: 20px; color: white; font-size: 11px; font-weight: bold; display: inline-block; }";
    html += ".btn { display: block; width: 100%; padding: 12px; background-color: #3498db; color: white; text-align: center; border: none; border-radius: 6px; font-size: 16px; font-weight: bold; text-decoration: none; margin-top: 20px; cursor: pointer; }";
    html += ".btn:hover { background-color: #2980b9; }";
    html += "</style></head><body>";
    
    html += "<div class='card'>";
    html += "<h2>📶 Wi-Fi Security Auditor</h2>";
    html += "<p style='text-align:center; color:#718096; margin-bottom:5px;'>Ranked from Strongest to Weakest</p>";
    html += "<p style='text-align:center; font-size:12px; color:#a0aec0;'>Page auto-refreshes every 15s</p>";
    html += "<table><tr><th>Network (SSID)</th><th>Signal</th><th>Security Strength</th></tr>";

    if (n == 0) {
        html += "<tr><td colspan='3' style='text-align:center;'>No networks detected nearby.</td></tr>";
        digitalWrite(STATUS_LED, LOW);
    } else {
        struct NetInfo { String ssid; int32_t rssi; wifi_auth_mode_t auth; };
        NetInfo* list = new NetInfo[n];
        bool hasOpenNetwork = false;

        for (int i = 0; i < n; i++) { 
            list[i] = {WiFi.SSID(i), WiFi.RSSI(i), WiFi.encryptionType(i)}; 
            if (list[i].auth == WIFI_AUTH_OPEN) {
                hasOpenNetwork = true;
            }
        }

        for (int i = 0; i < n - 1; i++) {
            for (int j = 0; j < n - i - 1; j++) {
                if (getSecurityWeight(list[j].auth) < getSecurityWeight(list[j + 1].auth)) {
                    NetInfo temp = list[j]; list[j] = list[j + 1]; list[j + 1] = temp;
                }
            }
        }

        for (int i = 0; i < n; i++) {
            String badgeColor = "";
            String securityText = getSecurityDetails(list[i].auth, badgeColor);
            html += "<tr>";
            html += "<td><strong>" + list[i].ssid + "</strong></td>";
            html += "<td>" + String(list[i].rssi) + " dBm</td>";
            html += "<td><span class='badge' style='background-color:" + badgeColor + ";'>" + securityText + "</span></td>";
            html += "</tr>";
        }
        delete[] list;

        if (hasOpenNetwork) {
            digitalWrite(STATUS_LED, LOW);
            Serial.println("[Alert] Unsecured networks detected in this area!");
        } else {
            digitalWrite(STATUS_LED, HIGH);
        }
    }

    html += "</table>";
    html += "<a href='/' class='btn'>🔄 Scan Airwaves Now</a>";
    html += "</div></body></html>";
    
    server.send(200, "text/html", html);
    WiFi.scanDelete(); 
}

void setup() {
    Serial.begin(115200);
    pinMode(STATUS_LED, OUTPUT);
    digitalWrite(STATUS_LED, LOW); 

    WiFi.mode(WIFI_AP_STA); 
    WiFi.begin(ssid, password);
    
    Serial.print("Connecting to Wi-Fi network");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500); 
        Serial.print(".");
    }
    
    Serial.println("\n[Status] Connected Successfully!");
    Serial.print("[Action] Open your browser and go to: http://");
    Serial.println(WiFi.localIP());

    server.on("/", handleRoot);
    server.begin();
    Serial.println("[Status] HTTP Server Active.");
}

void loop() {
    server.handleClient(); 
}
