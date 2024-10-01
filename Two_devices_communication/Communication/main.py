# pip install pyserial
from xml.etree.ElementTree import tostring
from random import randint
import time
import serial


responses = {'d': 7, # resp -> "led off"
             'u': 6, # resp ->"led on"
             'p': 4} # resp -> [0:1023], fix size - zero fill to left side

port_photo = '/dev/ttyUSB0'
port_led = '/dev/ttyUSB0'
connection_photo = serial.Serial(port_photo, baudrate=9600, timeout=2)
connection_led = serial.Serial(port_led, baudrate=9600, timeout=2)


def send_command(cmd: str, response_len: int, connection: serial.Serial):
    str_resp = ''
    connection.write(cmd.encode())
    if response_len > 0:
        resp: bytes = connection.read(response_len)
        str_resp = resp.decode()
        print(resp, str_resp)
    return str_resp


while True:
    time.sleep(1)
    # get data from sensor
    photo_val_resp: str = send_command(cmd='p', response_len=responses['p'], connection=connection_photo)
    #photo_val_resp = randint(0,1023)
    if photo_val_resp:
        photo_val = int(photo_val_resp)
        #send command depend on result
        if photo_val > 500:
            resp = send_command('u', responses['u'], connection=connection_led)
        else:
            resp = send_command('d', responses['d'], connection=connection_led)
        print(resp)