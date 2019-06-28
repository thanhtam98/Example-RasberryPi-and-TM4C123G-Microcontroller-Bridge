import paho.mqtt.client as mqtt
from test_data import GetTopic
from test_data import GetMessage
from test_data import process
import serial
import struct
from afproto import afproto_frame_data
from afproto import afproto_get_data
import threading 
import time
import test_data
import test_pub


START_BIT = '\x12'
STOP_BIT  = '\x13'

host = "192.168.1.92"  
port = 1883
keepalive = 60
payload = ""
pub_topic = "Request"   # de gui data/yeu cau len server
sub_topic = "Response/#"  # de nhan data tra ve tu server
dummy_topic = "dummy"

client = mqtt.Client("Rasberry 1")
client.username_pw_set("IoTteam","daihocbachkhoa")
#THONG
card_example = [0x12, 0x13, 0x93, 0x03, 0x01, 0x00, 0x00, 0x04, 0x15, 0xAB, 0x16, 0xed, 0x11, 0x11, 0x13]   # 4 bytes UID = FE A0 B2 C7
#TAM
#card_example = [0x12, 0x13, 0x93, 0x03, 0x01, 0x00, 0x00, 0x04, 0x96, 0xab, 0x96, 0xed, 0x11, 0x11, 0x13]

#ballentine's
barcode_example = [0x12, 0x13, 0x02, 0x04, 0x01, 0x00, 0x00, 0x04, 0x15, 0x12, 0x65, 0xAE, 0x11, 0x11, 0x13]
#hennessy
#barcode_example = [0x12, 0x13, 0x02, 0x04, 0x01, 0x00, 0x00, 0x04, 0xAB, 0x19, 0x52, 0xEF, 0x11, 0x11, 0x13]  # 4 bytes barcode = E9 5F D2 1C

#dispensing
dispensing_example = [0x12, 0x13, 0x02, 0x05, 0x01, 0x00, 0x00, 0x04, 0x69, 0xAB, 0x69, 0xED, 0x11, 0x11, 0x13] # 4 bytes RFID = 50 4E 13 7F

#sync

synchronize_example = [0x12, 0x08, 0x02, 0x06, 0x01, 0x00, 0x00, 0x11, 0x11, 0x13]
#example = barcode_example


received_msg = {
    "ID":"None",
    "CmdNumber":"None",
}
def on_connect(client, userdata, flags, rc):
    if rc==0:
        client.connected_flag=True #set flag
        print("Connected to Broker")
    else:
        print("Bad connection Returned code=",rc)
        client.bad_connection_flag=True
    client.subscribe(sub_topic, qos = 2)
    
def on_disconnect(client, userdata, rc):
    logging.info("Disconnecting reason  "  +str(rc))
    client.connected_flag=False
    client.disconnect_flag=True
    
def on_message(client, userdata,message):
    #print("Message: "+str(message.payload) + "Topic: " + message.topic + "QoS: " +str(message.qos))
    #client.publish(pub_topic, payload, qos = 2, retain = False)    #payload + str(mid)
    print("Topic: "+message.topic)
    process(message.topic,message.payload)
    client.disconnect()
#def on_publish(client, userdata, mid):   

    
def on_subscribe(client, userdata, mid, granted_qos): 
    # Sau khi subscribe de co the nhan data phan hoi tu server, 
              
    client.publish(pub_topic,payload, qos=2, retain = False)                                          # publish packet dau tien > cac packet sau(payload can gui len) tu dong pub 
    #print("\r\n****Former Message Published****")
    #print("This is the next message to send: ")
    print("Topic: " + pub_topic)
    print("Payload: " + payload)
    #print("SENT") 

        #   sau khi packet trc do da pub thanh cong
                                                                                     # packet dau tien co the tuy chinh thanh bat ki thong tin gi, 
                                                                                     #   muc dich chi de pub payload, co the lap trinh server bo qua packet dau tien nay
client.on_connect = on_connect
client.on_disconnect = on_disconnect
client.on_message = on_message
#client.on_publish = on_publish
client.on_subscribe = on_subscribe
#client.connect(host, port)

#try:
#    client.connect(host,port) #connect to broker
    
#except:
#    print("Connection Failed")
#    exit(1) #Should quit or raise flag to quit or retry
#client.loop_start()
i = 0

# Here is my code :)
def serial_write_data(get_frame=[], *args):
    len_data = len(get_frame)
    # print(len_data)
    frame = struct.pack('B'*len_data,*get_frame)
    encoded_frame = afproto_frame_data(frame)
    ser.write(encoded_frame)
def serial_data_handler(frame=[], *args):
    global example
    global payload
    global pub_topic
    example = frame
    print(type(frame[2]))
    print(type(frame))
    print(frame)
    pub_topic = GetTopic(example)
    print(pub_topic)
    payload = GetMessage(pub_topic, example)
    print(payload)
    client.connect(host,port)
    client.loop_forever()
    
    
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
        data_convert=[]
        if (data_decode != None):
            print("crc matched and jump into handler")
            leng = len(data_decode)
            for i in range (leng) :
                data_convert.append(ord(data_decode[i]))
            serial_data_handler(data_convert)
        
ser = serial.Serial(port='/dev/ttyS0',
               baudrate = 115200,
               parity=serial.PARITY_NONE,
               stopbits=serial.STOPBITS_ONE,
               bytesize=serial.EIGHTBITS,
               timeout=1)
print("Serial Initial Successful")

serial_data_thread = threading.Thread(target=serial_read_data)
serial_data_thread.start()

# End of my code :)


while True:

    # payload= [0x05,0x88,0x03,0x22,0x33,0x56,0x68,0x69,0x82]
    # serial_write_data(payload)
    time.sleep(1)
