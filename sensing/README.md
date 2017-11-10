### HARDWARE
Arduino UNO <br>
Grove Base Shield - [wiki](http://wiki.seeed.cc/Base_Shield_V2/) <br>
Grove Sound Sensor - [wiki](http://wiki.seeed.cc/Grove-Sound_Sensor/) <br>
Grove Temperature sensor - [wiki](http://wiki.seeed.cc/Grove-Temperature_Sensor_V1.2/) <br>
Grove Relay - [wiki](http://wiki.seeed.cc/Grove-Relay/)

### HOOK UP
Connect the sound sensor to the A0 header on the shield, the temperature sensor to A1 and the relay breakout to D1.

### HOW DOES IT WORK
The level of noise is remapped between 1 and 6, the serial reading is captured by the python script and poked to the server.
There is a threshold set to 22 degrees celsius, if the temperature is higher than that value, the relay is triggered.
