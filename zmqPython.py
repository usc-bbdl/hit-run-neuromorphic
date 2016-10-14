import zmq
#from numpy import matrix
import numpy as np
import ipdb


float_vector = []

Matrix_M = []
Matrix_F = []

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
	counter = 0
	list_of_float = []
	row_M = []
	row_F = []
	for x in temp:
		try:
			list_of_float.append(float(x))
			counter = counter + 1
			if(counter < 8):
				row_M.append(float(x))
			else:
				row_F.append(float(x))
		except ValueError:
			continue

	Matrix_M.append(row_M)
	Matrix_F.append(row_F)
	return list_of_float

context = zmq.Context()
socket = context.socket(zmq.REQ)

import json
data1 = [0,0,0,0,0,0,0]
#range(2187)
index = 6
for it in range(2000):
	user_input = ""
	if user_input == "":

		hello = python_server(socket)
	
		numJson = json.dumps(data1)
		hello.sendMessage(numJson)
		print "what we are sending"
		print data1
		data1[index] = data1[index] + 4
		for x in reversed(xrange(7)):
			if(data1[x] == 12):
				data1[x] = 0
				data1[x-1] = data1[x-1] + 4
				for y in range(x,7):
					data1[y] = 0
		data1 = np.random.uniform(0,10,7)
		#data1 = data1*100
		#data1 = round(data1)
		#data1 = np.round(data1,2)
		data1 = data1.tolist()
		#data1 = data1/100;
		#data1 = float("{0:.2f}".format(data1))
		message = socket.recv()
		print "What we got from c++"
		print message
		print "message in list form"
		float_vector = string_to_float(message)
		print float_vector
		#h = json.dumps(message)
		#j = json.loads(h)

	else:
		message = socket.recv()
		#print message
	import time
	time.sleep(0.5)

#function that parse string to list of float number
print "Matrix M is"
print Matrix_M
print "Matrix F is"
print Matrix_F
print "Calculate"


#from numpy import linalg


#M = np.random.randn(100,7)
M = Matrix_M; 
#A = np.random.randn(7,1)
# ipdb.set_trace()
F = Matrix_F
Ahat = np.linalg.lstsq(M,F)[0]
#diffA = A - Ahat
FHat = np.matmul(M , Ahat)
diffF = F - FHat
sse = sum(diffF.flatten()**2)
print 'see(F-Fhat) is : ', sse

# Function called throughout John's code that gets the message
def run_t5(activations):
   return message