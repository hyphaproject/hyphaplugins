import sys
import json
import time
import RPi.GPIO as GPIO

def beep(pin, freq):
    b = GPIO.PWM(pin, freq)  
   
    b.start(90)
        #for dc in range(0, 11, 5):
        #    p.ChangeDutyCycle(dc)
        #    time.sleep(0.1)
        #for dc in range(10, -1, -5):
        #    p.ChangeDutyCycle(dc)
        #    time.sleep(0.1)
    b.ChangeDutyCycle(80)
    b.ChangeFrequency(freq/2)
    time.sleep(0.25)  
    b.ChangeDutyCycle(90)
    b.ChangeFrequecy(freq - freq/4)
    time.sleep(0.2)
    b.stop()

jsondata = json.loads(str(sys.argv[1]))
print(str(sys.argv[1]))
GPIO.setmode(GPIO.BOARD)

RED = 36
GREEN = 38
YELLOW = 40
BEEP = 37

GPIO.setup(RED, GPIO.OUT) #red
GPIO.setup(GREEN, GPIO.OUT) #green
GPIO.setup(YELLOW, GPIO.OUT) #yellow
GPIO.setup(BEEP, GPIO.OUT) #beep

if("beep" in jsondata):
    beep(BEEP, jsondata["beep"])

if("red" in jsondata):
    if(jsondata["red"] == False):
        GPIO.output(RED, GPIO.LOW)
    if(jsondata["red"] == True):
        GPIO.output(RED, GPIO.HIGH)
if("green" in jsondata):
    if(jsondata["green"] == False):
        GPIO.output(GREEN, GPIO.LOW)
    if(jsondata["green"] == True):
        GPIO.output(GREEN, GPIO.HIGH)
if("yellow" in jsondata):
    if(jsondata["yellow"] == False):
        GPIO.output(YELLOW, GPIO.LOW)
    if(jsondata["yellow"] == True):
        GPIO.output(YELLOW, GPIO.HIGH)
#GPIO.cleanup()
