import RPi.GPIO as GPIO
import time

GPIO.setmode(GPIO.BOARD)
PIR = 40

GPIO.setup(PIR, GPIO.IN)

i = 0
while True:
	time.sleep(0.01)
	i = i + 1
	if(i > 100):
		break
	if(GPIO.input(PIR)):
		print("movement")
		break

