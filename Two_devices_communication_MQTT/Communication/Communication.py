import serial

num = 100


class Connect:
    responses = {'d': 7,  # resp -> "led off"
                 'u': 6,  # resp -> "led on"
                 's': 9,    # start stream -> "stream ON"
                 'e': 10,   # end stream -> "stream OFF"
                 'p': 3}  # resp -> [0:255], fix size - zero fill to left side


    def __init__(self, port='/dev/ttyUSB0', baudrate=9600):
        self.port = port
        self.connection = serial.Serial(self.port, baudrate=baudrate, timeout=2)

    def send_command(self, cmd: str, response_len: int, connection: serial.Serial):
        str_resp = ''
        connection.write(cmd.encode())
        if response_len > 0:
            resp: bytes = connection.read(response_len)
            str_resp = resp.decode()
        return str_resp

    def turn_photo_stream(self, toggle: bool):
        if toggle:
            photo_val_resp: str = self.send_command(cmd='s',
                                                response_len=self.responses['s'],
                                                connection=self.connection)
        else:
            photo_val_resp: str = self.send_command(cmd='e',
                                                response_len=self.responses['e'],
                                                connection=self.connection)

        if photo_val_resp:
            print(f"operation {photo_val_resp} is completed")
            return True
        else:
            print(f"operation {photo_val_resp} is failed")
            return False

    def get_photo_data_from_stream(self):
        resp = self.send_command(cmd='p',
                                response_len=self.responses['p'],
                                connection=self.connection)
        result = 0
        if resp:
            result = int(resp) * 4
        return result


    def get_photo_data(self):
        # print(self.port)
        photo_val_resp: str = self.send_command(cmd='p',
                                                response_len=self.responses['p'],
                                                connection=self.connection)
        if photo_val_resp:
            photo_val = int(photo_val_resp) * 4
            # print(f"GET photo value - {photo_val}")
            return photo_val
        else:
            # print(f"NO photo value")
            return 0


    def split_string(self, s, l=3):
        return [int(s[i:i + l]) for i in range(0, len(s), l)]


    def send_led_signal(self, turn:bool = True, lead: int = 3 ):
        if turn:
            print(f"SEND turn ON signal")
            resp = self.send_command(cmd=f'u{lead}',
                                     response_len=self.responses['u'],
                                     connection=self.connection)
        else:
            print(f"SEND turn OFF signal")
            resp = self.send_command(cmd=f'd{lead}',
                                     response_len=self.responses['d'],
                                     connection=self.connection)
        print(resp)
        return True
