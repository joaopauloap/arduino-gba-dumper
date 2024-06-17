import serial
import time

port = 'COM3' 
baudrate = 9600
timeout = 1 
dumpTimeout = 2 

# Conexão serial
try:
    ser = serial.Serial(port, baudrate, timeout=timeout)
except serial.SerialException as e:
    print(f'Error opening port {port}: {e}')
    exit()


def receiveDump(isCreateFile):
    data = b''
    timeout_counter = time.time()

    if isCreateFile:
        print('Dumping, please await...')
        fileName = 'dump.txt'
        file = open(fileName, 'wb')

    try:
        while True:
            byte = ser.read()
            print(byte.hex())
            if byte:
                if isCreateFile:
                    file.write(byte)  
                else:
                    data += byte  
                timeout_counter = time.time()
            else:
                if time.time() - timeout_counter > dumpTimeout:
                    break
    except serial.SerialException as e:
        print(f'Error: {e}')
    finally:
        if isCreateFile:
            file.close() 
            print(f'Done. Saved at "{fileName}".')
        else:
            print(data.decode())


def print_game_title():
    try:
        print('Getting game title...')
        ser.write(b'0')
    except serial.SerialException as e:
        print(f'Error: {e}')
        ser.close()
        exit()
    receiveDump(False)

def dump_rom():
    try:
        ser.write(b'1')
    except serial.SerialException as e:
        print(f'Error: {e}')
        ser.close()
        exit()
    receiveDump(True)

def dump_ram():
    try:
        ser.write(b'2')
    except serial.SerialException as e:
        print(f'Error: {e}')
        ser.close()
        exit()
    receiveDump(True)

def write_ram():
    print("WIP")

while True:
    print(f"\n=== GBA Game Dumper ({port}) ===")
    print("0 - Print game title")
    print("1 - Dump ROM")
    print("2 - Dump RAM")
    print("3 - Write RAM")
    print("4 - Exit")
    op = input("\n>")
    
    if op == '0':
        dumpTimeout = 2
        #print_game_title()
    elif op == '1':
        #dump_rom()
        print("")
    elif op == '2':
        dumpTimeout = 2   
        dump_ram()
    elif op == '3':
       #write_ram()
       print("")
    elif op == '4':
        break
    else:
        print("Invalid op.")