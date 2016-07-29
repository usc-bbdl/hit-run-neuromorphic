import zmq

class python_server(object):
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
incrementor = 0
IDNumber = 0

for it in range(1000):
	user_input = ""
	if user_input == "":

		hello = python_server(socket)
		row.append(IDNumber)
		numJson = json.dumps(row)
		hello.sendMessage(numJson)
		print "waht we are sending"
		print row
		print "-------"
		del row [6]
		IDNumber += 1

		message = socket.recv()
		print message
		##dataVector = []

		##dataVector = message.split(',')
		##dataVector_1 = []
		##for it in dataVector:
		##	integer = int(it)
		##	dataVector_1.append(integer)

		##print dataVector_1
	else:
		message = socket.recv()
		print message
	import time
	time.sleep(1)
