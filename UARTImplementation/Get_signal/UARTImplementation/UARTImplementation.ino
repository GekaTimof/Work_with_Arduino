#include <Arduino.h>

#define RX_PIN 2  // Пин для приема данных (INT0)

const unsigned long baudRate = 9600;
const unsigned long bitDuration = 1000000 / baudRate;  // Длительность одного бита (104 мкс для 9600 бод)
const unsigned long startBitDelay = bitDuration + (bitDuration / 2);  // Задержка 1.5 Tbit перед считыванием первого бита

volatile bool startDetected = false;
volatile unsigned char receivedByte = 0;
volatile bool byteReady = false;

// Функция для вычисления бита четности
bool calculateParity(unsigned char byte) {
    bool parity = 0;
    for (int i = 0; i < 8; i++) {
        parity ^= (byte >> i) & 1;  // XOR всех битов
    }
    return parity;  // Возвращаем четность: 0 - четная, 1 - нечетная
}

// Прерывание для обработки начала передачи
void ISR_INT0() {
    if (!startDetected) {
        startDetected = true;
        delayMicroseconds(startBitDelay);  // Задержка 1.5 Tbit

        unsigned char data = 0;

        // Чтение 8 бит данных
        for (int i = 0; i < 8; i++) {
            if (digitalRead(RX_PIN) == HIGH) {
                data |= (1 << i);  // Считываем бит
            }
            delayMicroseconds(bitDuration);  // Задержка на каждый бит
        }

        // Чтение бита четности
        bool receivedParity = digitalRead(RX_PIN);
        delayMicroseconds(bitDuration);  // Задержка для бита четности

        // Проверка четности
        bool calculatedParity = calculateParity(data);

        if (calculatedParity == receivedParity) {
            receivedByte = data;
            byteReady = true;
        }

        startDetected = false;  // Сброс флага начала передачи
    }
}

void setup() {
    pinMode(RX_PIN, INPUT);
    Serial.begin(9600);
    Serial.println("Setup started");
    attachInterrupt(digitalPinToInterrupt(RX_PIN), ISR_INT0, FALLING);  // Прерывание на спадающий фронт
}

void loop() {
    if (byteReady) {
        byteReady = false;
        Serial.print("Received byte: ");
        Serial.println(receivedByte, BIN);  // Вывод полученных данных в бинарной форме
    }
}
