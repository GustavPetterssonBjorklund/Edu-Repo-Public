#pragma once

#include <ArduinoJson.h>
#include <WiFiClientSecure.h>

class timeApi {
  private:
    bool silent;
    WiFiClientSecure client;
    const char* apiUrl;

    /**
     * @brief Helper function to fetch data from the API
     * @param url URL of the time API
     * @param client WiFiClientSecure& object
     * @return Payload from GET request
     */
    String getObject(const char* url, WiFiClientSecure& client) {
      if (WiFi.status() == WL_CONNECTED) {
        HTTPClient http;

        if (!silent) Serial.println("Connecting to API...");
        http.begin(client, url);
        int httpCode = http.GET();

        if (httpCode > 0) {
          if (!silent) Serial.printf("HTTP Response code: %d\n", httpCode);

          if (httpCode == HTTP_CODE_OK) {
            String payload = http.getString();
            if (!silent) {
              Serial.println("Received JSON payload:");
              Serial.println(payload);
            }
            http.end();
            return payload;
          } else {
            if (!silent) Serial.printf("Error: %d\n", httpCode);
          }
        } else {
          if (!silent) Serial.printf("Failed to fetch data. HTTP error: %s\n", http.errorToString(httpCode).c_str());
        }
        http.end();
      } else {
        if (!silent) Serial.println("WiFi not connected");
      }
      return "";
    }

    /**
     * @brief Helper function to parse JSON object
     * @param payload Fetched string from web API
     * @param doc JsonDocument to parse into
     * @return True if parsing succeeds, false otherwise
     */
    bool parseJson(const String& payload, JsonDocument& doc) {
      DeserializationError error = deserializeJson(doc, payload);
      if (error) {
        if (!silent) {
          Serial.print("JSON deserialization failed: ");
          Serial.println(error.c_str());
        }
        return false;
      }
      return true;
    }

  public:
    /**
    * @param apiUrl target url, default: https://timeapi.io/api/Time/current/zone?timeZone=Europe/Stockholm
    * @param silent if function should NOT print debug lines (should be silent), default: true
    * @note class additionally has the printDbg() function that will not be calles if silend == false
    * @note the target url includes time zone
    */
    timeApi(const char* apiUrl = "https://timeapi.io/api/Time/current/zone?timeZone=Europe/Stockholm",
            bool silent = true)
        : silent(silent), client(), apiUrl(apiUrl), date(""), time(""), timeZone(""), dayOfWeek(""), dstActive(false) {}

    // Variables parsed from JSON
    String date;
    String time;
    String timeZone;
    String dayOfWeek;
    bool dstActive;

    /**
     * @note Ensure WiFi is connected before initialization
     */
    void initialize() {
      client.setInsecure();
      fetchTimeFromAPI(); // Populate initial data
    }

    /**
     * @brief Fetch data from the API and parse it
     */
    void fetchTimeFromAPI() {
      String payload = getObject(apiUrl, client);
      if (payload.isEmpty()) {
        if (!silent) Serial.println("Failed to retrieve data from API.");
        return;
      }

      JsonDocument doc; 
      if (parseJson(payload, doc)) {
        // Extract available fields
        date = String(doc["date"] | "");
        time = String(doc["time"] | "");
        timeZone = String(doc["timeZone"] | "");
        dayOfWeek = String(doc["dayOfWeek"] | "");
        dstActive = doc["dstActive"] | false;

        if (!silent) Serial.println("Data successfully parsed!");
      }
    }

    /**
     * @brief Debug function to print all the stored data
     */
    void printDbg() {
      if (silent) return; // Suppress debug output if silent mode is enabled

      // Print extracted values
      Serial.printf("Date: %s\n", date.c_str());
      Serial.printf("Time: %s\n", time.c_str());
      Serial.printf("TimeZone: %s\n", timeZone.c_str());
      Serial.printf("Day of Week: %s\n", dayOfWeek.c_str());
      Serial.printf("DST Active: %s\n", dstActive ? "Yes" : "No");
    }
};
