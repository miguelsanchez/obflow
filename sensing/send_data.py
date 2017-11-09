import serial
from subprocess import call

raw = serial.Serial('/dev/tty.usbmodem1411', 115200)


x = 1
while True:
	noise = raw.readline()
	call(['/opt/oblong/Greenhouse/bin/poke', '-d', 'greenhouse', '-d', 'noise', '-i', 'noise:' + noise, 'tcp://eixample.local/hackathon'])