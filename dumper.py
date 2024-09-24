import serial #requires pySerial
import serial.tools.list_ports
import time

baudrate = 9600
timeout = 1 
dumpTimeout = 2 

def list_serial_ports():
    ports = serial.tools.list_ports.comports()
    available_ports = [port.device for port in ports]
    if available_ports:
        print("Available Serial Ports:")
        for i, port in enumerate(available_ports):
            print(f"{i}: {port}")
        return available_ports
    else:
        print("No serial ports found.")
        return []

def select_serial_port(available_ports):
    while True:
        try:
            index = int(input("Select port by index: "))
            if 0 <= index < len(available_ports):
                return available_ports[index]
            else:
                print("Invalid index, try again.")
        except ValueError:
            print("Please enter a valid number.")

ports = list_serial_ports()
if not ports:
    exit()

port = select_serial_port(ports)

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
        fileName = 'dump.sav'
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

# Loop principal
while True:
    print(f"\n=== GBA Game Dumper ({port}) ===")
    print("0 - Print game title")
    print("1 - Dump ROM")
    print("2 - Dump RAM")
    print("3 - Write RAM")
    print("4 - Exit")
    op = input("\n> ")
    
    if op == '0':
        #print_game_title()
        print('wip')
    elif op == '1':
        dump_rom()
    elif op == '2':
        dump_ram()
    elif op == '3':
        print('wip')
    elif op == '4':
        break
    else:
        print("Invalid option.")
