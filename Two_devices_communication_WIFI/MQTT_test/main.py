import requests
import paho.mqtt.client as mqtt


# Получение топика через HTTP-запрос
def get_topic():
    url = "http://copift.ru:5000/get_id"
    try:
        response = requests.get(url)
        response.raise_for_status()  # Проверяем, что запрос успешен
        data = response.json()  # Предполагается, что сервер возвращает JSON
        topic = data.get("pub_id")  # Извлекаем значение "id" из JSON
        if not topic:
            raise ValueError("Topic not found in server response")
        print(f"Received topic: {topic}")
        return topic
    except Exception as e:
        print(f"Error while getting topic: {e}")
        raise


# Callback для обработки входящих сообщений
def on_message(client, userdata, msg):
    print(f"Received message: {msg.payload.decode()} from topic: {msg.topic}")


# Основная программа
if __name__ == "__main__":
    try:
        # Получение топика
        mqtt_topic = get_topic()

        # Настройка MQTT клиента
        broker = "broker.emqx.io"
        port = 1883
        client = mqtt.Client()

        # Устанавливаем callback для обработки сообщений
        client.on_message = on_message

        # Подключение к брокеру
        print("Connecting to MQTT broker...")
        client.connect(broker, port)

        # Подписка на топик
        print(f"Subscribing to topic: {mqtt_topic}")
        client.subscribe(mqtt_topic)

        # Запуск обработки сообщений
        print("Listening for messages...")
        client.loop_forever()

    except Exception as e:
        print(f"An error occurred: {e}")
