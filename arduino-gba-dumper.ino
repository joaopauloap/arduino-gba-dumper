//cartridge pins
int RD = 4;   //control bit
int CS2 = 2;  //switch ROM/RAM
int CS = 3;   //switch Address/Data
//int WR = 5;   //Write strobe

//shift register 74HC595 pins
int CLOCK = A0;
int LATCH = A1;
int DATA = A2;

void setup() {
  Serial.begin(9600);
  pinMode(CLOCK, OUTPUT);
  pinMode(LATCH, OUTPUT);
  pinMode(DATA, OUTPUT);
  pinMode(RD, OUTPUT);
  pinMode(CS, OUTPUT);
  pinMode(CS2, OUTPUT);
  //pinMode(WR, OUTPUT);

  //Data bus
  for (int pin = 5; pin <= 12; pin++) {
    pinMode(pin, INPUT_PULLUP);
  }

  digitalWrite(RD, HIGH);
  digitalWrite(CS, HIGH);
  digitalWrite(CS2, HIGH);
  //digitalWrite(WR, HIGH);
}

void latchRAMAddress(unsigned int addr) {
  digitalWrite(LATCH, LOW);
  shiftOut(DATA, CLOCK, MSBFIRST, addr >> 8);
  shiftOut(DATA, CLOCK, MSBFIRST, addr);
  digitalWrite(LATCH, HIGH);
}



unsigned int readRAMDataBus() {
  uint8_t portd_bits = ~PIND & 0b11100000;
  portd_bits >>= 5;
  uint8_t portb_bits = ~PINB & 0b00011111;
  return (portb_bits << 3) | portd_bits;
}

void printByte(int value) {
  // char buffer[3];
  // sprintf(buffer, "%02X", value);
  // Serial.print(buffer);
  // Serial.print(" ");
  Serial.write(value);
}


void dumpSave() {
  // Serial.println("Dumping GBA Save...");
  digitalWrite(CS2, LOW);
  delay(100);

  for (unsigned int addr = 0x00; addr < 0xFFFF; addr++) {

    latchRAMAddress(addr);
    digitalWrite(RD, LOW);
    delayMicroseconds(10);
    digitalWrite(RD, HIGH);

    int b = readRAMDataBus();

    if (addr % 32 == 0) {
      Serial.println("");
    }
    printByte(b);
  }

  digitalWrite(CS2, HIGH);
}



void loop() {
  while (Serial.available() == 0)
    ;
  char op = Serial.read();
  if (op == '2') {
    dumpSave();
    // Serial.println("END OF DUMP");
  }
}
