// #include "helpers/utils.h"

// void handleIRInput()
// {
//   if (irrecv.decode(&results))
//   {
//     Serial.println("⌛ Прийнято дані");

//     // Виводимо загальну інформацію про сигнал
//     Serial.println(resultToHumanReadableBasic(&results));
//     Serial.println(resultToSourceCode(&results));
//     Serial.print("IR код (HEX): ");
//     Serial.println(resultToHexidecimal(&results));

//     // Обробка лише "невідомих" протоколів з 50 бітами
//     if (results.decode_type == UNKNOWN && results.bits == 50)
//     {
//       Serial.println("✅ Прийнято ІЧ-сигнал, виконуємо дію");
//     }
//     else
//     {
//       Serial.print("⚠️ Пропущено інший протокол: ");
//       Serial.println(resultToHumanReadableBasic(&results));
//     }

//     // Готуватися до наступного сигналу
//     irrecv.resume();
//   }
// }

// void handlerWiFiStatus()
// {
//   if (WiFi.status() != WL_CONNECTED)
//   {
//     if (wifiWasConnected)
//     {
//       Serial.println("⚠️ WiFi з'єднання втрачено");
//       digitalWrite(LED_WIFI_PIN, HIGH);
//       wifiWasConnected = false;
//     }
//   }
//   else
//   {
//     if (!wifiWasConnected)
//     {
//       Serial.println("✅ WiFi знову підключено");
//       digitalWrite(LED_WIFI_PIN, LOW);
//       wifiWasConnected = true;
//     }
//   }
// }
