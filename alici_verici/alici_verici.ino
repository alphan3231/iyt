#include <WiFi.h>
#include <WiFiClient.h>

// Wi-Fi Credentials
const char* ssid = "SUPERONLINE_WiFi_4662";
const char* password = "N3YUM4XAERCM";

// Server IP and Port
const char* serverHost = "192.168.1.34";
const int serverPort = 5000;

WiFiServer server(80);

void setup() {
  Serial.begin(115200);

  // Initialize Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  // Wi-Fi bağlantısı başarılı olduğunda IP adresini yazdırın
  Serial.println("Connected to WiFi");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Start the server
  server.begin();
}

void loop() {
  WiFiClient client = server.available();
  if (client) {
    String request = client.readStringUntil('\r');
    client.flush();

    if (request.length() > 0) {
      // Forward the data to Flask server
      WiFiClient serverClient;
      if (serverClient.connect(serverHost, serverPort)) {
        serverClient.println("POST /verilerial HTTP/1.1");
        serverClient.println("Host: " + String(serverHost));
        serverClient.println("Content-Type: application/x-www-form-urlencoded");
        serverClient.print("Content-Length: ");
        serverClient.println(request.length());
        serverClient.println();
        serverClient.println(request);
        serverClient.stop();
      }
    }
    client.stop();
  }
}
