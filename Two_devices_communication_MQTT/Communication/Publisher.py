# pip install pyserial
from sqlite3 import connect
from xml.etree.ElementTree import tostring
from random import randint, random
import time
import serial
import paho.mqtt.client as mqtt_client
from uuid import getnode as get_mac
import hashlib

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
    state = "on" if randint(0, 1) else "off"
    print(f"state - {state}")

    # send data
    client.publish(f"home/{pub_id}/room/light", state)
    # wait before nex sending
    time.sleep(2)

# stop sending data
client.disconnect()
client.loop_stop()

# responses = {'d': 7, # resp -> "led off"
#              'u': 6, # resp ->"led on"
#              'p': 3} # resp -> [0:255], fix size - zero fill to left side
#
# port_photo = '/dev/ttyUSB1'
# port_led = '/dev/ttyUSB0'
# connection_photo = serial.Serial(port_photo, baudrate=9600, timeout=2)
# connection_led = serial.Serial(port_led, baudrate=9600, timeout=2)
#
#
# def send_command(cmd: str, response_len: int, connection: serial.Serial):
#     str_resp = ''
#     connection.write(cmd.encode())
#     if response_len > 0:
#         resp: bytes = connection.read(response_len)
#         str_resp = resp.decode()
#         print(resp, str_resp)
#     return str_resp
#
#
# while True:
#     #time.sleep(1)
#     # get data from sensor
#     photo_val_resp: str = send_command(cmd='p', response_len=responses['p'], connection=connection_photo)
#     #photo_val_resp = randint(0,1023)
#     if photo_val_resp:
#         photo_val = int(photo_val_resp) * 4
#         #send command depend on result
#         if photo_val > 500:
#             resp = send_command('u', responses['u'], connection=connection_led)
#         else:
#             resp = send_command('d', responses['d'], connection=connection_led)
#         print(resp)
