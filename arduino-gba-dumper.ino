int RD = 1;
int WR = 4;
int CS = 2;
int CS2 = 3;

int DS = 10;
int CLOCK = 11;
int LATCH = 12;

void setup() {
  Serial.begin(9600);
  pinMode(DS, OUTPUT);
  pinMode(CLOCK, OUTPUT); 
  pinMode(LATCH, OUTPUT); 

  pinMode(RD, OUTPUT);
  pinMode(CS, OUTPUT);
  digitalWrite(RD, HIGH);
  digitalWrite(WR, HIGH);
  digitalWrite(CS, LOW);
  digitalWrite(CS2, HIGH);
}

void loop() {
}
