from random import randint, random
import time
import paho.mqtt.client as mqtt_client
from uuid import getnode as get_mac
import hashlib
from Communication import Connect

# Create connection
connection = Connect(port='/dev/ttyUSB1')

# set broker host (this is free brokers host)
broker = "broker.emqx.io"

# det publisher id by getting hash from mac address
h = hashlib.new('sha256')
mac = get_mac()
h.update(str(mac).encode())
pub_id = h.hexdigest()[:10]
# send publisher id
print(f"Listen me at id - ",pub_id)

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

# send data 100 times
for i in range(100):
    # get array of data to send
    state_arr = connection.get_array_photo_data()
    if state_arr:

        average_state = int(sum(state_arr) / len(state_arr))
        print(f"state_arr - {state_arr}")
        print(f"average_state - {average_state}")

        state = state_arr[0]
        print(f"state - {state}")

        # send state data
        client.publish(f"lab/{pub_id}/photo/instant", state)

        # send state data array
        client.publish(f"lab/{pub_id}/photo/average", state)

    # wait before next sending
    print()
    time.sleep(0.1)


# stop sending data
client.disconnect()
client.loop_stop()

