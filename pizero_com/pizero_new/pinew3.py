import serial
import subprocess
from xmodem import XMODEM
import  time
import RPi.GPIO as GPIO
import Queue

GPIO.setwarnings(False)
GPIO.setmode (GPIO.BOARD)
DIR=18 
EN=16 
GPIO.setup(EN,GPIO.OUT)
GPIO.setup(DIR,GPIO.OUT)
GPIO.setup(22,GPIO.OUT)
GPIO.setup(24,GPIO.OUT)
ser = serial.Serial("/dev/serial0", baudrate=115200, timeout=10.0)
def getc(size, timeout=1):
	return ser.read(size) or None
def putc(data, timeout=1):
	return ser.write(data)  # note that this ignores the timeout
modem = XMODEM(getc, putc)
com=""
pstart=False
print "start"
while True:
	#port.write("Say something:")
	ser.close()
	ser.open()
	# modem = XMODEM(getc, putc)
	print "wait input"
	print pstart
	rcv = ser.read()
	print("input="+rcv);
	com=com+rcv
	print(com)
	if com=="R" or com=="rec":
		# port.write("\r\nYou sent:" + repr(rcv))
		print("yyy ="+repr(com))
		com="" 
		data = subprocess.check_output("./a.out")
		stream = open('sound1.wav', 'rb')
		ser.write("K") 
		print("sending")
		modem.send(stream)
		print("sending sucess")
		stream.close()
		ser.close()
		

	elif com=='<':
		print("Left ="+repr(com))
		com=""
		if pstart==True:
		    pstart=False
		    p1.stop()
		    p2.stop()
		elif pstart==False:
		    pstart=True
		    p1 = GPIO.PWM(16,100)
		    p1.start(100)
		    GPIO.output(18, 1)
		    p1.ChangeDutyCycle(100)
		    #p2 = GPIO.PWM(22,100)   #38 40, 22 36
		    #p2.start(100)
		    #GPIO.output(24, 0)
		    #time.sleep(5)
                    #p1.stop()
                    #p2.stop()

	   

	elif com=='>':
		print("Right ="+repr(com))
		com=""
		if pstart==True:
		    pstart=False
		    p1.stop()
		    p2.stop()	
		elif pstart==False:
		    pstart=True
		    #p1 = GPIO.PWM(16,100)
		    #p1.start(100)
		    #GPIO.output(18, 1)
		    #p1.ChangeDutyCycle(100)
		    p2= GPIO.PWM(22,100)   #38 40, 22 36
		    p2.start(100)
		    GPIO.output(24, 0)
                    p2.ChangeDutyCycle(100)
		    #time.sleep(5)
                    #p1.stop()
                    #p2.stop()
	    
	  


	elif com=='^':
		print("Up ="+repr(com))
		com=""

		if pstart==True:
		    pstart=False
		    p1.stop()
		    p2.stop()
		elif pstart==False:
		    print "tee up"
		    pstart=True
		    p1 = GPIO.PWM(16,100)   #38 40, 22 36
		    p1.start(100)
		    GPIO.output(18, 1)
		    time.sleep(0.1)
		    p2 = GPIO.PWM(22,100)   #38 40, 22 36
		    p2.start(100)
		    GPIO.output(24, 0)
		    #p.ChangeDutyCycle(100)
		    #time.sleep(5)
                    #p1.stop()
                    #p2.stop()

	  
	elif com=='v':
		print("Down ="+repr(com))
		com=""
		if pstart==True:
		    pstart=False
		    p1.stop()
		    p2.stop()
		elif pstart==False:
		    pstart=True
		    p1 = GPIO.PWM(16,100)   #38 40, 22 36
		    p1.start(100)
		    GPIO.output(18, 0)
		    time.sleep(0.1)
		    p2 = GPIO.PWM(22,100)   #38 40, 22 36
		    p2.start(100)
		    GPIO.output(24, 1)
		    #p.ChangeDutyCycle(100)
		    #time.sleep(5)
                    #p1.stop()
                    #p2.stop()
         


        
        
        
