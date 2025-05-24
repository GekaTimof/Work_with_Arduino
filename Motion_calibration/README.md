## Links

[link to video demonstration](https://drive.google.com/file/d/138RgA3yOCfYzPGSnz6BsTojOpLlt7Vag/view?usp=drive_link)


## Description of how the circuit works
**1. Connection and start**
Upload the sketch to Arduino.
Connect the power, make sure the Bluetooth module is working (BTSerial).
Open the Serial Monitor or connect via Bluetooth.

**2. Switching modes**
Control is carried out via Bluetooth commands (or via Serial):
'X' - Enable/disable the calibration mode (Calibration mode).
'S' - Enable/disable the movement mode (Move mode).
'C' - Enable/disable the wheel calibration mode (Wheels mode).
'T' - Enable/disable the state machine mode (State mode).

**3. Calibration of movement directions**
In calibration mode ('X'), use the commands:
'F' — Forward
'B' — Backward
'L' — Left
'R' — Right

Each press cyclically changes the direction calibration (goes through the phases of movement). After pressing, the robot briefly moves in the selected direction.
**4. Calibration of wheel speed**
In Wheels mode ('C'):
'L' — Select left wheel
'R' — Select right wheel
'F' — Increase the speed of the selected wheel
'B' — Decrease the speed of the selected wheel

The speed changes in steps of 10 (from 100 to 260). After changing the speed, the robot briefly moves forward for testing.
**5. Testing the operation**
In Move mode ('S'), use 'F', 'B', 'L', 'R' to test the movement.
In State mode ('T') the robot will move automatically using distance sensors.
