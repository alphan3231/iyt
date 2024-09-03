#include <Wire.h>
#include <Adafruit_ADS1X15.h>
#include <WiFi.h>
#include <HTTPClient.h>

Adafruit_ADS1115 ads;  /* Use this for the 16-bit version */

// Define constants for earthquake detection
const int THRESHOLD = 3000;  // Sensitivity threshold for detecting earthquake
const int RESET_COUNT_THRESHOLD = 6;  // Number of consecutive low values required to reset quake duration
unsigned long quakeDuration = 0;  // Duration of the detected earthquake
int resetCounter = 0;  // Counter for consecutive low readings
String area = "Location_1";  // Define the area where the earthquake is being detected

// WiFi credentials
const char* ssid = "SUPERONLINE_WiFi_4662";  // Your WiFi SSID
const char* password = "N3YUM4XAERCM";  // Your WiFi password

void setup(void) {
  // Initialize Serial communication at 115200 baud for ESP32
  Serial.begin(115200);
  // Wait for Serial to initialize
  while (!Serial) {
    delay(10);
  }

  // Connect to WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("Connected to WiFi!");

  // Initialize the ADS1115
  ads.setGain(GAIN_SIXTEEN);  // 16x gain  +/- 0.256V  1 bit = 0.0078125mV
  if (!ads.begin()) {
    Serial.println("Failed to initialize ADS.");
    while (1);
  }
}

void loop(void) {
  static int16_t previousResult = 0;  // Store the previous reading
  int16_t results;

  /* Be sure to update this value based on the IC and the gain settings! */
  float multiplier = 0.1875F;  /* ADS1115  @ +/- 6.144V gain (16-bit results) */

  // Read differential value from channels 0 and 1
  results = ads.readADC_Differential_0_1();

  // Calculate the difference between the current and previous results
  int16_t difference = abs(results - previousResult);

  // If the difference is greater than the threshold, it indicates a potential earthquake
  if (difference > THRESHOLD) {
    quakeDuration += 100;  // Increment quake duration by 100 milliseconds (based on delay)
    Serial.print("Earthquake detected! Duration: ");
    Serial.print(quakeDuration);
    Serial.println(" ms");
    resetCounter = 0;  // Reset the counter since a significant reading is detected
  } else {
    // If difference is less than or equal to the threshold, increment the reset counter
    resetCounter++;
    Serial.println(0);

    // Only reset quake duration if we have consecutive low readings
    if (resetCounter >= RESET_COUNT_THRESHOLD) {
      // Send data to server since earthquake is finished
      sendEarthquakeData(area, quakeDuration);

      quakeDuration = 0;  // Reset quake duration after sending data
      resetCounter = 0;   // Reset the reset counter
    }
  }

  // Print the differential voltage in mV
  Serial.println(results * multiplier);

  // Update previousResult with the current reading
  previousResult = results;

  delay(100);  // Slight delay to avoid flooding the serial monitor
}

// Function to send earthquake data to the server
void sendEarthquakeData(String area, unsigned long duration) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin("http://shakesphere.com.tr:5000/depremyolla");

    // Specify content-type header
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");

    // Prepare data payload
    String postData = "area=" + area + "&duration=" + String(duration);

    // Send POST request
    int httpResponseCode = http.POST(postData);

    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.print("Server Response: ");
      Serial.println(response);
    } else {
      Serial.print("Error on sending POST: ");
      Serial.println(httpResponseCode);
    }

    // End the connection
    http.end();
  } else {
    Serial.println("Error in WiFi connection");
  }
}
