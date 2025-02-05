/*
 * Kode ESP8266 untuk membaca suhu & kelembaban dari DHT22,
 * lalu menampilkannya di web server melalui WiFi.
 */

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <DHT.h>

// Pilih mode WiFi (STA atau AP)
#define USE_STA_MODE true  // true = mode Station (terhubung ke router), false = mode Access Point

// Konfigurasi WiFi (ubah sesuai kebutuhan)
const char* ssid     = "Wifi";      // Nama WiFi (jika mode STA)
const char* password = "WifiPass";  // Password WiFi

// Jika mode AP (Access Point)
const char* ap_ssid = "ESP8266_AP";
const char* ap_password = "12345678";  // Minimal 8 karakter

// Konfigurasi DHT22
#define DHT_SENSOR_PIN D7  // Pin ESP8266 ke DHT22
#define DHT_SENSOR_TYPE DHT22
DHT dht(DHT_SENSOR_PIN, DHT_SENSOR_TYPE);

// Konfigurasi Web Server di port 80
ESP8266WebServer server(80);

void setup() {
  Serial.begin(9600);
  dht.begin(); // Inisialisasi DHT sensor

  if (USE_STA_MODE) {
    // Mode Station (Terhubung ke Router WiFi)
    WiFi.begin(ssid, password);
    Serial.print("Connecting to WiFi...");
    while (WiFi.status() != WL_CONNECTED) {
      delay(1000);
      Serial.print(".");
    }
    Serial.println("\nConnected to WiFi!");
    Serial.print("ESP8266 IP Address: ");
    Serial.println(WiFi.localIP());
  } else {
    // Mode Access Point (ESP8266 Menjadi Hotspot)
    WiFi.softAP(ap_ssid, ap_password);
    Serial.println("Access Point Started!");
    Serial.print("ESP8266 IP Address: ");
    Serial.println(WiFi.softAPIP());
  }

  // Rute Web Server untuk halaman utama
  server.on("/", []() {
    float humi = dht.readHumidity();
    float tempC = dht.readTemperature();
    float tempF = dht.readTemperature(true);

    String html = "<html><head><title>ESP8266 DHT22</title></head><body>";
    html += "<h1>Data Sensor DHT22</h1>";
    if (isnan(humi) || isnan(tempC) || isnan(tempF)) {
      html += "<p><strong>Error:</strong> Gagal membaca sensor!</p>";
    } else {
      html += "<p><strong>Kelembaban:</strong> " + String(humi) + "%</p>";
      html += "<p><strong>Suhu:</strong> " + String(tempC, 2) + "&deg;C  ~  " + String(tempF, 2) + "&deg;F</p>";
    }
    html += "</body></html>";

    server.send(200, "text/html", html);
  });

  // Mulai Web Server
  server.begin();
  Serial.println("Web Server Started!");
}

void loop() {
  server.handleClient(); // Menangani permintaan HTTP
}
