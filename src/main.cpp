// #include <Arduino.h>
// #include <WiFi.h>

// // Structure to hold network data
// struct WifiNetwork {
//     String ssid;
//     wifi_auth_mode_t authMode;
//     int32_t rssi;
// };

// // Function to convert auth mode enum to an integer weight for sorting
// int getSecurityWeight(wifi_auth_mode_t authMode) {
//     switch (authMode) {
//         case WIFI_AUTH_WPA3_PSK: return 6;
//         case WIFI_AUTH_WPA2_WPA3_PSK: return 5;
//         case WIFI_AUTH_WPA2_PSK: return 4;
//         case WIFI_AUTH_WPA_PSK: return 3;
//         case WIFI_AUTH_WEP: return 2;
//         case WIFI_AUTH_OPEN: return 1;
//         default: return 0; // Unknown
//     }
// }

// // Helper to print human-readable auth names
// String getAuthTypeName(wifi_auth_mode_t authMode) {
//     switch (authMode) {
//         case WIFI_AUTH_OPEN: return "OPEN";
//         case WIFI_AUTH_WEP: return "WEP";
//         case WIFI_AUTH_WPA_PSK: return "WPA";
//         case WIFI_AUTH_WPA2_PSK: return "WPA2";
//         case WIFI_AUTH_WPA2_WPA3_PSK: return "WPA2/WPA3";
//         case WIFI_AUTH_WPA3_PSK: return "WPA3";
//         default: return "UNKNOWN";
//     }
// }

// void setup() {
//     Serial.begin(115200);
//     WiFi.mode(WIFI_STA);
//     WiFi.disconnect();
//     delay(100);
//     Serial.println("Setup done. Starting scan...");
// }

// void loop() {
//     int n = WiFi.scanNetworks();
//     if (n == 0) {
//         Serial.println("No networks found.");
//     } else {
//         Serial.printf("%d networks found.\n", n);

//         // Populate our network array
//         WifiNetwork networks[n];
//         for (int i = 0; i < n; ++i) {
//             networks[i].ssid = WiFi.SSID(i);
//             networks[i].authMode = WiFi.encryptionType(i);
//             networks[i].rssi = WiFi.RSSI(i);
//         }

//         // Bubble sort from strongest to weakest security
//         for (int i = 0; i < n - 1; i++) {
//             for (int j = 0; j < n - i - 1; j++) {
//                 if (getSecurityWeight(networks[j].authMode) < getSecurityWeight(networks[j + 1].authMode)) {
//                     WifiNetwork temp = networks[j];
//                     networks[j] = networks[j + 1];
//                     networks[j + 1] = temp;
//                 }
//             }
//         }

//         // Print sorted results
//         Serial.println("\n--- Networks Sorted by Security Strength ---");
//         for (int i = 0; i < n; ++i) {
//             Serial.printf("%d: %s | Security: %s | Signal: %d dBm\n", 
//                           i + 1, 
//                           networks[i].ssid.c_str(), 
//                           getAuthTypeName(networks[i].authMode).c_str(), 
//                           networks[i].rssi);
//         }
//     }
//     Serial.println("--------------------------------------------");
    
//     // Wait 10 seconds before scanning again
//     delay(10000); 
// }



#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "AkiraCHix";
const char* password = "Akirachix@2011";

WebServer server(80);


void handleRoot() {
    int n = WiFi.scanNetworks();
    String html = "<html><head><title>ESP32 Security Scanner</title>";
    html += "<style>body{font-family:Arial; margin:40px;} th,td{padding:10px; border:1px solid #ddd;}</style></head><body>";
    html += "<h2>WiFi Security Scan Results</h2>";
    html += "<table><tr><th>SSID</th><th>Signal (dBm)</th></tr>";

    if (n == 0) {
        html += "<tr><td colspan='2'>No networks found</td></tr>";
    } else {
        for (int i = 0; i < n; ++i) {
            html += "<tr><td>" + WiFi.SSID(i) + "</td><td>" + String(WiFi.RSSI(i)) + " dBm</td></tr>";
        }
    }
    
    html += "</table></body></html>";
    server.send(200, "text/html", html);
}

void setup() {
    Serial.begin(115200);
    
    
    WiFi.begin(ssid, password);
    Serial.print("Connecting to WiFi");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    
    //  local IP address
    Serial.println("\nConnected!");
    Serial.print("Type this IP address into your browser: ");
    Serial.println(WiFi.localIP());

    
    server.on("/", handleRoot);
    server.begin();
}

void loop() {
    server.handleClient(); 
}
