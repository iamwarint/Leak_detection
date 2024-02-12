import threading
import socket
import numpy as np
from tkinter import * 
import tkinter as tk
from scipy.io import wavfile
import serial
import matplotlib
matplotlib.use('TkAgg')
from xmodem import XMODEM
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
import matplotlib.pyplot as plt
import time
import sys
import pywt
import scipy.fftpack
import tensorflow as tf 
import os
from tensorflow import keras

global countfile
countfile = 0


def ceiling_division(n, d):
    q, r = divmod(n, d)
    return q #+ bool(r)
    
num_fft = 128
n_dim = num_fft
n_hidden_1 = num_fft
n_hidden_2 = num_fft
n_hidden_3 = num_fft

def predict(path_name):
	model = keras.models.load_model('./model/annmodel1_2_1_4bar')
	num_fft = 128
	filename = path_name
	samplerate, data = wavfile.read(filename)
	newdata = data[:]
	times = np.arange(len(newdata))
	numDec = 5
	data_dec1 = []
	for ind in range (0, len(newdata)):
    		if ind % numDec == 0:
    			data_dec1.append(newdata[ind])
	newdata=data_dec1
	newdata=(newdata/max(np.abs(newdata)))
	w = pywt.Wavelet('sym3') 
	cA, cD = pywt.dwt(newdata, wavelet=w, mode='constant')
	cA1,cD1 = pywt.dwt(cA, wavelet=w, mode='constant')
	yf2 = scipy.fftpack.fft(cD1)
	num = len(yf2)
	avr_num = ceiling_division(num, num_fft)
	absyf2 = np.abs(yf2)
	absyf2 = absyf2/max(absyf2)
	newx_train = []    
	for ind in range (0, num_fft):
		newx_train.append(np.sum(absyf2[avr_num * ind:avr_num * (ind + 1)]) / avr_num)
		
	newx_train = np.array(newx_train)
	x = newx_train
	prediction = model.predict(x.reshape(1,128))
	print(prediction)
	predicted_index = np.argmax(prediction, axis=1)
	print(predicted_index)
	if predicted_index[0] == 0:
		return ("Non-Leak")
	elif predicted_index[0] == 1:
		return ("Leak")
	else:
		return ("Unknown")


serRo=serial.Serial(port='/dev/ttyUSB0',baudrate=115200,parity=serial.PARITY_NONE,stopbits=serial.STOPBITS_ONE,bytesize=serial.EIGHTBITS)#timeout=1)
def getc(size, timeout=1):
        return serRo.read(size) or None
def putc(data, timeout=1):
        return serRo.write(data)  # note that this ignores the timeout
modem = XMODEM(getc, putc)

sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sock.setblocking(0)
sock.settimeout(1)
port=int(input("Enter port : "))
server_address = ('172.20.10.3', port)
sock.connect(server_address)
lock = threading.Lock()
global status
status = False

root = tk.Tk()
fig = plt.figure(1)
plt.ion()
t = np.arange(0.0,0.5,0.01)
s = np.sin(np.pi*t)
#plt.plot(t,s)
fig.line1, = plt.plot(t,s,'r-')
canvas = FigureCanvasTkAgg(fig, master=root)
plot_widget = canvas.get_tk_widget()
tee=StringVar()
tee.set(" Result = ")
distance=StringVar()
distance.set(" distance = 0 cm.")
# Creating a photoimage object to use image 
photo1 = PhotoImage(file = r"pic/4.png") 
photo2 = PhotoImage(file = r"pic/1.png") 
photo3 = PhotoImage(file = r"pic/3.png") 
photo4 = PhotoImage(file = r"pic/2.png") 
photo5 = PhotoImage(file = r"pic/mic2.png") 
photo6 = PhotoImage(file = r"pic/on.png") 
photo7 = PhotoImage(file = r"pic/off.png") 
photo8 = PhotoImage(file = r"pic/con.png") 
photo9 = PhotoImage(file = r"pic/robot.png") 
photo10 = PhotoImage(file = r"pic/for.png") 
photo11 = PhotoImage(file = r"pic/back.png") 
photo12 = PhotoImage(file = r"pic/cable.png") 
photo13 = PhotoImage(file = r"pic/star.png") 
photo14 = PhotoImage(file = r"pic/leaking.png") 
  
# Resizing image to fit on button 
photoimage1 = photo1.subsample(10, 10) 
photoimage2 = photo2.subsample(10, 10)  
photoimage3 = photo3.subsample(10, 10) 
photoimage4 = photo4.subsample(10, 10)
photoimage5 = photo5.subsample(20, 20)  
photoimage6 = photo6.subsample(20, 20)  
photoimage7 = photo7.subsample(20, 20)  
photoimage8 = photo8.subsample(20, 20)  
photoimage9 = photo9.subsample(20, 20)  
photoimage10 = photo10.subsample(20, 20)  
photoimage11 = photo11.subsample(20, 20)  
photoimage12 = photo12.subsample(20, 20)
photoimage13 = photo13.subsample(20, 20)
photoimage14 = photo14.subsample(20, 20)

root.resizable(0, 0)
def client2(direct):
        global thread1
        global thread2
        dis = ""
        print(sys.stderr,'connecting to',server_address[0],'port',server_address[1])

        try:
                        # Send data
                message = direct
                print(sys.stderr, 'sending', message)
                #sock.sendall(message)
                while True:
                        try:
                                data = sock.recv(1024)
                        except:
                                break
                sock.sendall(bytearray(message, "utf8"))
                amount_received = 0
                amount_expected = 1
                data = b''
                while 1:
                        try:
                                print('recv....')
                                data = sock.recv(8)
                        except:
                                continue
                        finally:
                                print(sys.stderr, 'received', data)
                                dis = str(data, 'utf-8')
                                break
        finally:
                print ("end.")
        if direct in ['k', 'l']:
                distance.set(" distance = " + dis  + " cm.")
                print(distance.get())
                display1.configure(text = distance.get())
                
def client(direct):
	global thread1
	global thread2
	global status
	dis = ""
	if not thread1.is_alive():
		thread1 = threading.Thread(target = send, args = (direct))
		thread1.start()
		thread1.join()
	if status:
		thread2 = threading.Thread(target = recv, args = ())
		thread2.start()
                
def recv():
	global status
	while status:
		amount_received = 0
		amount_expected = 1
		lock.acquire()
		try:
			data = sock.recv(8)
		except:
			if status:
				continue
			else:
				break
		finally:
			data = str(data, 'utf-8')
			distance.set(" distance = " + data  + " cm.")
			print(distance.get())
			display1.configure(text = distance.get())
		lock.release()
		
		if not status:
			break
				
def send(direct):
	global status
	#n = input("Enter : ")
	n = direct
	if status:
		status = False
	else:
		status = True
	print(n)
	#lock.acquire()
	#print("1")
	sock.sendall(bytearray(n, "utf8"))
	print("send :", n)
	#lock.release()
	
thread1 = threading.Thread(target = send, args = (''))
thread2 = threading.Thread(target = recv, args = ())

def robotmove(direct):
        serRo.close()
        serRo.open()
        print (direct)
        serRo.write(direct)
        time.sleep(1)
	
def update():
        global countfile
        serRo.close()
        serRo.open()
        stop=False
        com=""
        global display
        while 1:
                serRo.write("R".encode("utf-8"))  
                print ("send111111")
                while 1:
                        rcv = serRo.read(1).decode("utf-8")
                        com=com+rcv
                        if com=="K" or com=="ok." or com=="ok.":
                                # port.write("\r\nYou sent:" + repr(rcv))
                                print("yyy ="+repr(com))
                                stop=True
                                break
                if stop==True:
                        stop=False
                        break
        print("open")
        nametmp = 'sound'+ str(countfile) + '.wav'
        stream = open(nametmp, 'wb')
        modem.recv(stream)
        stream.close()
        serRo.close()
        print("sucess")
        s=predict('sound'+ str(countfile) +'.wav')
        f=open('answer.txt','w')
        f.write(s)
        f.close()
        fig = plt.figure(1)
        plt.ion()
        samplerate, data = wavfile.read(nametmp)
        times = np.arange(len(data))/float(samplerate)
        newdata = data[:]
        yf = scipy.fftpack.fft(newdata)
        absyf2 =  np.abs(yf)
        absyf2 = absyf2/max(absyf2)
        freq = np.arange(len(absyf2))*samplerate/len(absyf2)
        
        fig.line1.set_data(freq,absyf2)
        #plt.axis([0, len(freq), 0, 0.1])
        plt.xlabel('Freq')
        plt.ylabel('PSD')
        plt.xlim(0, 4000)
        plt.ylim(0, 1)
        fig.canvas.draw()
        f=open('answer.txt','r')
        ans=f.read()
        f.close()
        tee.set("result = " + ans)
        print(tee.get())
        display.configure(text = tee.get())
        countfile += 1
        
def update2():
        for i in range(1000):
                global countfile
                serRo.close()
                serRo.open()
                stop=False
                com=""
                global display
                while 1:
                        serRo.write("R".encode("utf-8"))  
                        print ("send111111")
                        while 1:
                                rcv = serRo.read(1).decode("utf-8")
                                com=com+rcv
                                if com=="K" or com=="ok." or com=="ok.":
                                        # port.write("\r\nYou sent:" + repr(rcv))
                                        print("yyy ="+repr(com))
                                        stop=True
                                        break
                        if stop==True:
                                stop=False
                                break
                print("open")
                nametmp = 'sound'+ str(countfile) + '.wav'
                stream = open(nametmp, 'wb')
                modem.recv(stream)
                stream.close()
                serRo.close()
                print("sucess")
                s=predict('sound'+ str(countfile) +'.wav')
                f=open('answer.txt','w')
                f.write(s)
                f.close()
                fig = plt.figure(1)
                plt.ion()
                samplerate, data = wavfile.read(nametmp)
                times = np.arange(len(data))/float(samplerate)
                newdata = data[:]
                yf = scipy.fftpack.fft(newdata)
                absyf2 =  np.abs(yf)
                absyf2 = absyf2/max(absyf2)
                freq = np.arange(len(absyf2))*samplerate/len(absyf2)
        
                fig.line1.set_data(freq,absyf2)
                #plt.axis([0, len(freq), 0, 0.1])
                plt.xlabel('Freq')
                plt.ylabel('PSD')
                plt.xlim(0, 4000)
                plt.ylim(0, 0.2)
                fig.canvas.draw()
                
                fig.canvas.flush_events()
                f=open('answer.txt','r')
                ans=f.read()
                f.close()
                tee.set("result = " + ans)
                print(tee.get())
                display.configure(text = tee.get())
                countfile += 1

Label(root, text="Encoder", image = photoimage8, compound = LEFT).grid(row=1,column=11)
Label(root, text="ROBOT", image = photoimage9, compound = LEFT).grid(row=1,column=13)
display = Label(root, text=tee.get(), image = photoimage14, compound = LEFT)
display.grid(row=1,column=6)
display1 = Label(root, text=distance.get(), image = photoimage14, compound = LEFT)
display1.grid(row=1,column=7)


plot_widget.grid(row=2, column=0,rowspan=7,columnspan=10)
tk.Button(root,text="Record",command=update2, image = photoimage5, compound = LEFT).grid(row=1, column=4)

tk.Button(root,text="Forward cable encoder",command=lambda:client('e'), compound = LEFT,image = photoimage12).grid(row=2, column=11)
tk.Button(root,text="backward cable encoder",command=lambda:client('n'), compound = LEFT,image = photoimage12).grid(row=3, column=11)
tk.Button(root,text="Forward cable 5 cm",command=lambda:client2('k'), compound = LEFT,image = photoimage12).grid(row=4, column=11)
tk.Button(root,text="backward cable 10 cm",command=lambda:client2('l'), compound = LEFT,image = photoimage12).grid(row=5, column=11)

tk.Button(root,command=lambda:robotmove('^'.encode("utf-8")), image = photoimage1, compound = LEFT).grid(row=2, column=13)
tk.Button(root,command=lambda:robotmove('v'.encode("utf-8")), image = photoimage2, compound = LEFT).grid(row=4, column=13)
tk.Button(root,command=lambda:robotmove('<'.encode("utf-8")), image = photoimage3, compound = LEFT).grid(row=3, column=12)
tk.Button(root,command=lambda:robotmove('>'.encode("utf-8")), image = photoimage4, compound = LEFT).grid(row=3, column=14)


root.mainloop()


			
