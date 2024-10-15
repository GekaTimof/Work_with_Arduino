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
    # get data to send
    #state = "on" if randint(0, 1) else "off"
    state = connection.get_photo_data()
    print(f"state - {state}")

    if bool(state):
        # send data
        client.publish(f"home/{pub_id}/room/light", state)

    # wait before nex sending
    print()
    time.sleep(2)

# stop sending data
client.disconnect()
client.loop_stop()

