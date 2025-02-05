#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <DHT.h>

// Pilih mode WiFi (STA atau AP)
#define USE_STA_MODE true  // true = mode Station (terhubung ke router), false = mode Access Point

// Konfigurasi WiFi
const char* ssid     = "Wifi";      // Nama WiFi (mode STA)
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

// LED bawaan ESP8266 (D4 / GPIO2)
#define LED_PIN LED_BUILTIN

// Variabel untuk menyimpan data sensor
float temperature_C, temperature_F, humidity;
unsigned long previousMillis = 0;
const long interval = 2000; // Interval pembacaan sensor (2 detik)

void setup() {
  Serial.begin(9600);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH); // LED mati (ESP8266 aktif low)
  
  dht.begin(); // Inisialisasi DHT sensor

  if (USE_STA_MODE) {
    // Mode Station (Terhubung ke Router WiFi)
    WiFi.begin(ssid, password);
    Serial.print("Connecting to WiFi...");
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
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
    String html = "<html><head><meta charset='UTF-8'><title>ESP8266 DHT22</title>";
    html += "<script>";
    html += "function updateData() {";
    html += " fetch('/data')";  // Ambil data JSON dari ESP8266
    html += " .then(response => response.json())";
    html += " .then(data => {";
    html += "   document.getElementById('humidity').innerText = data.humidity.toFixed(2) + '%';";
    html += "   document.getElementById('temperature').innerText = data.temperature_C.toFixed(2) + '°C ~ ' + data.temperature_F.toFixed(2) + '°F';";
    html += " });";
    html += "}";
    html += "setInterval(updateData, 2000);";  // Perbarui data setiap 2 detik
    html += "</script></head><body onload='updateData()'>";
    html += "<h1>Data Sensor DHT22</h1>";
    html += "<p><strong>Kelembaban:</strong> <span id='humidity'>Loading...</span></p>";
    html += "<p><strong>Suhu:</strong> <span id='temperature'>Loading...</span></p>";
    html += "</body></html>";
    server.send(200, "text/html", html);
  });

  // Rute API untuk memberikan data JSON
  server.on("/data", []() {
    String json = "{";
    json += "\"humidity\":" + String(humidity, 2) + ",";
    json += "\"temperature_C\":" + String(temperature_C, 2) + ",";
    json += "\"temperature_F\":" + String(temperature_F, 2);
    json += "}";
    server.send(200, "application/json", json);
  });

  // Mulai Web Server
  server.begin();
  Serial.println("Web Server Started!");
}

void loop() {
  server.handleClient(); // Menangani permintaan HTTP

  unsigned long currentMillis = millis();
  
  // Baca data sensor setiap 2 detik
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    // LED berkedip setiap pembacaan data
    digitalWrite(LED_PIN, LOW);
    delay(100);
    digitalWrite(LED_PIN, HIGH);

    // Baca sensor
    humidity = dht.readHumidity();
    temperature_C = dht.readTemperature();
    temperature_F = dht.readTemperature(true);

    // Debug di Serial Monitor
    if (isnan(humidity) || isnan(temperature_C) || isnan(temperature_F)) {
      Serial.println("Gagal membaca sensor DHT22!");
    } else {
      Serial.print("Kelembaban: ");
      Serial.print(humidity, 2);
      Serial.print("%  |  Suhu: ");
      Serial.print(temperature_C, 2);
      Serial.print("°C  ~  ");
      Serial.print(temperature_F, 2);
      Serial.println("°F");
    }
  }
}
