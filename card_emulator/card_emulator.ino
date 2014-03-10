//Magnetic Card emulator (track 2 only)
//Connect an H-Bridge to the left and right pins defined below
//Connect the H-Bridge outputs to a coil wrapped around a ferrous strip thin enough to insert into a card reader.
int leftPin = 2;
int rightPin = 3;
int ledPin = 13;
int clockSpeed = 1000;
int clockHalf = 0;
int replayDelay = 2000;
char track2[255];


//Track 2 data
//Up to 40 characters (numeric) including SS, ES and LRC.
String data = ";2813308004?";

void setup() {
  pinMode(leftPin, OUTPUT);
  pinMode(rightPin, OUTPUT);
  pinMode(ledPin, OUTPUT); 
 
  for (int i = 0; i < data.length(); i++) {
    track2[i] = data[i];
  }
}

void writeLow() {
  digitalWrite(leftPin, LOW);
  digitalWrite(rightPin, LOW);
}

void writeBit(int lowOrHigh) {
  if(lowOrHigh == 1) {
    digitalWrite(rightPin, ((clockHalf == 0)?LOW:HIGH));
    digitalWrite(leftPin, ((clockHalf == 0)?HIGH:LOW));
    delayMicroseconds(clockSpeed);
    digitalWrite(rightPin, ((clockHalf == 0)?HIGH:LOW));
    digitalWrite(leftPin, ((clockHalf == 0)?LOW:HIGH));
    delayMicroseconds(clockSpeed);
  } else {
    digitalWrite(rightPin, ((clockHalf == 0)?LOW:HIGH));
    digitalWrite(leftPin, ((clockHalf == 0)?HIGH:LOW));
    delayMicroseconds(clockSpeed * 2);
    clockHalf = (clockHalf+1)%2;
  }
}

void writeChar(char c) {
  byte t = 0x00;
  switch (c) {
    case '0': t = 0x01; break;
    case '1': t = 0x10; break;
    case '2': t = 0x08; break;
    case '3': t = 0x19; break;
    case '4': t = 0x04; break;
    case '5': t = 0x15; break;
    case '6': t = 0x0d; break;
    case '7': t = 0x1c; break;
    case '8': t = 0x02; break;
    case '9': t = 0x13; break;
    case ':': t = 0x0b; break;
    case ';': t = 0x1a; break;
    case '<': t = 0x07; break;
    case '=': t = 0x16; break;
    case '>': t = 0x0e; break;
    case '?': t = 0x1f; break;
  }
  for (int i = 4; i >= 0; i--) {
    writeBit((t>>i)&0x01);
  }
}

int LRC(String data, int length) {
  int lrc = 0;
  for (int i = 0; i < length; i++) {
    lrc ^= (data[i]-0x30);
  }
  return lrc+0x30;
}

void loop(){
  //Turn on active led
  digitalWrite(ledPin, HIGH);
  for(int i = 0; i < 20; i ++) {
    writeBit(0);
  }
  for (int i = 0; i < data.length(); i++) { 
    writeChar(data[i]);
  }
  writeChar(LRC(data, data.length()));
  for(int i = 0; i < 20; i ++) {
    writeBit(0);
  }
  writeLow();
  
  //Turn off active led
  digitalWrite(ledPin, LOW);
  
  delay(replayDelay);
}
