import time
import serial
import numpy as np
import matplotlib.pyplot as plt

# pip install pyserial

get_data_cmd = "g"
responses = {get_data_cmd: 8} # resp -> [0000:1023]+[0000:1023], fix size - zero fill to left side

port = '/dev/ttyUSB0'
connection = serial.Serial(port, baudrate=9600, timeout=5)

min_percentage = 0.9

def send_command(cmd: str, response_len: int, connection: serial.Serial):
    str_resp = ''
    # buffer clearing
    connection.flushInput()
    connection.write(cmd.encode())

    if response_len > 0:
        resp: bytes = connection.read(response_len)
        str_resp = resp.decode(errors='ignore').strip()
        # print(f"raw: {resp}, decoded: '{str_resp}'")
    return str_resp


def full_percentage(measurements_dict: dict):
    full_keys = 0
    for key in measurements_dict.keys():
        if measurements_dict[key] is not None:
            full_keys += 1

    return full_keys / len(measurements_dict.keys())


def fill_None_values(data: dict):
    sorted_keys = sorted(data.keys())

    for i, key in enumerate(sorted_keys):
        if data[key] is None:
            if i == 0:
                data[key] = next(v for v in data.values() if v is not None)
            elif i == len(sorted_keys) - 1:
                data[key] = data[sorted_keys[i - 1]]
            else:
                left = data[sorted_keys[i - 1]]
                right = next((data[k] for k in sorted_keys[i + 1:] if data[k] is not None), left)
                data[key] = (left + right) // 2
    return data


# get array with data from two sensors (min_l - min sm. distance, max_l - max sm. distance, step - step between measurements)
def get_data_array(min_l:int = 12, max_l:int = 80, step:int = 4):
    measurements_dict = {}
    for i in range(min_l, max_l, step):
        measurements_dict[i] = None

    while full_percentage(measurements_dict) < min_percentage:
        get_data_resp: str = send_command(cmd=get_data_cmd, response_len=responses[get_data_cmd], connection=connection)

        if get_data_resp:
            distance: int = int(get_data_resp[:4])
            s: int = int(get_data_resp[4:9])

            if min_l <= distance // step * step < max_l:
                measurements_dict[distance // step * step] = s
            print(measurements_dict)
            print(f"{round(full_percentage(measurements_dict) * 100)}% scanned")

    measurements_dict = fill_None_values(measurements_dict)

    return measurements_dict


def get_polynom_function(measurements_dict: dict, degree = 2):
    x = np.array(list(measurements_dict.keys()))
    y = np.array(list(measurements_dict.values()))

    print(x)
    print(y)

    coefficients = np.polyfit(x, y, degree)

    poly_func = np.poly1d(coefficients)
    return poly_func



data = get_data_array()
x = np.array(list(data.keys()))
y = np.array(list(data.values()))
poly_func = get_polynom_function(data)
print(poly_func)

# generate test data
x_plot = np.linspace(0, max(x), 100)
y_plot = poly_func(x_plot)

plt.scatter(x, y, color='red', label='real data')
plt.plot(x_plot, y_plot, label=f'function prediction', color='blue')
plt.xlabel("distance")
plt.ylabel("sensor value")
plt.legend()
plt.grid()
plt.show()