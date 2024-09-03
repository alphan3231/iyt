#include <Wire.h>
#include "MAX30105.h"
#include "heartRate.h"
#include <TinyGPSPlus.h>
#include <HardwareSerial.h>
#include <WiFi.h>
#include <HTTPClient.h>

MAX30105 particleSensor;
TinyGPSPlus gps;
HardwareSerial ss(2); // UART2 kullanarak RX2 ve TX2 pinleri için donanım serisi

const char* ssid = "SUPERONLINE_WiFi_4662";
const char* password = "N3YUM4XAERCM";
const char* serverName = "http://104.247.173.154:5000/verilerial";

const byte RATE_SIZE = 4; // Heart rate averaging size
byte rates[RATE_SIZE];    // Array of heart rates
byte rateSpot = 0;
long lastBeat = 0;        // Time at which the last beat occurred
long lastGPSTime = 0;     // Last time GPS data was received
const long GPS_TIMEOUT = 5000; // 5 seconds timeout for GPS data
long lastPostTime = 0;    // Last time data was posted
const long POST_INTERVAL = 10000; // 10 minutes in milliseconds

float beatsPerMinute;
int beatAvg;

String personID = "1234"; // Your person ID
String areaID = "5678";   // Your area ID

void setup() {
  Serial.begin(115200);
  Serial.println("Initializing...");

  // Initialize MAX30102 sensor
  if (!particleSensor.begin(Wire, I2C_SPEED_FAST)) {
    Serial.println("MAX30102 not found. Please check wiring/power.");
    while (1);
  }

  particleSensor.setup();                    // Configure sensor with default settings
  particleSensor.setPulseAmplitudeRed(0x0A); // Turn Red LED to low to indicate sensor is running
  particleSensor.setPulseAmplitudeGreen(0);  // Turn off Green LED

  // Initialize GPS module
  ss.begin(9600, SERIAL_8N1, 4, 3); // GPS modülü için 9600 baud hızında seri portu başlat

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi.");
}

void loop() {
  // Read the IR value from the sensor
  long irValue = particleSensor.getIR();

  // Heart rate calculation
  if (checkForBeat(irValue) == true) {
    long delta = millis() - lastBeat;
    lastBeat = millis();

    beatsPerMinute = 60 / (delta / 1000.0);

    if (beatsPerMinute < 255 && beatsPerMinute > 20) {
      rates[rateSpot++] = (byte)beatsPerMinute; // Store the latest heart rate in the array
      rateSpot %= RATE_SIZE;                    // Wrap variable

      // Take average of readings
      beatAvg = 0;
      for (byte x = 0; x < RATE_SIZE; x++)
        beatAvg += rates[x];
      beatAvg /= RATE_SIZE;
    }
  }

  // GPS data processing
  bool gpsDataReceived = false;
  String latitude = "Satellite Error";
  String longitude = "Satellite Error";
  String altitude = "Satellite Error";

  while (ss.available() > 0) {
    gps.encode(ss.read()); // GPS verilerini al ve işle
    gpsDataReceived = true;
    lastGPSTime = millis(); // Update last GPS data received time
  }

  // Update GPS data if available
  if (gpsDataReceived) {
    if (gps.location.isValid()) {
      latitude = String(gps.location.lat(), 6);
      longitude = String(gps.location.lng(), 6);
    }
    if (gps.altitude.isValid()) {
      altitude = String(gps.altitude.meters());
    }
  }

  // Check if it's time to send a POST request
  if (millis() - lastPostTime > POST_INTERVAL) {
    lastPostTime = millis(); // Update last post time
    sendPostRequest(personID, areaID, beatAvg, latitude, longitude, altitude);
    Serial.println("Sending POST request...");
  }

  // Display heart rate and GPS values
  // Serial.print("IR: ");
  // Serial.print(irValue);
  // Serial.print(" BPM: ");
  // Serial.print(beatsPerMinute);
  // Serial.print(" Avg BPM: ");
  // Serial.print(beatAvg);
  // Serial.print(" Latitude: ");
  // Serial.print(latitude);
  // Serial.print(" Longitude: ");
  // Serial.print(longitude);
  // Serial.print(" Altitude: ");
  // Serial.println(altitude);


}

void sendPostRequest(String personID, String areaID, float bpm, String latitude, String longitude, String altitude) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    http.begin(serverName);
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");

    String httpRequestData = "personID=" + personID + "&areaID=" + areaID +
                             "&bpm=" + String(bpm) +
                             "&latitude=" + latitude +
                             "&longitude=" + longitude +
                             "&altitude=" + altitude;

    int httpResponseCode = http.POST(httpRequestData);

    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
      Serial.println(response);
    } else {
      Serial.print("Error on sending POST: ");
      Serial.println(httpResponseCode);
    }

    http.end(); // Free resources
  } else {
    Serial.println("WiFi Disconnected");
  }
}
