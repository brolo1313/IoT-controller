#ifndef UTILS_H
#define UTILS_H

#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRutils.h>
#include <ESP8266WiFi.h> // або <WiFi.h> для ESP32

// Зовнішні змінні, які використовуються у функціях
extern IRrecv irrecv;
extern decode_results results;
extern bool wifiWasConnected;
extern const int LED_WIFI_PIN;

// Прототипи функцій
void handleIRInput();
void handlerWiFiStatus();

#endif
