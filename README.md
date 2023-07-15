# TOPP_MINIPROJEKT
Studentec Project

# Pin useage
Servo1 auf pin 13 -> SPN Dir
Servo2 auf pin 12 -> SPN En

ECHO1
TRIG1

ECHO2
TRIG2

ECHO3
TRIG3

TX auf TX
RX auf RX

# Serial Communication 
The GUI, the ESP32 and the Arduino Mega communicate via Commands in ASCII code

| byte value | char | command type | optional value |
| --- | --- | --- | --- |
| 48-57 | 0-9 | move direction | | 
| 97-104 | a-h | move direction | | 
| 119 | w | spit water | |
| 112 | p | place lighter | |
| 108 | l | rotate left | |
| 114 | r | rotate right | |
| 118 | v | toggle cam | bool(48/49) |
| 109 | m | toggle auto drive | bool |
| 107 | k | set brightness | byte (A-Z) |
| 115 | s | set speed | byte |
