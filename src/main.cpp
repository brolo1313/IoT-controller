#include <Arduino.h>
#include <ESP8266WiFi.h>      // Це офіційна бібліотека для роботи з Wi-Fi на чіпах ESP8266.
#include <Wire.h>             // Це бібліотека для роботи з шиною I2C (Дозволяє "розмовляти" з пристроями по I2C (OLED, акселерометри, RTC))
#include <Adafruit_GFX.h>     // Це графічна бібліотека від Adafruit, яка дає універсальні методи для малювання на екранах
#include <Adafruit_SSD1306.h> // Це драйвер для OLED-дисплеїв з контролером SSD1306 (найпоширеніші 128x64 OLED дисплеї).
#include <ESP8266WebServer.h>
#include <IRrecv.h>
#include <IRremoteESP8266.h>
#include <IRutils.h>
#include <IRsend.h>
#include "secrets.h"
//  Рекомендовано для IRrecv: D1, D2, D5, D6, D7

// IR код TV
// 0xD - mute
// IR код air conditioner
uint16_t rawData[99] = {3158, 1542, 566, 1042, 570, 1014, 566, 278, 570, 222,
                        622, 222, 596, 1012, 596, 224, 594, 250, 594, 1014, 570, 1014, 598, 218, 596, 1042, 570, 222, 568,
                        302, 572, 1014, 566, 1044, 542, 276, 568, 1040, 572, 1014, 566, 278, 570, 246, 598, 1014, 568, 224,
                        620, 224, 594, 1016, 598, 218, 570, 302, 572, 218, 590, 280, 568, 222, 592, 278, 570, 246, 598, 220,
                        598, 246, 598, 220, 594, 276, 542, 276, 542, 302, 572, 218, 592, 280, 568, 224, 594, 274, 570, 1014,
                        594, 222, 596, 224, 620, 1014, 572, 274, 570, 222, 568};

const uint8_t RECV_PIN = D5;

const uint16_t kIrLedPin = D2; // Пін, до якого підключений ІЧ-світлодіод
IRsend irsend(kIrLedPin);
IRrecv irrecv(RECV_PIN);
decode_results results;

ESP8266WebServer server(SECRET_PORT);

#define LED_WIFI_PIN LED_BUILTIN

void handleRoot()
{
  server.send(200, "text/plain", "ESP працює!");
}

void handleOn()
{
  Serial.println("🟢 Команда ON від Telegram");
  irsend.sendRaw(rawData, 99, 38); // 99 - довжина масиву, 38 - частота кГц
  // irsend.sendNEC(0xF7C03F, 32);
  Serial.println("📡 ІЧ-сигнал надіслано!");

  server.send(200, "application/json", "{\"status\":\"ok\",\"action\":\"on\"}");
}

void handleOff()
{
  Serial.println("🟢 Команда Off від Telegram");
  server.send(200, "application/json", "{\"status\":\"ok\",\"action\":\"off\"}");
}

void handleMuteOff()
{
  Serial.println("🟢 Команда Mute від Telegram");
  irsend.sendRC6(0xD, 20);
  server.send(200, "application/json", "{\"status\":\"ok\",\"action\":\"off\"}");
}

void handleNotFound()
{
  server.send(404, "application/json", "{\"status\":\"ok\",\"message\":\"not found\"}");
}

void handleIRInput();
void handlerWiFiStatus();

void setup()
{
  Serial.begin(115200);
  delay(1000);
  Serial.println("ESP8266 працює! ✅");

  pinMode(LED_WIFI_PIN, OUTPUT);
  digitalWrite(LED_WIFI_PIN, HIGH);

  irsend.begin();      // Ініціалізація ІЧ-передавача
  irrecv.enableIRIn(); // ← ПРАВИЛЬНИЙ запуск
  Serial.println("IR Receiver test start...");

  if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS))
  {
    Serial.println("⚠️ Конфігурація IP не вдалася");
  }

  WiFi.begin(SECRET_SSID, SECRET_PASSWORD);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print("...");
  }

  Serial.println("\nWiFi OK ✅");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.on("/air_on", handleOn);
  server.on("/air_off", handleOff);
  server.on("/mute_off", handleMuteOff);
  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP сервер запущено");
}

bool wifiWasConnected = false;

void loop()
{
  server.handleClient();
  handlerWiFiStatus();
}

void handleIRInput()
{
  if (irrecv.decode(&results))
  {
    Serial.println("⌛ Прийнято дані");

    // Виводимо загальну інформацію про сигнал
    Serial.println(resultToHumanReadableBasic(&results));
    Serial.println(resultToSourceCode(&results));
    Serial.print("IR код (HEX): ");
    Serial.println(resultToHexidecimal(&results));

    // Обробка лише "невідомих" протоколів з 50 бітами
    if (results.decode_type == UNKNOWN && results.bits == 50)
    {
      Serial.println("✅ Прийнято ІЧ-сигнал, виконуємо дію");

    }
    else
    {
      Serial.print("⚠️ Пропущено інший протокол: ");
      Serial.println(resultToHumanReadableBasic(&results));
    }

    // Готуватися до наступного сигналу
    irrecv.resume();
  }
}

void handlerWiFiStatus()
{
  if (WiFi.status() != WL_CONNECTED)
  {
    if (wifiWasConnected)
    {
      Serial.println("⚠️ WiFi з'єднання втрачено");
      digitalWrite(LED_WIFI_PIN, HIGH);
      wifiWasConnected = false;
    }
  }
  else
  {
    if (!wifiWasConnected)
    {
      Serial.println("✅ WiFi знову підключено");
      digitalWrite(LED_WIFI_PIN, LOW);
      wifiWasConnected = true;
    }
  }
}
