import threading
import socket
import sys
import RPi.GPIO as GPIO
import time
from encoder import Encoder

#feed
GPIO.setwarnings(False)
GPIO.setmode (GPIO.BOARD)
STEP=33
DIR=35
EN=37

GPIO.setup(STEP,GPIO.OUT)
GPIO.setup(DIR,GPIO.OUT)
GPIO.setup(EN,GPIO.OUT)

#helper
GPIO.setup(36,GPIO.OUT)

#cable roll

GPIO.setup(38,GPIO.OUT)
GPIO.setup(40,GPIO.OUT)
#motor encoder
GPIO.setup(11,GPIO.OUT)
GPIO.setup(13,GPIO.OUT)

sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
port=int(input("Enter port : "))
server_address = ('172.20.10.3', port)

print >>sys.stderr, 'starting up on %s port %s' % server_address
sock.bind(server_address)
sock.listen(1)
fstart=False
pstart=False
phelpstart=False

lock = threading.Lock()
event = threading.Event()
global status
global connection
status = False

def valueChanged(value):
	pass

e1 = Encoder(31, 29, valueChanged)

def send():
	global status
	global connection
	while status:
		event.wait(0.05)
		lock.acquire()
		connection.sendall(str(format(2 * 22 * 25.91 * e1.getValue() / 1024 / 7 / 10, '.2f')))
		#connection.sendall(bytearray(str(i), "utf8"))
		print >> sys.stderr, 'send : "%s"' % str(e1.getValue())
		lock.release()
	
try:
	while True:
		print >>sys.stderr, 'waiting for a connection'
		connection, client_address = sock.accept()
		thread1 = threading.Thread(target = send, args = ())
		try:
			print >>sys.stderr, 'connection from', client_address
			while True:
				print >>sys.stderr, 'waiting....'
				data = connection.recv(16)
				print >>sys.stderr, 'received "%s"' % data
				if data == 'e':
					if pstart == True:
						pstart = False
						p.stop()
					elif pstart == False:
						pstart = True
						p = GPIO.PWM(11,100)
						p.start(0)
						GPIO.output(13, 0)
						p.ChangeDutyCycle(100)
					if status:
						status = False
						thread1.join()
					else:
						status = True
						
					if not thread1.is_alive() and status:
						print('start thread...')
						thread1 = threading.Thread(target = send, args = ())
						thread1.start()
				elif data == 'n':
					if pstart == True:
						pstart = False
						p.stop()
					elif pstart == False:
						pstart=True
						p = GPIO.PWM(11,100)
						p.start(0)
						GPIO.output(13, 1)
						p.ChangeDutyCycle(100)
						#time.sleep(1)
						#p.stop()
					if status:
						status = False
						thread1.join()
					else:
						status = True
						
					if not thread1.is_alive():
						print('start thread...')
						thread1 = threading.Thread(target = send, args = ())
						thread1.start()
				elif data=='k':
					p = GPIO.PWM(11,100)
					en = 2 * 22 * 25.65 * e1.getValue() / 1024 / 7 / 10
					if en == 0:
					        goal = (2 * 22 * 25.65 * e1.getValue() / 1024 / 7 / 10) + 4.5
					else:
					        goal = (2 * 22 * 25.65 * e1.getValue() / 1024 / 7 / 10) + 4.5
					p.start(0)
					GPIO.output(13, 0)
					p.ChangeDutyCycle(100)
					#time.sleep(0.5)
					while en < goal:
					        print(en)
					        en = 2 * 22 * 25.65 * e1.getValue() / 1024 / 7 / 10
					p.stop()
				    	print("submit")        
				    	print >>sys.stderr, 'sending data back to the client'
				    	time.sleep(0.2)
				    	connection.sendall(str(format(2 * 22 * 25.65 * e1.getValue() / 1024 / 7 / 10, '.2f')))
			      
			    	elif data=='l':
				    	p = GPIO.PWM(11,100)
					p.start(0)
					GPIO.output(13, 1)
					p.ChangeDutyCycle(100)
					#time.sleep(0.5)
					en = 2 * 22 * 25.65 * e1.getValue() / 1024 / 7 / 10
					if en == 0:
					        goal = (2 * 22 * 25.65 * e1.getValue() / 1024 / 7 / 10) + 5
					else:
					        goal = (2 * 22 * 25.65 * e1.getValue() / 1024 / 7 / 10) + 4.5
					while en < goal:
					        en = 2 * 22 * 25.65 * e1.getValue() / 1024 / 7 / 10
					p.stop()
				    	print("submit")        
				    	print >>sys.stderr, 'sending data back to the client'
				    	connection.sendall(str(format(2 * 22 * 25.65 * e1.getValue() / 1024 / 7 / 10, '.2f')))
				else:
					print >>sys.stderr, 'no more data from', client_address
					break
		finally:
			connection.close()
except KeyboardInterrupt:
	connection.close()
	
	


			
