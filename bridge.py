from flask import Flask
from flask import request

import os
import sys

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

if __name__ == '__main__':
    app.run(port=port,host="0.0.0.0")
