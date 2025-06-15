/*
ONBOARD SENSORS: 
- DHT22 (D7)
- SOIL MOISTURE (A0)
- WATER PRESSURE (A2)
- RAIN SENSOR (A1)
- TILT SENSOR (D5)

MICROCONTROLLER: ARDUINO MEGA
*/

#include <DHT.h>
#include <SPI.h>
#include <SD.h>

// LoRa Serial: Serial1 (Pins 18 TX1, 19 RX1)

#define DHTPIN 7
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

#define SOIL_MOIST A0
#define RAIN A1
#define WTR_PRESSURE A2
#define TILT_PIN 5           // Tilt sensor on digital pin 5
const float OffSet = 0.072;
const int chipSelect = 4;

// Variables
int tilt = 0;
float hum = 0.0, temp = 0.0;
float moisture_percentage = 0.0;
int sensor_analog = 0;
int rain_percentage = 0;
int rain_analog = 0;

String outgoingData;
int stringLength;

void setup() {
  Serial.begin(115200);
  Serial1.begin(9600);
  delay(2000);

  pinMode(TILT_PIN, INPUT_PULLUP); // Tilt sensor with internal pull-up

  Serial.println("🔧 Initializing LoRa Configuration...");

  Serial1.print("AT\n");
  delay(100);
  Serial.print("Interface: "); Serial.println(Serial1.readString());

  Serial1.print("AT+OPMODE=1\n");
  delay(100);
  Serial.print("OPMODE: "); Serial.println(Serial1.readString());

  Serial1.print("AT+BAND=923000000\n");
  delay(100);
  Serial.print("BAND: "); Serial.println(Serial1.readString());

  Serial1.print("AT+ADDRESS=2\n"); // This is the sender node
  delay(100);
  Serial.print("ADDRESS: "); Serial.println(Serial1.readString());

  Serial.println("✅ LoRa Node Configured");
  Serial.println("-------------------------------------------");
  Serial.println("🔌 Sensor Pins:");
  Serial.println("🌡️  DHT22         : Pin " + String(DHTPIN));
  Serial.println("🌱 Soil Moisture : Pin " + String(SOIL_MOIST));
  Serial.println("☔ Rain Sensor    : Pin " + String(RAIN));
  Serial.println("💧 Water Pressure: Pin " + String(WTR_PRESSURE));
  Serial.println("🎯 Tilt Sensor    : Pin " + String(TILT_PIN));
  Serial.println("-------------------------------------------\n");

  dht.begin();
}

void loop() {
  // Reading from DHT22
  hum = dht.readHumidity();
  temp = dht.readTemperature();

  if (isnan(hum) || isnan(temp)) {
    Serial.println("⚠️  Failed to read from DHT sensor!");
    return;
  }

  // Soil Moisture
  sensor_analog = analogRead(SOIL_MOIST);  
  moisture_percentage = (100 - ((sensor_analog / 1023.00) * 100));

  // Rain Sensor
  rain_analog = analogRead(RAIN);
  rain_percentage = (100 - ((rain_analog / 1023.00) * 100));

  // Tilt Sensor (1 = tilted, 0 = stable)
  tilt = digitalRead(TILT_PIN);
  tilt = !tilt; // Invert: HIGH means not tilted → 0, LOW means tilted → 1

  // Prepare the outgoing message
  outgoingData = "Tilt: " + String(tilt) +
                 ", Temp: " + String(temp, 2) +
                 ", Hum: " + String(hum, 2) +
                 ", Soil_%moist: " + String(moisture_percentage, 2) +
                 ", Rain_%: " + String(rain_percentage);
  stringLength = outgoingData.length();

  // Print to serial
  Serial.println("📤 Sending to LoRa Receiver:");
  Serial.println("Message: " + outgoingData);
  Serial.println("Length : " + String(stringLength));
  Serial.println("-------------------------------------------");

  // Transmit to LoRa Receiver (Address 1)
  Serial1.print("AT+SEND=1," + String(stringLength) + "," + outgoingData + "\n");

  // Check LoRa response
  if (Serial1.available()) {
    Serial.println("📨 LoRa Response:");
    Serial.println(Serial1.readString());
  }

  // Serial passthrough
  if (Serial.available()) {
    Serial1.print(Serial.readString());
  }

  delay(5000); // Adjust delay as needed
}

