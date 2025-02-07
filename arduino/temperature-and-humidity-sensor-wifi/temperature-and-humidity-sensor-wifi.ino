#include <ESP8266WiFi.h>
#include <DHT.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#define LED_PIN LED_BUILTIN

const char* ssid = "Wifi"; // Wifi Name
const char* password = "WifiPass"; // Wifi Pass
const char* serverName = "http://192.168.1.2:8080/update";  // IP Backend

#define DHT_SENSOR_PIN D7
#define DHT_SENSOR_TYPE DHT22
DHT dht(DHT_SENSOR_PIN, DHT_SENSOR_TYPE);

void setup() {
  Serial.begin(9600);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH); // LED mati (ESP8266 aktif low)
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
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

  String postData = "{\"humidity\":" + String(humidity, 2) + 
                    ",\"temperature_C\":" + String(temperature_C, 2) + 
                    ",\"temperature_F\":" + String(temperature_F, 2) + "}";

  WiFiClient client;  // Buat objek WiFiClient
  HTTPClient http;
  
  http.begin(client, serverName);  // Gunakan WiFiClient sebagai parameter pertama
  http.addHeader("Content-Type", "application/json");

  int httpResponseCode = http.POST(postData);

  // Lampu Berkedip Setiap Mengirim Data
  digitalWrite(LED_PIN, LOW);
  delay(100);
  digitalWrite(LED_PIN, HIGH);

  if (httpResponseCode > 0) {
    String response = http.getString();
    Serial.println("Response: " + response);
  } else {
    Serial.print("Error in sending POST request, Code: ");
    Serial.println(httpResponseCode);
  }

  http.end();
  delay(3000);
}
