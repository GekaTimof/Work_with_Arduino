#include <Arduino.h>

const unsigned long baudRate = 9600;
const unsigned long bitDuration = 1000000 / baudRate;  // Длительность одного бита (104 мкс для 9600 бод)

// Вычисление бита четности
bool calculateParity(unsigned char byte) {
    bool parity = 0;
    for (int i = 0; i < 8; i++) {
        parity ^= (byte >> i) & 1;  // XOR всех битов
    }
    return parity;  // Возвращаем четность: 0 - четная, 1 - нечетная
}

void sendByte(unsigned char data) {
    bool parity = calculateParity(data);

    // Стартовый бит
    Serial.write(LOW);  // Отправляем через TX
    delayMicroseconds(bitDuration);

    // 8 бит данных
    for (int i = 0; i < 8; i++) {
        Serial.write((data >> i) & 1);
        delayMicroseconds(bitDuration);
    }

    // Бит четности
    Serial.write(parity);
    delayMicroseconds(bitDuration);

    // Стоповый бит
    Serial.write(HIGH);
    delayMicroseconds(bitDuration);
}

void setup() {
    Serial.begin(baudRate);  // Открытие последовательного порта
    delay(1000);  // Задержка для стабильности
    Serial.println("Setup started");
}

void loop() {
    sendByte(0x55);  // Отправка данных (0x55 = 01010101)
    delay(1000);  // Задержка между отправками
}
