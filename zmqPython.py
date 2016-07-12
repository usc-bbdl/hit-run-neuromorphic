
import zmq

class test(object):
	context = zmq.Context()
	socket = context.socket(zmq.REQ)
	def __init__(self, socket):
		self.socket = socket
		self.socket.connect("tcp://localhost:5555")

	def sendMessage(self, message):
		socket.send(message)
	
context = zmq.Context()
socket = context.socket(zmq.REQ)

import json
row = [1,2,3,4,5,6]
row_json = json.dumps(row)
print row_json
incrementor = 0;

for it in range(10):
	user_input = raw_input("waiting for user's input:")
	if user_input == "":

		hello = test(socket)
		numJson = json.dumps(incrementor)
		hello.sendMessage(numJson)
		print numJson;
		incrementor += 1
		print "-------"


		message = socket.recv()
		print message
		dataVector = []

		dataVector = message.split(',')
		dataVector_1 = []
		for it in dataVector:
			integer = int(it)
			dataVector_1.append(integer)

		print dataVector_1
	else:
		message = socket.recv()
		print message
