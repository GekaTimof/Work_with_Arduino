import time
import paho.mqtt.client as mqtt_client
from uuid import getnode as get_mac
import hashlib
from Communication import Connect

# Create connection
connection = Connect(port='/dev/ttyUSB0')

# set broker host (this is free brokers host)
broker = "broker.emqx.io"

# set publisher id
pub_id = "a879fd15cf"
# check publisher id
if not pub_id:
    raise RuntimeError("Publisher id is not defined")

# det subscriber id by getting hash from mac address
h = hashlib.new('sha256')
mac = get_mac()
h.update(str(mac).encode())
sub_id = h.hexdigest()[10:20]

# set our method on_message
def on_massage(client, userdata, message):
    data = int(message.payload.decode("utf-8"))
    print("received message = ", data)
    connection.send_led_signal(photo_val=data)
    print()
# set client and git him his id (sub_id)
client = mqtt_client.Client(
    mqtt_client.CallbackAPIVersion.VERSION2,
    sub_id
)
# overwrite on_message method
client.on_message=on_massage

# start getting data loop
print("Connection to broker - ", broker)
print(client.connect(broker))
client.loop_start()
print("Subscribing")

# subscribing for publisher
client.subscribe(f"home/{pub_id}/room/light")

# stop listening after time
time.sleep(1800)
client.disconnect()
client.loop_stop()
