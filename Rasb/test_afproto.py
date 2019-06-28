import serial
import struct
from afproto import afproto_frame_data
from afproto import afproto_get_data
import threading 
import time


START_BIT = '\x12'
STOP_BIT  = '\x13'

def serial_write_data(get_frame=[], *args):
    len_data = len(get_frame)
    # print(len_data)
    frame = struct.pack('B'*len_data,*get_frame)
    encoded_frame = afproto_frame_data(frame)
    ser.write(encoded_frame)
def serial_data_handler(frame=[], *args):
    
    print(frame)
    
def serial_read_data():
    while(True):
        IS_Read    = 0
        IS_Started = 0 
        frame_recevied = ""
        while (True):
            raw_data = ser.read()
            if raw_data == START_BIT:
                print("after start bit ")
                IS_Started=1
            if IS_Started == 1 :
                frame_recevied += raw_data
                print(raw_data)
            if raw_data == STOP_BIT:
                print("stop")
                break
        data_decode,raw_frame_data = afproto_get_data(frame_recevied)

        if (data_decode != None):
            print("crc matched and jump into handler")
            serial_data_handler(data_decode)
        
ser = serial.Serial(port='/dev/ttyS0',
               baudrate = 115200,
               parity=serial.PARITY_NONE,
               stopbits=serial.STOPBITS_ONE,
               bytesize=serial.EIGHTBITS,
               timeout=1)
print("Serial Initial Successful")

serial_data_thread = threading.Thread(target=serial_read_data)
serial_data_thread.start()

while True:

    payload= [0x09,0x02,0x02,0x01,0x00,0x00,0x05,0x04,0x69,0xAB,0x69,0xED]
    serial_write_data(payload)
    time.sleep(1)
