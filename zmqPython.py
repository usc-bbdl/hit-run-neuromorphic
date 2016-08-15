import zmq

float_vector = []

class python_server(object):
	context = zmq.Context()
	socket = context.socket(zmq.REQ)
	def __init__(self, socket):
		self.socket = socket
		self.socket.connect("tcp://localhost:5555")

	def sendMessage(self, message):
		socket.send(message)
	
def string_to_float(message):
	temp = message.split(',')
	list_of_float = []
	for x in temp:
		try:
			list_of_float.append(float(x))
		except ValueError:
			continue
	return list_of_float

context = zmq.Context()
socket = context.socket(zmq.REQ)

import json
data1 = [0,0,0,0,0,0,0]

index = 6
for it in range(1000):
	user_input = ""
	if user_input == "":

		hello = python_server(socket)
	
		numJson = json.dumps(data1)
		hello.sendMessage(numJson)
		print "what we are sending"
		print data1
		data1[index] = data1[index] + 1
		for x in range(7):
			if(data1[x] == 16):
				data1[x] = 0
				data1[x-1] = data1[x-1] + 1
				for y in range(x,7):
					data1[y] = 0
				break

		message = socket.recv()
		print "What we got from c++"
		print message
		print "message in list form"
		float_vector = string_to_float(message)
		print float_vector

	else:
		message = socket.recv()
		print message
	import time
	time.sleep(0.5)

#function that parse string to list of float number

