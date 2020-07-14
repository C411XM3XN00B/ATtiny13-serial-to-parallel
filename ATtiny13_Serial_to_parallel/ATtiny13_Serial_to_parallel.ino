/*
  PB0 and PB1 are serial I/O
  PB2 = LATCH, PB3 = CLOCK, PB4 = DATA
  Expected data format on serial input: "B00000000" --> starts with b to indicate start of transmission, followed directly by the 8 different binary states

  ATtiny13 Clock vs baud rate
  
  (External) 20 MHz   115200
  (External) 16 MHz   115200
  (External) 12 MHz   115200
  (External) 8 MHz    115200
  (External) 1 MHz    19200
  (Internal) 9.6 MHz  115200
  (Internal) 4.8 MHz  57600
  (Internal) 1.2 MHz  19200
  (Internal) 600 kHz  9600
*/
int raw = 0;

bool bitvalue = 0;
bool serialbyte[9] = {0, 0, 0, 0, 0, 0, 0, 0};

void setup() {
  delay(500);
  DDRB |= 0b00011100;                     //sets pins 2(Latch), 3(Clock) and 4(Data) as outputs without altering states of other pins
  Serial.begin(115200);                   //baud rate set automatically by MicroCore board library, otherwise ignored.
}

void loop() {
  while (Serial.read() == 'B') {             //waits for 'B' as indicator for the start of a transmission
    for (int i = 0; i < 8; i++) {         //reads the serial input and puts 8bits in the serialbyte array
      delay(1);
      raw = Serial.read() - 48;
      if (raw == 1) {
        bitvalue = 1;  
      }
      else if (raw == 0) {
        bitvalue = 0;  
      } 
      else {                              //will automatically set bits to zero if not enough digits were sent
        bitvalue = serialbyte[i];
      }
      serialbyte[i] = bitvalue;
    } 

    
    Serial.print("received Byte: B");              //optional, sends the received binary values back to host
    for (int i = 0; i < 8; i++) {
      Serial.print(serialbyte[i]);
    }
    Serial.println();

    
    for (int i = 0; i < 8; i++) {
       digitalWrite(4, serialbyte[i]);    //pulls data line high or low
       digitalWrite(3, 1);                //pulls clock high
       digitalWrite(4, 0);                //pulls data line low
       digitalWrite(3, 0);                //pulls clock low
    }
    digitalWrite(2, 1);                   //pulses latch line
    digitalWrite(2, 0);
    //PORTB |= (1 << PORTB2);
    //PORTB &= ~(1 << PORTB2);
  }
}
