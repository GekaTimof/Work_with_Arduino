import serial

num = 100


class Connect:
    responses = {'d': 7,  # resp -> "led off"
                 'u': 6,  # resp ->"led on"
                 'p': 3,  # resp -> [0:255], fix size - zero fill to left side
                 'a': 3*num}  # resp -> [0:255], fix size - zero fill to left side

    def __init__(self, port='/dev/ttyUSB0'):
        self.port = port
        self.connection = serial.Serial(self.port, baudrate=9600, timeout=2)

    def send_command(self, cmd: str, response_len: int, connection: serial.Serial):
        str_resp = ''
        connection.write(cmd.encode())
        if response_len > 0:
            resp: bytes = connection.read(response_len)
            str_resp = resp.decode()
        return str_resp


    def get_photo_data(self):
        print(self.port)
        photo_val_resp: str = self.send_command(cmd='p', response_len=self.responses['p'], connection=self.connection)
        if photo_val_resp:
            photo_val = int(photo_val_resp) * 4
            # print(f"GET photo value - {photo_val}")
            return photo_val
        else:
            # print(f"NO photo value")
            return 0


    def split_string(self, s, l=3):
        return [int(s[i:i + l]) for i in range(0, len(s), l)]


    def get_array_photo_data(self):
        photo_val_resp_all: str = self.send_command(cmd='a', response_len=self.responses['a'], connection=self.connection)
        if photo_val_resp_all:
            photo_val_resp = self.split_string(s=photo_val_resp_all, l=int(self.responses['a']/num))
            # print(f"GET photo values - {photo_val_resp}")
            return photo_val_resp
        else:
            # print(f"NO photo value")
            return 0



    def send_led_signal(self, photo_val: int):
        if photo_val * 4 > 500:
            print(f"SEND turn ON signal")
            resp = self.send_command('u', self.responses['u'], connection=self.connection)
        else:
            print(f"SEND turn OFF signal")
            resp = self.send_command('d', self.responses['d'], connection=self.connection)
        print(resp)
        return True
