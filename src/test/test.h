#include <Arduino.h>
#define IR_LED_PIN D2

void setup()
{
    pinMode(IR_LED_PIN, OUTPUT);
}

void loop()
{
    // Генерація сигналу 38 кГц приблизно на 1 секунду
    for (int i = 0; i < 38000; i++)
    {
        digitalWrite(IR_LED_PIN, HIGH);
        delayMicroseconds(13); // половина періоду 38кГц
        digitalWrite(IR_LED_PIN, LOW);
        delayMicroseconds(13);
    }
    delay(3000); // пауза 3 сек
}



//FOR regular ir transmitter without controller ( use with IRremoteESP8266)
// #include <IRremoteESP8266.h>
// #include <IRsend.h>
// #include <Arduino.h>

// const uint16_t kIrLed = 4; // GPIO4 = D2
// IRsend irsend(kIrLed);

// void setup() {
//   Serial.begin(115200);
//   irsend.begin();
//   delay(2000);
//   Serial.println("🔁 Запуск циклічної передачі IR-сигналу кожні 2 секунди");
// }


//FOR IR MODULE CONTROLLER (SoftwareSerial)
// void loop() {
//   Serial.println("📡 Надсилаю IR сигнал...");
//   irsend.sendNEC(0x20DF10EF, 32); // NEC 32-бітовий код
//   delay(2000); // Затримка 2 секунди
// }


// #include <Arduino.h>
// #include <SoftwareSerial.h>

// // Налаштування UART-зв'язку
// #define IR_TX_PIN D2     // TX на ESP8266 → RXD на модулі
// #define IR_RX_PIN D1     // Просто заглушка для SoftwareSerial (не використовується)

// SoftwareSerial irSerial(IR_RX_PIN, IR_TX_PIN); // RX, TX — RX можна довільний

// // Наприклад, команда "mute" для телевізора
// uint8_t tv_mute_code[] = {
//   0xA1, 0xF1, 0x00, 0x00, 0x00, 0x0D, 0xA0
// };

// void setup() {
//   Serial.begin(115200);
//   irSerial.begin(115200); // 👈 Зазвичай модуль YS-IRTM працює на 9600 (або 115200 — див. наклейку)

//   Serial.println("🔄 Очікування запуску...");
//   delay(2000); // Затримка перед стартом

//   Serial.println("📤 Відправка IR коду...");
//   irSerial.write(tv_mute_code, sizeof(tv_mute_code));
//   Serial.println("✅ Код надіслано!");

//   delay(2000); // Почекати 2 секунди
// }

// void loop() {
//   // Кожні 3 секунди надсилати сигнал
//   irSerial.write(tv_mute_code, sizeof(tv_mute_code));
//   Serial.println("📤 Надіслано повторно");
//   delay(3000);
// }
