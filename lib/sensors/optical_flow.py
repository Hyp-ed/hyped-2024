from pmw3901 import PMW3901
import paho.mqtt.client as mqtt
import tomllib
import json
import time

file = open('pod.toml', 'rb')
config = tomllib.load(file)
mqttc = mqtt.Client()
mqttc.connect(config['mqtt']['host'], config['mqtt']['port'])


sensor = PMW3901(spi_cs=0)
mqttc = mqtt.Client()

while True:
    (x, y) = sensor.get_motion()
    measurement = {'header': {'timestamp': time.time(), 'priority': 1
                              }, 'payload': {'x': x, 'y': y}}
    mqttc.publish('hyped/cart_2024/measurement/optical_flow',
                  json.dumps(measurement))
    time.sleep(0.05)
