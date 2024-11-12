import time
import paho.mqtt.client as mqtt_client
from uuid import getnode as get_mac
import hashlib
import statistics
from numpy.ma.extras import average
from Communication import Connect
import requests
# stream visualisation
import matplotlib
matplotlib.use("TkAgg")
import matplotlib.pyplot as plt
import numpy as np
from collections import deque
import threading
from matplotlib.animation import FuncAnimation


# Create connection
connection = Connect(port='/dev/ttyUSB1', baudrate=9600)

# set broker host (this is free brokers host)
broker = "broker.emqx.io"

# set publisher id
response = requests.get("http://localhost:5000/get_value")

# check publisher id
if response.status_code == 200:
    pub_id = response.json()["value"]
    print(f"Received pub_id: {pub_id}")
else:
    print("Pub_id wasn't received")
    raise RuntimeError("Publisher id is not defined")


# det subscriber id by getting hash from mac address
h = hashlib.new('sha256')
mac = get_mac()
h.update(str(mac).encode())
sub_id = h.hexdigest()[10:20]

# data queue
data_points = 100
data_queue = deque([0] * data_points, maxlen=data_points)

# visualisation
fig, ax = plt.subplots()
line, = ax.plot(np.arange(data_points), np.zeros(data_points))
ax.set_ylim(-10, 10)

# update graphic function
def animate(i):
    line.set_ydata(data_queue)

ani = FuncAnimation(fig, animate, interval=10)


# create que
queue = deque(maxlen=5)
last_queue_value = 0

# min and max
max_state = 1024
min_state = 0

# set on_message callback
def on_massage(client, userdata, message):
    try:
        data = float(message.payload.decode("utf-8"))

        if message.topic ==  f"lab/{pub_id}/photo/instant":
            data_queue.append(data)
            queue.append(data)
            # print(f"get instant data - {data}")

        if message.topic ==  f"lab/{pub_id}/photo/average":
            # queue.append(data)
            # print(f"get average data - {data}")
            pass

        if message.topic ==  f"lab/{pub_id}/photo/min":
            min_state = data

        if message.topic ==  f"lab/{pub_id}/photo/max":
            max_state = data

    except ValueError:
        print("Received non-numeric data.")

# set client and git him his id (sub_id)
client = mqtt_client.Client(
    mqtt_client.CallbackAPIVersion.VERSION2,
    sub_id
)
# overwrite on_message method
client.on_message=on_massage

# Connect and start loop
print("Connecting to broker -", broker)
if client.connect(broker) == 0:
    print("Successfully connected to broker.")
else:
    print("Failed to connect to broker.")
client.loop_start()

# subscribing for publisher state data stream
client.subscribe(f"lab/{pub_id}/photo/stream")
# subscribing for publisher state data
client.subscribe(f"lab/{pub_id}/photo/instant")
# subscribing for publisher state data average
client.subscribe(f"lab/{pub_id}/photo/average")
# subscribing for publisher state data min
client.subscribe(f"lab/{pub_id}/photo/min")
# subscribing for publisher state data max
client.subscribe(f"lab/{pub_id}/photo/max")

# Display the plot
plt.ylim(0, 1200)
plt.show()

# args to stop cycle
start_time = time.time()
duration_sec = 3600

# args to calculate duration
last_time_6_sec = time.time()
interval_6_sec = 6
last_time_5_sec = time.time()
interval_5_sec = 5

# get data cycle
print("Start get data from stream")
while True:
    current_time = time.time()
    if current_time - last_time_6_sec >= interval_6_sec and queue:
        # check switching
        print(statistics.median(queue), last_queue_value)
        if statistics.median(queue) < last_queue_value:
            connection.send_led_signal(turn=True, lead=3)
        else:
            connection.send_led_signal(turn=False, lead=3)
        last_queue_value = max(queue) #+ 0.1
        last_time_6_sec = current_time

    if current_time - last_time_5_sec >= interval_5_sec:
        if statistics.median(queue) < (min_state + max_state) / 2:
            connection.send_led_signal(turn=True, lead=4)
        else:
            connection.send_led_signal(turn=False, lead=4)

    time.sleep(0.01)

    # stopping timer
    if current_time - start_time > duration_sec:
        break

# stop listening after time
client.disconnect()
client.loop_stop()
