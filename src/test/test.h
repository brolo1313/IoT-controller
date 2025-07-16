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

// #include <SoftwareSerial.h>

// #define IR_RX_PIN D7 // RX IR-модуля ← TX ESP8266
// #define IR_TX_PIN D8 // TX IR-модуля → RX ESP8266

// SoftwareSerial irSerial(IR_RX_PIN, IR_TX_PIN); // RX, TX

// uint8_t testCode[] = {
//   0xA1, 0xF1, 0x00, 0x00, 0x00, 0x0D, 0xA0 // Mute для TV
// };

// void setup() {
//   Serial.begin(115200);
//   irSerial.begin(115200); // Швидкість для YS-IRTM
//   delay(1000);

//   Serial.println("▶ Відправляю IR-код");
//   irSerial.write(testCode, sizeof(testCode));
// }

// void loop() {
//   // Повтор кожні 5 сек
//   delay(5000);
//   Serial.println("▶ Повтор");
//   irSerial.write(testCode, sizeof(testCode));
// }
