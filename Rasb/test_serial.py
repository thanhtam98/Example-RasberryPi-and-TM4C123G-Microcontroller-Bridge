import time
import serial
import struct   
data_frame_read = [0xAA, 0xA0, 0x01 ,0x03, 0x01 ,0x01 , 0x12 ,0x34, 0xAE ,0xC5, 0x0, 0x0]
data_frame_write = []
def serial_process_read():
    data = ser.read()
    # print(data.encode('hex'))
    if(data == '\x12' ):       #if data is start bit
        L   = ord(ser.read())
        print(L)
        ID  = ser.read()
        FT  = ser.read()
        CMD_num1 = ser.read()
        CMD_num2 = ser.read()
        print(type(data))
        data_cmd = []
        for x in range (L- 6):
            data_cmd.append(ser.read())
        Cks_1 = ser.read()
        Cks_2 = ser.read()
        end = ser.read()
        if (end != '\x13'):  # check if start bit comflict with another data in frame.
            print("data read fail")
            pass

        print(L,ID,FT,CMD_num1,CMD_num2,data_cmd,Cks_1,Cks_2)
        global data_frame_read    
        data_frame_read = [L,ID,FT,CMD_num1,CMD_num2,data_cmd,Cks_1,Cks_2]
        
def serial_process_write(ID,FT,CMD_num1,CMD_num2,data,Cks_1,Cks_2):

    L = 6 + len(data)
    ser.write('\x12')
    # ser.write(bytes(L))
    # ser.write(bytes(ID))
    # ser.write(bytes(FT))
    # ser.write(bytes(CMD_num1))
    # ser.write(bytes(CMD_num2))
    # ser.write(bytes(data))
    # ser.write(bytes(Cks_1))
    # ser.write(bytes(Cks_2))
    print(L)
    ser.write(struct.pack('>B', L))
    ser.write(struct.pack('>B', ID))
    ser.write(struct.pack('>B', FT))
    ser.write(struct.pack('>B', CMD_num1))
    ser.write(struct.pack('>B',CMD_num2))

    for i in range (L-6):
        ser.write(struct.pack('>B', data[i]))
    ser.write(struct.pack('>B',Cks_1))
    ser.write(struct.pack('>B', Cks_2))
    ser.write('\x13')
    print("data write successful")

    



ser = serial.Serial(port='/dev/ttyS0',
               baudrate = 115200,
               parity=serial.PARITY_NONE,
               stopbits=serial.STOPBITS_ONE,
               bytesize=serial.EIGHTBITS,
               timeout=1)
counter=0
print("Serial init successful")         
      
while True:
    # serial_process_read()
    serial_process_write(0x01,0x02,0x03,0x04,[0x08,0x08],0x06,0x07)
    time.sleep(1)
    counter += 1
  