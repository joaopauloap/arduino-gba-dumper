//cartridge pins
int WR = A3;   //Write strobe
int RD = A2;   //control bit
int CS = A1;   //switch Address/Data for ROM
int CS2 = A0;  //switch ROM/RAM

//shift register 74HC595 pins
int CLOCK = 10;
int DATA = 11;
int LATCH = 12;

void setup() {
  Serial.begin(9600);
  pinMode(RD, OUTPUT);
  pinMode(CS, OUTPUT);
  pinMode(CS2, OUTPUT);
  pinMode(WR, OUTPUT);
  pinMode(CLOCK, OUTPUT);
  pinMode(LATCH, OUTPUT);
  pinMode(DATA, OUTPUT);

  //Data bus
  for (int pin = 2; pin <= 9; pin++) {
    pinMode(pin, INPUT_PULLUP);
  }

  digitalWrite(RD, HIGH);
  digitalWrite(CS, HIGH);
  digitalWrite(CS2, HIGH);
  digitalWrite(WR, HIGH);
}

void latchAddress(unsigned int addr) {
  digitalWrite(LATCH, LOW);
  shiftOut(DATA, CLOCK, MSBFIRST, (addr >> 8));
  shiftOut(DATA, CLOCK, MSBFIRST, addr);
  digitalWrite(LATCH, HIGH);
}


unsigned int readDataBus() {
  return ((PINB & 0b00000011) << 6) | ((PIND & 0b11111100) >> 2);
}

void printByte(int value) {
  char buffer[3];
  sprintf(buffer, "%02X", value);
  Serial.print(buffer);
  Serial.print(" ");
}

void dumpSave() {
  // Serial.println("Dumping GBA Save...");
  //digitalWrite(CS, HIGH);  //Disable ROM
  digitalWrite(CS2, LOW);
  delay(100);

  for (unsigned int addr = 0x00; addr < 0x1FFF; addr++) {
    latchAddress(addr);
    digitalWrite(RD, LOW);
    delayMicroseconds(10);
    digitalWrite(RD, HIGH);
    int b = readDataBus();
    Serial.write(b);
  }
  digitalWrite(CS2, HIGH);
}



void loop() {
  char op = Serial.read();
  if (op == '2') {
    dumpSave();
    // Serial.println("END OF DUMP");
  }
}
