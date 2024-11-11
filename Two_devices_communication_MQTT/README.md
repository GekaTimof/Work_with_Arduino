## Links
link to Circuit diagram - https://drive.google.com/file/d/1dF5dk004Fyz-sOwiLolLn_QRxGuYyVmW/view?usp=sharing

link to no Circuit diagram - https://drive.google.com/file/d/1nCKiJ300w9cQV-0H3kuj9g6RYBOdFq_9/view?usp=sharing

link to video demonstration - https://drive.google.com/file/d/10aUyXGXitjncQNUkmf5Vk8rLI8Y8rgcJ/view?usp=drive_link


## Description of the scheme operation
First, we launch API_to_pub_id.py , to get the publisher's ip.
After that, we launch Publisher.py it connects to the sensor, sends its id via the api and starts sending values.
Connecting Subscriber.py he gets the publisher's id through the api and starts waiting for the values for the selected topics

At startup Subscriber.py A graph will be displayed showing the current illumination value. (close the chart to start the ## Description of the scheme operation
First, we launch API_to_pub_id.py , to get the publisher's ip.
After that, we launch Publisher.py it connects to the sensor, sends its id via the api and starts sending values.
Connecting Subscriber.py he gets the publisher's id via the api and starts waiting for the values for the selected topics

At startup Subscriber.py A graph will be displayed showing the current illumination value. (close the graph to start processing the values)

The 3 pin light bulb turns on when the light level starts to drop

The 4-pin light bulb turns on when the light level falls below the average between maximum and minimum illumination
