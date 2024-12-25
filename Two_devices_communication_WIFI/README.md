## Links
[link to video Publisher demonstration](https://drive.google.com/file/d/11KLvUd20mJZmKCN8-AkuqEzPX_t_yO5l/view?usp=sharing)


[link to video Subscriber demonstration](https://drive.google.com/file/d/11LIt7iPLUHMeF3bmXPp0daPaZiJWSmjv/view?usp=sharing)


## Description of how the circuits works
First, we connect Publisher, it connects to WIFI according to the data from Config.h.

Then Publisher connects to the broker via MQTT and creates a topic there.

Then the Publisher sends the topic to the server, after which it begins to send data from the sensor to this topic.

The next step is to launch Subscriber, it also connects to WIFI according to the data from Config.h.

Then it accesses the server and gets the topic.

Then it is connected via MQTT to the topic and, depending on the received data, changes the LED state.
