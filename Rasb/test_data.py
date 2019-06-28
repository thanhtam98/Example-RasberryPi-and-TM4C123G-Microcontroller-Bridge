import json
# len   id  cmd   ft  cmd_num     data

error_example =             [0x09, 0x01, 0x01, 0x01, 0x00, 0x00, 0x02]  # Bottle is removed without RFID
event_example =             [0x09, 0x02, 0x02, 0x01, 0x00, 0x00, 0x05, 0x04, 0x69, 0xAB, 0x69, 0xED]  # Bottle removed
card_example =              [0x13, 0x93, 0x03, 0x01, 0x00, 0x00, 0x04, 0xFE, 0xA0, 0xB2, 0xC7]   # 4 bytes UID = FE A0 B2 C7
barcode_example =           [ 0x13, 0x94, 0x04, 0x01, 0x00, 0x00, 0x04, 0x15, 0x12, 0x65, 0xae]  # 4 bytes barcode = E9 5F D2 1C
dispensing_example =        [ 0x13, 0x95, 0x05, 0x01, 0x00, 0x00, 0x04, 0x50, 0x4E, 0x13, 0x7F]  # 4 bytes RFID = 50 4E 13 7F
synchronize_example =       [ 0x08, 0x96, 0x06, 0x01, 0x00, 0x00]
updatestatus_example =      [ 0x12, 0x97, 0x07, 0x01, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF]
checkconnection_example =   [ 0x08, 0x99, 0x09, 0x01, 0x00, 0x00]
def GetChildTopic(DataFrame):
    if DataFrame[2] == 0x01:
        return "Error"
    elif DataFrame[2] == 0x02:
        return "Event"
    elif DataFrame[2] == 0x03:
        return "Card"
    elif DataFrame[2] == 0x04:
        return "Barcode"
    elif DataFrame[2] == 0x05:
        return "DispensingSpirit"
    elif DataFrame[2] == 0x06:
        return "Synchronize"
    elif DataFrame[2] == 0x07:
        return "UpdateStatus"
    #elif DataFrame[3] == 0x08:         # TO DO, NOT THIS VERSION
    #    return "LockBottle"
    else:   # DataFrame[3] == 0x09:
        return "CheckConnection"

def GetTopic(DataFrame):
    if DataFrame[3] == 0x01:
        topic = "Request/" + GetChildTopic(DataFrame)
    else:
        topic = "Ping"
    return topic

def GetMessage(topic, DataFrame):
    ID = DataFrame[1]
    CmdNumber = (DataFrame[4]<<8)|(DataFrame[5])
    message = {
        "ID": ID,
        "CmdNumber": CmdNumber
        }
    if topic == "Request/Error":
        message["ErrorCode"] = DataFrame[6]
    elif topic == "Request/Event":       
        message["EventCode"] = DataFrame[6] 
        UIDlength = DataFrame[7]
        if UIDlength == 4:
            UIDbytes = (DataFrame[8]<<24)|(DataFrame[9]<<16)|(DataFrame[10]<<8)|(DataFrame[11])
        elif UIDlength == 7:
            UIDbytes = (DataFrame[8]<<48)|(DataFrame[9]<<40)|(DataFrame[10]<<32)|\
            (DataFrame[11]<<24)|(DataFrame[12]<<16)|(DataFrame[13]<<8)|(DataFrame[14])
        else:
            UIDbytes = (DataFrame[8]<<72)|(DataFrame[9]<<64)|(DataFrame[10]<<56)|\
            (DataFrame[11]<<48)|(DataFrame[12]<<40)|(DataFrame[13]<<32)|(DataFrame[14]<<24)|(DataFrame[15]<<16)|\
            (DataFrame[16]<<8)|(DataFrame[17])
        message["UID"] = hex(UIDbytes)
    elif topic == "Request/Card":
        UIDlength = DataFrame[6]
        if UIDlength == 4:
            UIDbytes = (DataFrame[7]<<24)|(DataFrame[8]<<16)|(DataFrame[9]<<8)|(DataFrame[10])
        elif UIDlength == 7:
            UIDbytes = (DataFrame[7]<<48)|(DataFrame[8]<<40)|(DataFrame[9]<<32)|\
            (DataFrame[10]<<24)|(DataFrame[11]<<16)|(DataFrame[12]<<8)|(DataFrame[13])
        else:
            UIDbytes = (DataFrame[7]<<72)|(DataFrame[8]<<64)|(DataFrame[9]<<56)|\
            (DataFrame[10]<<48)|(DataFrame[11]<<40)|(DataFrame[12]<<32)|(DataFrame[13]<<24)|(DataFrame[14]<<16)|\
            (DataFrame[15]<<8)|(DataFrame[16])
        message["UID"] = hex(UIDbytes)
    elif topic == "Request/Barcode":
        Barcodelength = DataFrame[6]
        if Barcodelength == 4:
            Barcode = (DataFrame[7]<<24)|(DataFrame[8]<<16)|(DataFrame[9]<<8)|(DataFrame[10])
        elif Barcodelength == 7:
            Barcode = (DataFrame[7]<<48)|(DataFrame[8]<<40)|(DataFrame[9]<<32)|\
            (DataFrame[10]<<24)|(DataFrame[11]<<16)|(DataFrame[12]<<8)|(DataFrame[13])
        else:
            Barcode = (DataFrame[7]<<72)|(DataFrame[8]<<64)|(DataFrame[9]<<56)|\
            (DataFrame[10]<<48)|(DataFrame[11]<<40)|(DataFrame[12]<<32)|(DataFrame[13]<<24)|(DataFrame[14]<<16)|\
            (DataFrame[15]<<8)|(DataFrame[16])
        message["Barcode"] = hex(Barcode)
    elif topic == "Request/DispensingSpirit":
        RFIDlength = DataFrame[6]
        if RFIDlength == 4:
            RFID = (DataFrame[7]<<24)|(DataFrame[8]<<16)|(DataFrame[9]<<8)|(DataFrame[10])
        elif RFIDlength == 7:
            RFID = (DataFrame[7]<<48)|(DataFrame[8]<<40)|(DataFrame[9]<<32)|\
            (DataFrame[10]<<24)|(DataFrame[11]<<16)|(DataFrame[12]<<8)|(DataFrame[13])
        else:
            RFID = (DataFrame[7]<<72)|(DataFrame[8]<<64)|(DataFrame[9]<<56)|\
            (DataFrame[10]<<48)|(DataFrame[11]<<40)|(DataFrame[12]<<32)|(DataFrame[13]<<24)|(DataFrame[14]<<16)|\
            (DataFrame[15]<<8)|(DataFrame[16])
        message["RFID"] = hex(RFID)
    elif topic == "Request/Synchronize":
        message = message
    elif topic == "Request/UpdateStatus":
        message["RFIDReaderStatus"] = DataFrame[6]
        message["BarcodeReaderStatus"] = DataFrame[7]
        message["CupStatus"] = DataFrame[8]
        message["BottlePresenceStatus"] = DataFrame[9]
    else: # topic == "Request/CheckConnection":
        message = message
    return json.dumps(message)
def process(topic_in,message):
    msg = json.loads(message)
    print("ID: "+msg["ID"])
    print("CmdNumber: "+msg["CmdNumber"])
    if topic_in == "Response/Card":
        print("AuthenticationStatus: "+msg["AuthenticationStatus"])
        print("CardHolderName: "+msg["CardHolderName"])
        print("CardHolderRole: "+msg["CardHolderRole"])
    elif topic_in == "Response/Barcode":
        print("AuthenticationStatus: "+msg["AuthenticationStatus"])
        print("SpiritName: "+msg["SpiritName"])
        print("RemainingShot: "+msg["RemainingShot"])
    elif topic_in == "Response/DispensingSpirit":
        print("SessionTotalShot: "+msg["SessionTotalShot"])
        print("RemainingShot: "+msg["RemainingShot"])
    elif topic_in == "Response/Synchronize":
        print("DispenserID: "+msg["DispenserID"])
        print("SpiritName: "+msg["SpiritName"])
        print("RemainingShot: "+msg["RemainingShot"])







# JUST FOR TESTING





