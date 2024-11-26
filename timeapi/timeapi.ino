#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h> // Include the ArduinoJson library

const char* ssid = "iceWelder";
const char* password = "sam23sam";

// Use the API URL
const char* apiUrl = "https://timeapi.io/api/Time/current/zone?timeZone=Europe/Paris";

WiFiClientSecure client;

void setup() {
  Serial.begin(115200);
  delay(1000);

  client.setInsecure(); // For HTTPS

  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi...");

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  Serial.println("\nWiFi connected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  fetchTimeFromAPI();
}

void loop() {
  delay(60000); // Fetch every 60 seconds
  fetchTimeFromAPI();
}

void fetchTimeFromAPI() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    Serial.println("Connecting to API...");
    http.begin(client, apiUrl); // HTTPS URL with secure client
    int httpCode = http.GET();

    if (httpCode > 0) {
      Serial.printf("HTTP Response code: %d\n", httpCode);

      if (httpCode == HTTP_CODE_OK) {
        String payload = http.getString();
        Serial.println("Received JSON payload:");
        Serial.println(payload);

        // Parse JSON
        StaticJsonDocument<512> doc; // Adjust size based on expected payload
        DeserializationError error = deserializeJson(doc, payload);

        if (error) {
          Serial.print("JSON deserialization failed: ");
          Serial.println(error.c_str());
          return;
        }

        // Extract values
        const char* dateTime = doc["dateTime"];
        const char* timeZone = doc["timeZone"];
        const char* utcOffset = doc["utcOffset"];

        // Print extracted values
        Serial.println("Parsed JSON:");
        Serial.print("DateTime: ");
        Serial.println(dateTime);
        Serial.print("TimeZone: ");
        Serial.println(timeZone);
        Serial.print("UTC Offset: ");
        Serial.println(utcOffset);
      }
    } else {
      Serial.printf("Failed to fetch data. HTTP error: %s\n", http.errorToString(httpCode).c_str());
    }

    http.end();
  } else {
    Serial.println("WiFi not connected");
  }
}
