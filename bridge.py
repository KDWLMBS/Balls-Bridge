from flask import Flask
from flask import request
from array import array

import os
import sys
import time
import spidev
import RPi.GPIO as GPIO
#https://github.com/doceme/py-spidev
import spidev
import struct
import random

# PINS
LASER_PIN=10
CONTROLLER1_PIN=7
CONTROLLER2_PIN=8

# OTHER CONFIGS
SPI_SPEED=3000000
MOTOR_COUNT=30




#https://stackoverflow.com/questions/12173774/modify-bits-in-an-integer-in-python
def set_bit(v, index, x):
  """Set the index:th bit of v to 1 if x is truthy, else to 0, and return the new value."""
  mask = 1 << index   # Compute mask, an integer with just bit 'index' set.
  v &= ~mask          # Clear the bit indicated by the mask (if x is False)
  if x:
    v |= mask         # If x was True, set the bit indicated by the mask.
  return v

class Controller:
    def __init__(self, name, motorCount, motorStartIndex):
        self.name = name
        self.motorCount = motorCount
        self.motorStartIndex = motorStartIndex
        self.motors = []
        for i in range(motorCount):
            motor = Motor(i + motorStartIndex)
            self.motors.append(motor)
            self.motors.randomPosition()

class Motor:
    minPosition = -1000
    maxPosition = 1000

    def __init__(self, index):
        self.position = 0
        self.target = 0
        self.driveBit = (index * 2)
        self.directionBit = (index * 2) + 1
        self.index = index

    # pass the 32-bit integer. we will set our 2 bytes
    def drive(self, data):
        if self.position == self.target:
            data = set_bit(data, self.driveBit, 0)
        else:
            data = set_bit(data, self.driveBit, 1)
            if(self.target > self.position):
                self.position += 1
                data = set_bit(data, self.directionBit, 1)
            else:
                self.position -= 1
                data = set_bit(data, self.directionBit, 0)
        return data

    def randomPosition(self):
        self.target = random.randrange(self.minPosition, self.maxPosition)

motors0 = []
motors1 = []

for i in range(MOTOR_COUNT / 2):
    motor = Motor(i)
    motors0.append(motor)
    motors0.randomPosition()
    data0 = motor.drive(data0)


GPIO.setmode(GPIO.BOARD)
GPIO.setup(CONTROLLER1_PIN, GPIO.IN, pull_up_down=GPIO.PUD_DOWN)
GPIO.setup(CONTROLLER2_PIN, GPIO.IN, pull_up_down=GPIO.PUD_DOWN)
GPIO.setup(LASER_PIN, GPIO.IN, pull_up_down=GPIO.PUD_DOWN)

spi = spidev.SpiDev()
spi.open(0, 0)
spi.max_speed_hz = SPI_SPEED


if 'BRIDGE_PORT' in os.environ:
  port = os.environ['BRIDGE_PORT']
else:
  port = 8080

app = Flask(__name__)

@app.route('/')
def index():
  return "Balls-Bridge is running!"

@app.route('/send', methods=['POST'])
def send():
  data = request.json['data']
  #TODO send data to controller
  return "ok"

def send_positions(x):
    global data
    print "writing " + str(bin(data))
    splitted = struct.unpack('4B', struct.pack('>I', data))
    for numb in splitted:
        print hex(numb)
    spi.writebytes(splitted)

def laser_interrupted(x):
    print "omg laser interrupted"

GPIO.add_event_detect(CONTROLLER1_PIN, GPIO.RISING, callback=send_positions, bouncetime=300)
GPIO.add_event_detect(CONTROLLER2_PIN, GPIO.RISING, callback=send_positions, bouncetime=300)
GPIO.add_event_detect(LASER_PIN, GPIO.RISING, callback=laser_interrupted, bouncetime=300)

try:
    if __name__ == '__main__':
        # todo: change this to 127.0.0.1 for production
        app.run(port=port,host="0.0.0.0")
except KeyboardInterrupt:
    print "cleanup after KeyboardInterrupt"
    GPIO.cleanup()
    spi.close()
except:
    print "cleanup after except"
    GPIO.cleanup()
    spi.close()
