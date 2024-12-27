import paho.mqtt.client as mqtt
import time
import argparse

# How to run script
# python main.py Lamp/1234/turn

# MQTT settings
MQTT_BROKER = "broker.emqx.io"
MQTT_PORT = 1883

START_ON_TIME = 20  # Start time for light
START_OFF_TIME = 40  # End time for light
MIN_ON_TIME = 10  # Minimum light duration

def control_lamp(client, topic):
    """
    Control lamp based on time.
    """
    print(f"Publishing to topic: {topic}")
    on_time = START_ON_TIME
    off_time = START_OFF_TIME

    while True:
        # Get current second in the minute
        current_second = time.localtime().tm_sec

        # Turn the lamp on
        if current_second == on_time:
            print(f"Turning lamp ON. Time: {current_second}s")
            client.publish(topic, "ON")

        # Turn the lamp off
        if current_second == off_time:
            print(f"Turning lamp OFF. Time: {current_second}s")
            client.publish(topic, "OFF")

            # Update times for the next minute
            on_time += 1
            off_time -= 1
            if off_time - on_time < MIN_ON_TIME:
                on_time = START_ON_TIME
                off_time = START_OFF_TIME
                print("Resetting lamp times to default.")

        time.sleep(1)

def on_connect(client, userdata, flags, rc):
    """
    Handle connection to MQTT broker.
    """
    if rc == 0:
        print("Connected to MQTT broker successfully.")
        client.subscribe(userdata['topic'])
        print(f"Subscribed to topic: {userdata['topic']}")
    else:
        print(f"Failed to connect to MQTT broker. Code: {rc}")

def on_subscribe(client, userdata, mid, granted_qos):
    """
    Handle successful subscription to topic.
    """
    print(f"Successfully subscribed to topic: {userdata['topic']} with QoS {granted_qos}")

def main():
    parser = argparse.ArgumentParser(description="MQTT Client for controlling a lamp.")
    parser.add_argument("lamp_id", help="ID of the lamp (MQTT topic)")
    args = parser.parse_args()

    client = mqtt.Client(userdata={"topic": args.lamp_id})
    client.on_connect = on_connect
    client.on_subscribe = on_subscribe

    try:
        client.connect(MQTT_BROKER, MQTT_PORT, 60)
    except Exception as e:
        print(f"Failed to connect to MQTT broker: {e}")
        return

    topic = args.lamp_id

    client.loop_start()
    try:
        time.sleep(2)  # Wait for subscription to be confirmed
        control_lamp(client, topic)
    except KeyboardInterrupt:
        print("Stopping the client.")
    finally:
        client.loop_stop()
        client.disconnect()

if __name__ == "__main__":
    main()
