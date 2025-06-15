#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "xxxxxxxxx";  // wifi username
const char* password = "xxxxxxxxx"; // wifi password

// Replace with your actual Google Apps Script Web App URL
const char* scriptURL = "https://script.google.com/macros/s/AKfycbznoC07PuYi8F2X6zg6qFQWFgcAZ2X4F5KR_aPwNof8tKX9ITT0zotSZ4-wzeu-1fEArA/exec";

void setup() {
  Serial.begin(115200);                // USB Serial Monitor
  Serial2.begin(9600, SERIAL_8N1, 17, 16);  // RX=17, TX=16 from Mega

  WiFi.begin(ssid, password);
  Serial.println("📶 Connecting to WiFi...");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\n✅ WiFi connected.");
}

void loop() {
  if (Serial2.available()) {
    String rawData = Serial2.readStringUntil('\n');
    rawData.trim();
    Serial.println("📨 Received from Mega: " + rawData);

    // Example: Tilt:12,Temp:25.3,Hum:68,Soil:40,Rain:2
    int tiltIdx = rawData.indexOf("Tilt:");
    int tempIdx = rawData.indexOf("Temp:");
    int humIdx  = rawData.indexOf("Hum:");
    int soilIdx = rawData.indexOf("Soil:");
    int rainIdx = rawData.indexOf("Rain:");

    if (tiltIdx >= 0 && tempIdx >= 0 && humIdx >= 0 && soilIdx >= 0 && rainIdx >= 0) {
      String tilt = rawData.substring(tiltIdx + 5, tempIdx - 1);
      String temp = rawData.substring(tempIdx + 5, humIdx - 1);
      String hum  = rawData.substring(humIdx + 4, soilIdx - 1);
      String soil = rawData.substring(soilIdx + 5, rainIdx - 1);
      String rain = rawData.substring(rainIdx + 5);

      // Build JSON
      String json = "{";
      json += "\"Tilt\":\"" + tilt + "\",";
      json += "\"Temp\":\"" + temp + "\",";
      json += "\"Hum\":\"" + hum + "\",";
      json += "\"Soil\":\"" + soil + "\",";
      json += "\"Rain\":\"" + rain + "\"";
      json += "}";

      Serial.println("🔄 Sending JSON to Google Sheets: " + json);

      if (WiFi.status() == WL_CONNECTED) {
        HTTPClient http;
        http.begin(scriptURL);
        http.addHeader("Content-Type", "application/json");

        int httpResponseCode = http.POST(json);
        if (httpResponseCode > 0) {
          String response = http.getString();
          Serial.println("✅ Response: " + response);
        } else {
          Serial.println("❌ Error sending data. Code: " + String(httpResponseCode));
        }
        http.end();
      } else {
        Serial.println("⚠️ WiFi not connected.");
      }
    } else {
      Serial.println("⚠️ Data format invalid or incomplete.");
    }
  }

  delay(500); // Slight delay to avoid rapid looping
}
