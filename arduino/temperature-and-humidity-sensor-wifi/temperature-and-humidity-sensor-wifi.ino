#include <ESP8266WiFi.h>
#include <DHT.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h>
#define LED_PIN LED_BUILTIN

const char* ssid = "xxxxxxxxxxxx";  // Wi-Fi Name
const char* password = "xxxxxxxxxxxx";  // Wi-Fi Password
const char* serverName = "https://xxxxxxxxxxx/update";  // Backend URL

#define DHT_SENSOR_PIN D7
#define DHT_SENSOR_TYPE DHT22
DHT dht(DHT_SENSOR_PIN, DHT_SENSOR_TYPE);

void setup() {
  Serial.begin(9600);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);  // LED mati (ESP8266 aktif low)
  WiFi.begin(ssid, password);

  unsigned long startAttemptTime = millis();
  const unsigned long timeout = 10000;  // Timeout 10 detik

  while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < timeout) {
    delay(500);
    Serial.print(".");
  }

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("\nFailed to connect to WiFi.");
    return;
  }
  Serial.println("\nConnected to WiFi");

  dht.begin();
}

void loop() {
  float humidity = dht.readHumidity();
  float temperature_C = dht.readTemperature();
  float temperature_F = dht.readTemperature(true);

  if (isnan(humidity) || isnan(temperature_C) || isnan(temperature_F)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  Serial.println("DHT Sensor Readings:");
  Serial.print("Humidity: ");
  Serial.println(humidity);
  Serial.print("Temperature (C): ");
  Serial.println(temperature_C);
  Serial.print("Temperature (F): ");
  Serial.println(temperature_F);

  String postData = "{\"humidity\":" + String(humidity, 2) +
                    ",\"temperature_C\":" + String(temperature_C, 2) +
                    ",\"temperature_F\":" + String(temperature_F, 2) + "}";

  WiFiClientSecure client;  // Gunakan WiFiClientSecure untuk HTTPS
  client.setInsecure();     // Nonaktifkan validasi SSL

  HTTPClient http;
  http.begin(client, serverName);  // Gunakan WiFiClientSecure
  http.addHeader("Content-Type", "application/json");

  int httpResponseCode = http.POST(postData);

  // Lampu berkedip saat mengirim data
  digitalWrite(LED_PIN, LOW);
  delay(100);
  digitalWrite(LED_PIN, HIGH);

  if (httpResponseCode > 0) {
    String response = http.getString();
    Serial.println("Response: " + response);
  } else {
    Serial.print("Error in sending POST request, Code: ");
    Serial.println(httpResponseCode);
    // Indikasi error dengan LED
    for (int i = 0; i < 3; i++) {
      digitalWrite(LED_PIN, LOW);
      delay(200);
      digitalWrite(LED_PIN, HIGH);
      delay(200);
    }
  }

  http.end();
  delay(3000);
}
