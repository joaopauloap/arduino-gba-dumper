#define RAM_SIZE 128  //for 128kb ram

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

void latchAddress(unsigned long addr) {
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

void dumpSave(int size) {
  // Serial.println("Dumping GBA Save...");
  unsigned long lastAddr = (unsigned long) size * 1024;
  digitalWrite(CS, HIGH);  //Disable ROM
  digitalWrite(CS2, LOW);
  delay(100);

  for (unsigned long addr = 0; addr < lastAddr; addr++) {
    latchAddress(addr);
    digitalWrite(RD, LOW);
    delayMicroseconds(1);
    digitalWrite(RD, HIGH);
    int b = readDataBus();
    Serial.write(b);
  }
  digitalWrite(CS2, HIGH);
}



void loop() {
  if (Serial.available() > 0) {
    char op = Serial.read();

    //Dump save option
    if (op == '2') {
      dumpSave(128);
    }
  }
}