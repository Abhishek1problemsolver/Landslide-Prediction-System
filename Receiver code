RECEIVER CODE :
#include <SPI.h>

void setup() {
  Serial.begin(115200);     // USB Monitor
  Serial1.begin(9600);      // LoRa Module (TX = 18, RX = 19)
  Serial2.begin(9600);      // To ESP32 (TX2 = 16, RX2 = 17)

  delay(2000);
  Serial.println("🔧 Configuring LoRa Receiver...");

  Serial1.print("AT\n");
  delay(100);
  Serial.println("Interface: " + Serial1.readString());

  Serial1.print("AT+OPMODE=1\n");
  delay(100);
  Serial.println("OPMODE: " + Serial1.readString());

  Serial1.print("AT+BAND=923000000\n");
  delay(100);
  Serial.println("BAND: " + Serial1.readString());

  Serial1.print("AT+ADDRESS=1\n");
  delay(100);
  Serial.println("ADDRESS: " + Serial1.readString());

  Serial.println("✅ LoRa Receiver Ready. Waiting for data...\n");
}

void loop() {
  if (Serial1.available()) {
    String incoming = Serial1.readStringUntil('\n');
    incoming.trim();
    Serial.println("📡 Raw LoRa Data: " + incoming);

    int payloadStart = incoming.indexOf("Tilt:");
    if (payloadStart >= 0) {
      String payload = incoming.substring(payloadStart);
      int endIdx = payload.indexOf(",-", payloadStart);
      if (endIdx >= 0) {
        payload = payload.substring(0, endIdx);
      }

      int tiltIdx = payload.indexOf("Tilt:");
      int tempIdx = payload.indexOf("Temp:");
      int humIdx  = payload.indexOf("Hum:");
      int soilIdx = payload.indexOf("Soil_%moist:");
      int rainIdx = payload.indexOf("Rain_%:");

      if (tiltIdx >= 0 && tempIdx >= 0 && humIdx >= 0 && soilIdx >= 0 && rainIdx >= 0) {
        String tilt = payload.substring(tiltIdx + 5, tempIdx - 1);
        String temp = payload.substring(tempIdx + 5, humIdx - 1);
        String hum  = payload.substring(humIdx + 4, soilIdx - 1);
        String soil = payload.substring(soilIdx + 13, rainIdx - 1);

        String rain = payload.substring(rainIdx + 8);
        int rainEnd = rain.indexOf(',');
        if (rainEnd != -1) rain = rain.substring(0, rainEnd);
        rain.trim();

        String cleaned = "Tilt:" + tilt + ",Temp:" + temp + ",Hum:" + hum + ",Soil:" + soil + ",Rain:" + rain;
        Serial.println("✅ Data Received: " + cleaned);

        Serial2.println(cleaned);  // Send to ESP32
      } else {
        Serial.println("⚠️ Parsing error: Some values missing.");
      }
    }
  }

  if (Serial.available()) {
    Serial1.print(Serial.readString());
  }
}

