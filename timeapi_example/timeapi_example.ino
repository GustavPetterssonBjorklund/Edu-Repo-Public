#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h>

#include "timeApi.h"

const char* ssid = "ssid";
const char* password = "passphrase";

timeApi tA;

void setup() {
  Serial.begin(115200);
  delay(1000);

  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi...");

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  Serial.println("\nWiFi connected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  tA.initialize();
}

void loop() {
  delay(5000);  // Fetch every 5 seconds
  tA.fetchTimeFromAPI();

  // Print the stored data using Serial.printf

  /// @note using c_str() to convert to constant char* from String due to limitations in printf
  Serial.printf("Time: %s\n", tA.time.c_str());
  Serial.printf("Date: %s\n", tA.date.c_str());
}
