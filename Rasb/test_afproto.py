import serial
import struct
from afproto import afproto_frame_data
from afproto import afproto_get_data

import time

START_BIT = '\x12'
STOP_BIT = '\x13'

def serial_write_data(Len,ID,FT,CMDnumber,data):
    len_data = len(data)
    print(len_data)
    frame = struct.pack('>BBB'+'H'+'B'*len_data,Len,ID,FT,CMDnumber,*data)
    encoded_frame = afproto_frame_data(frame)
    ser.write(encoded_frame)

def serial_read_data():
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
        print("crc matched")
        Len     = data_decode[0]
        ID      = data_decode[1]
        FT      = data_decode[2]
        CMDnumber = data_decode [3:5]
        data    =  data_decode[5:]
        print(Len,ID,FT,CMDnumber,data)
        return Len,ID,FT,CMDnumber,data
ser = serial.Serial(port='/dev/ttyS0',
               baudrate = 115200,
               parity=serial.PARITY_NONE,
               stopbits=serial.STOPBITS_ONE,
               bytesize=serial.EIGHTBITS,
               timeout=1)
print("Serial Initial Successful")

while True:


    # serial_write_data(0x05,0x88,0x03,0x555,[0x56,0x68,0x69,0x80])
    # serial_read_data()
    # time.sleep(1)
