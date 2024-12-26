import time
import random
import paho.mqtt.client as mqtt
import requests

# Конфигурация MQTT
mqtt_broker = "broker.emqx.io"  # Замените на адрес вашего MQTT брокера
mqtt_port = 1883  # Обычно 1883 для непришифрованного соединения
mqtt_topic = "sensor/topic"  # Тема для публикации


# Получаем топик с сервера (симуляция запроса на сервер)
def get_topic_from_server():
    url = "http://copift.ru:5000/get_id"  # Ваш сервер, который возвращает топик
    try:
        response = requests.get(url)
        if response.status_code == 200:
            payload = response.json()
            return payload.get("id", "")  # Предполагаем, что ответ содержит {"id": "topic_name"}
        else:
            print(f"Error: {response.status_code}")
            return None
    except Exception as e:
        print(f"Exception while requesting topic: {e}")
        return None


# Создание и отправка топика на сервер
def send_topic_to_server(topic):
    url = "http://copift.ru:5000/refresh"
    headers = {"Content-Type": "application/json"}
    payload = {"message": topic}

    try:
        response = requests.post(url, json=payload, headers=headers)
        if response.status_code == 200:
            print(f"Successfully sent topic '{topic}' to the server")
        else:
            print(f"Error while sending topic: {response.status_code}")
    except Exception as e:
        print(f"Exception while sending topic: {e}")


# Настройка клиента MQTT
def on_connect(client, userdata, flags, rc):
    print(f"Connected to MQTT Broker with result code {rc}")
    client.subscribe(mqtt_topic)


def on_message(client, userdata, msg):
    print(f"Message received: {msg.payload.decode()} on topic: {msg.topic}")


# Основная логика
def main():
    # Получаем топик с сервера
    topic_from_server = get_topic_from_server()
    if topic_from_server:
        print(f"Received topic from server: {topic_from_server}")
    else:
        topic_from_server = mqtt_topic
        print(f"Using default topic: {topic_from_server}")

    # Отправляем топик на сервер
    send_topic_to_server(topic_from_server)

    # Настроим MQTT клиент
    client = mqtt.Client()
    client.on_connect = on_connect
    client.on_message = on_message

    # Подключаемся к MQTT брокеру
    client.connect(mqtt_broker, mqtt_port, 60)
    client.loop_start()

    # Отправка случайных значений на топик
    try:
        while True:
            random_value = random.randint(0, 1024)
            print(f"Publishing {random_value} to topic {topic_from_server}")
            client.publish(topic_from_server, str(random_value))  # Отправляем случайное значение

            time.sleep(5)  # Публикуем значение каждую секунду
    except KeyboardInterrupt:
        print("Exiting...")
        client.loop_stop()
        client.disconnect()


if __name__ == "__main__":
    main()
