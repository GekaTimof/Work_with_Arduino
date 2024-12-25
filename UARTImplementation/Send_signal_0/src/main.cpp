#include <Arduino.h>

const unsigned long baudRate = 9600;
const unsigned long bitDuration = 1000000 / baudRate;  // Длительность одного бита (104 мкс для 9600 бод)

// Вычисление бита четности
bool calculateParity(unsigned char byte) {
    bool parity = 0;
    for (int i = 0; i < 8; i++) {
        parity ^= (byte >> i) & 1;
    }
    return parity;  // Возвращаем четность
}

void sendByte(unsigned char data) {
    // Стартовый бит
    digitalWrite(2, LOW);
    delayMicroseconds(bitDuration);

    // 8 бит данных
    for (int i = 0; i < 8; i++) {
        digitalWrite(2, (data >> i) & 1);
        delayMicroseconds(bitDuration);
    }

    // Бит четности
    digitalWrite(2, calculateParity(data));
    delayMicroseconds(bitDuration);

    // Стоповый бит
    digitalWrite(2, HIGH);
    delayMicroseconds(bitDuration);
}

void setup() {
    pinMode(2, OUTPUT);
    digitalWrite(2, HIGH);  // Линия в состоянии покоя
}

void loop() {
    sendByte(0x55);  // Отправка данных (0x55 = 01010101)
    delay(1000);  // Задержка между отправками
}
