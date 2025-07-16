#ifndef HANDLERS_H
#define HANDLERS_H

#include <Arduino.h>
#include <ESP8266WebServer.h> // або #include <WebServer.h> для ESP32
#include <IRremoteESP8266.h>
#include <IRsend.h>

// Зовнішні змінні, які використовуються
extern ESP8266WebServer server; // або extern WebServer server;
extern IRsend irsend;
extern uint16_t rawData[];

// Функції обробки
void handleRoot();
void handleOn();
void handleOff();
void handleMuteOff();
void handleNotFound();

#endif
