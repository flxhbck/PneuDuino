/*************************************************** 

 ****************************************************/

#include <PneuDuino.h>
#include <Wire.h>
#include "avr/interrupt.h"

#if defined(__AVR__)
 #define WIRE Wire
#elif defined(CORE_TEENSY) // Teensy boards
 #define WIRE Wire
#else // Arduino Due
 #define WIRE Wire1
#endif

// Set to true to print some debug messages, or false to disable them.
#define DEBUG false


PneuDuino::PneuDuino()
{
}

void PneuDuino::begin(void)
{
  pinMode(LED, OUTPUT);
  pinMode(CONNECT_BTN, INPUT_PULLUP);

  leds.begin();  // initialize LED driver
  leds.setPWMFreq(1600);  // maximum PWM frequency
  uint8_t twbrbackup = TWBR;

  Wire.begin(); // join the I2C Bus as Master
}

void PneuDuino::update()
{
  while(!foundNode){
    digitalWrite(LED, HIGH);
    checkAddressSpace();
    delay(500);
  }


  if ( !digitalRead(CONNECT_BTN) ){
    digitalWrite(LED, HIGH);
    reset();
    checkAddressSpace();
    delay(500);
  }

  if(foundNode) digitalWrite(LED, LOW);

  checkNodes(); // request data from all available nodes
}

void PneuDuino::in(int address, int side)
{
  if (nodes[address] != 1) return;
  byte b;
  if (side == LEFT)  b = 'L';
  if (side == RIGHT)  b = 'R';
  if (side == BOTH) b = 'B';

  Wire.beginTransmission(address);
  Wire.write(b);
  Wire.endTransmission();
}

void PneuDuino::out(int address, int side)
{
  if (nodes[address] != 1) return;

  byte b;
  if (side == LEFT)  b = 'K';
  if (side == RIGHT)  b = 'E';
  if (side == BOTH) b = 'N';


  Wire.beginTransmission(address);
  Wire.write(b);
  Wire.endTransmission();
}



int PneuDuino::readPressure(int address)
{
  if (nodes[address] == 0) return -1;
  //if (getState(address) == "HOLD") return pressureHACK;
  return pressure[address];
}

bool PneuDuino::readButton(int number)
{
  if (nodes[12] == 0) return 0;
  if (number == 0) return !io_btn2;
  if (number == 1) return !io_btn1;
  return 0;
}

int PneuDuino::readPot()
{
  if (nodes[12] == 0) return -1;
  return io_pot;
}

void PneuDuino::setLED(int pos, int value)
{  
  value = constrain(value, 0, 4095);
  pos--;
  pos = constrain(pos, 0, 11);
  leds.setPWM(LEDmap[pos], 0, value);
}

void PneuDuino::setAllLEDs(int value)
{
  for(int i = 1; i <= 12; i++){
    leds.setPWM(LEDmap[i], 0, value);
  }
}

int PneuDuino::getNodeAmount()
{
  int amount = 0;
  for(int i = 1; i <= 12; i++){
    if (nodes[i] == 1) amount++;
  }
  return amount;
}


/***************
     PRIVATE
 ***************/

void PneuDuino::checkNodes(void)
{
  for (int i = 1; i < 12; i++)
  {
    if (nodes[i] == 1){
      // this is where to distinguish between IO and valve. send different requests accordingly.
      Wire.requestFrom(i, 2);
      while (Wire.available())
      { 
        byte lowByte = Wire.read();
        // Serial.print(lowByte);
        byte highByte = Wire.read();
        // Serial.print(" - ");
        // Serial.println(highByte);
        pressure[i] = int( (highByte << 8) + lowByte );
      }
    }
  }

  // check for 1 allowed IO board
  if ( nodes[12] == 1 ) {
    Wire.requestFrom(12, 1);
    while (Wire.available())
    {
      byte inByte = byte( Wire.read() );
      io_btn1 = bitRead(inByte, 0);
      io_btn2 = bitRead(inByte, 1);
      io_pot = int( inByte >> 2 );
    }
  } 
}


void PneuDuino::checkAddressSpace()
{
  if (DEBUG) Serial.println("Check for devices");

  // reset foundNode
  foundNode = false;
  
  for (int address = 1; address <= 12; address++)
  {
    Wire.requestFrom(byte(address), 1);
    
    if (!Wire.available())
    {
      setLED(address, 0);
      nodes[address] = 0;
      
      if (DEBUG) Serial.print(address);
      if (DEBUG) Serial.println(" : nope.");
    }
    else
    {
      while (Wire.available())
      {
        foundNode = true;

        int temp = int( Wire.read() );
        setLED(address, 4095);
        nodes[address] = 1;
        
        if (DEBUG) Serial.print(address);
        if (DEBUG) Serial.println(" yup");
      }
    }
  }
  if (foundNode) digitalWrite(LED, LOW);
}

void PneuDuino::reset()
{
  for (int i = 0; i <= 12; i++)
  {
    foundNode = false;
    nodes[i] = 0;
    pressure[i] = 0;
    setAllLEDs(0);
  }

  io_btn1 = false;
  io_btn2 = false;
  io_pot = -1;
}




/****************************
 * THINGS TO BE IMPLEMENTED *
 ****************************/
/*

bool PneuDuino::checkTouch(int address)
{
  if (nodes[address] == 0) return false;
  return touched[address];
}

 String PneuDuino::getState(int address)
{
  if (nodes[address] == 0) return "X";
  if (states[address] == 'X') return "NC";
  if (states[address] == 'I') return "INFLATE";
  if (states[address] == 'D') return "DEFLATE";
  if (states[address] == 'H') return "HOLD";
  if (states[address] == 'R') return "OPEN";
  return "ERROR";
}
*/

/*
void PneuDuino::hold(int address, int side)
{
  if (nodes[address] != 1) return;

  byte b = 'H';
  Wire.beginTransmission(address);
  Wire.write(b);
  Wire.endTransmission();
}

void PneuDuino::clear(int address, int side)
{
  if (nodes[address] != 1) return;

  byte b = 'R';
  Wire.beginTransmission(address);
  Wire.write(b);
  Wire.endTransmission();
}
*/
/*
void PneuDuino::recalibrate(int address)
{
  if (nodes[address] != 1) return;

  byte b = 'C';
  Wire.beginTransmission(address);
  Wire.write(b);
  Wire.endTransmission();
}


void PneuDuino::inflate(int address, int pressure)
{
  if (nodes[address] != 1) return;
  //pressureHACK = pressure;

  byte b = 'P';
  byte p = byte(pressure);
  Wire.beginTransmission(address);
  Wire.write(b);
  Wire.write(p);
  Wire.endTransmission();
}
*/

/*
PneuDuino::reset()
{

}
*/
