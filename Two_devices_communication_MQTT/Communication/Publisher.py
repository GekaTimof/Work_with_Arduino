from random import randint, random
import time
import paho.mqtt.client as mqtt_client
from uuid import getnode as get_mac
import hashlib
from Communication import Connect
from collections import deque
import requests


# Create connection
connection = Connect(port='/dev/ttyUSB0', baudrate=115200)

# set broker host (this is free brokers host)
broker = "broker.emqx.io"

# det publisher id by getting hash from mac address
h = hashlib.new('sha256')
mac = get_mac()
h.update(str(mac).encode())
pub_id = h.hexdigest()[:10]

# send publisher id
response = requests.post("http://localhost:5000/set_value", json={"value": pub_id})

if response.status_code == 200:
    print("Pub_id sent successfully:", response.json())
else:
    print("Failed to send pub_id")
    RuntimeError("Publisher id wasn't created")

# set client and git him his id (pub_id)
client = mqtt_client.Client(
    mqtt_client.CallbackAPIVersion.VERSION2,
    pub_id
)

# start sending data loop
print("Connection to broker - ", broker)
print(client.connect(broker))
client.loop_start()
print("Publishing")

# start data stream
time.sleep(2)
print(connection.turn_photo_stream(toggle=True))


# args to stop cycle
start_time = time.time()
duration_sec = 3600

# args to calculate duration
last_time_1_sec = time.time()
last_time_2_sec = time.time()
last_time_5_sec = time.time()
last_time_dynamic = time.time()

# create que
queue = deque(maxlen=100)
# create min and max parameters
min_state:float = 1024
max_state:float = 0

interval_1_sec = 1
interval_2_sec = 2
interval_5_sec = 5
dynamic_interval = 2
dynamic_argument = 0.5
dynamic_go_down = True

print("Start stream")
while True:
    current_time = time.time()

    # send state data to stream
    if current_time - last_time_dynamic >= dynamic_interval:
        state = connection.get_photo_data_from_stream()
        if state:
            # client.publish(f"lab/{pub_id}/photo/stream", state)
            # add state to que
            queue.append(state)

            # check max
            if state > max_state:
                max_state = state
            # check max
            if state < min_state:
                min_state = state

            # go interval down
            if dynamic_go_down and dynamic_interval > dynamic_argument:
                dynamic_interval -= dynamic_argument
        else:
            dynamic_go_down = False
            dynamic_interval += dynamic_argument
            break

        last_time_dynamic = current_time

    # send state data
    if current_time - last_time_1_sec >= interval_1_sec and queue:
        client.publish(f"lab/{pub_id}/photo/instant", queue[-1])
        print(f"to instant - {queue[-1]}")
        last_time_1_sec = current_time

    # send state data average
    if current_time - last_time_2_sec >= interval_2_sec and queue:
        client.publish(f"lab/{pub_id}/photo/average", sum(queue) / len(queue))
        print(f"    to average - {sum(queue) / len(queue)}")
        last_time_2_sec = current_time

    # send min and nax values data
    if current_time - last_time_5_sec >= interval_5_sec and queue:
        client.publish(f"lab/{pub_id}/photo/min", )
        client.publish(f"lab/{pub_id}/photo/max", )
        print(f"           min - {min_state}, max - {max_state}")
        last_time_5_sec = current_time


    # stopping timer
    if current_time - start_time > duration_sec:
        break

# stop data stream
print(connection.turn_photo_stream(toggle=False))

# stop sending data
client.disconnect()
client.loop_stop()

