// #include "helpers/handlers-route.h"

// void handleRoot()
// {
//   server.send(200, "text/plain", "ESP працює!");
// }

// void handleOn()
// {
//   Serial.println("🟢 Команда ON від Telegram");
//   irsend.sendRaw(rawData, 99, 38); // 99 - довжина масиву, 38 - частота кГц
//   Serial.println("📡 ІЧ-сигнал надіслано!");

//   server.send(200, "application/json", "{\"status\":\"ok\",\"action\":\"on\"}");
// }

// void handleOff()
// {
//   Serial.println("🟢 Команда Off від Telegram");
//   server.send(200, "application/json", "{\"status\":\"ok\",\"action\":\"off\"}");
// }

// void handleMuteOff()
// {
//   Serial.println("🟢 Команда Mute від Telegram");
//   irsend.sendRC6(0xD, 20);
//   server.send(200, "application/json", "{\"status\":\"ok\",\"action\":\"off\"}");
// }

// void handleNotFound()
// {
//   server.send(404, "application/json", "{\"status\":\"ok\",\"message\":\"not found\"}");
// }
