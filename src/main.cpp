#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ESP8266WebServer.h>
#include <SoftwareSerial.h>

// Піни
#define IR_TX_PIN D2   // TX ESP → RX модуля
#define LED_WIFI_PIN D6

SoftwareSerial irSerial(IR_TX_PIN, -1); // Тільки TX (TX ESP → RX IR-модуля)

ESP8266WebServer server(80);

IPAddress local_IP(192, 168, 31, 206);
IPAddress gateway(192, 168, 31, 1);
IPAddress subnet(255, 255, 255, 0);
IPAddress primaryDNS(8, 8, 8, 8);
IPAddress secondaryDNS(8, 8, 4, 4);

// WiFi
const char *ssid = "Xiaomi_5DBA";
const char *password = "0634615110";
bool wifiWasConnected = true;

// Буфери для прийнятих команд
uint8_t receivedCommand[32];  // максимальна довжина команди
size_t receivedLength = 0;

void handleRoot() {
  server.send(200, "text/plain", "ESP працює!");
}

void sendCommandToIRModule(uint8_t command[], size_t length) {
  irSerial.write(command, length);
  irSerial.flush();
  Serial.print("📤 Надіслано IR команду: ");
  for (int i = 0; i < length; i++) {
    Serial.printf("0x%02X ", command[i]);
  }
  Serial.println();
}

void handleOn() {
  Serial.println("🟢 Команда ON від Telegram");
  uint8_t air_on[] = {0xA1, 0xF1, 0x01, 0x00, 0x00, 0xA2}; // приклад
  sendCommandToIRModule(air_on, sizeof(air_on));
  server.send(200, "application/json", "{\"status\":\"ok\",\"action\":\"on\"}");
}

void handleOff() {
  Serial.println("🟢 Команда OFF від Telegram");
  uint8_t air_off[] = {0xA1, 0xF1, 0x02, 0x00, 0x00, 0xA3}; // приклад
  sendCommandToIRModule(air_off, sizeof(air_off));
  server.send(200, "application/json", "{\"status\":\"ok\",\"action\":\"off\"}");
}

void handleMuteOff() {
  Serial.println("🟢 Команда MUTE");
  uint8_t mute[] = {0xA1, 0xF1, 0x0D, 0x00, 0x00, 0xAE};
  sendCommandToIRModule(mute, sizeof(mute));
  server.send(200, "application/json", "{\"status\":\"ok\",\"action\":\"mute\"}");
}

void handleNotFound() {
  server.send(404, "application/json", "{\"status\":\"error\",\"message\":\"not found\"}");
}

void handlerWiFiStatus() {
  if (WiFi.status() != WL_CONNECTED) {
    if (wifiWasConnected) {
      Serial.println("⚠️ WiFi з'єднання втрачено");
      digitalWrite(LED_WIFI_PIN, LOW);
      wifiWasConnected = false;
    }
  } else {
    if (!wifiWasConnected) {
      Serial.println("✅ WiFi знову підключено");
      digitalWrite(LED_WIFI_PIN, HIGH);
      wifiWasConnected = true;
    }
  }
}

void receiveIRCommandFromRemote() {
  bool gotSomething = false;

  while (irSerial.available()) {
    uint8_t b = irSerial.read();
    Serial.printf("📥 UART байт: 0x%02X\n", b);

    if (receivedLength < sizeof(receivedCommand)) {
      receivedCommand[receivedLength++] = b;
      gotSomething = true;
    }
    delay(2); // Буферизація
  }

  if (gotSomething && receivedLength >= 3) { // менше 3 — скоріше за все шум
    Serial.println("🔸 Отримано повну команду:");

    Serial.print("uint8_t command[] = { ");
    for (size_t i = 0; i < receivedLength; i++) {
      Serial.printf("0x%02X", receivedCommand[i]);
      if (i < receivedLength - 1) Serial.print(", ");
    }
    Serial.println(" };");

    Serial.printf("📏 Довжина: %d байт\n", receivedLength);
  } else if (gotSomething) {
    Serial.println("⚠️ Отримано шум або неповну команду.");
  }

  receivedLength = 0; // скидаємо буфер
}


void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("ESP8266 запущено ✅");

  pinMode(LED_WIFI_PIN, OUTPUT);
  digitalWrite(LED_WIFI_PIN, LOW);

  // UART до IR модуля
  irSerial.begin(9600);
  delay(1000);
  Serial.println("ІЧ модуль ініціалізовано");

  // WiFi
  if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)) {
    Serial.println("⚠️ Налаштування IP невдале");
  }

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\n✅ WiFi підключено");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
  digitalWrite(LED_WIFI_PIN, HIGH);

  // HTTP-маршрути
  server.on("/", handleRoot);
  server.on("/air_on", handleOn);
  server.on("/air_off", handleOff);
  server.on("/mute_off", handleMuteOff);
  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("🌐 HTTP сервер запущено");
}

void loop() {
  server.handleClient();
  handlerWiFiStatus();
  receiveIRCommandFromRemote(); // 👈 нова функція прослуховує прийом команд
}
