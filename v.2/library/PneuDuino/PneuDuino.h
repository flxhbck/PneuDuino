/*************************************************** 
  This is a library for the PneuDuino Modules.
  It facilitates easy control of inflatable actuators and interactive pneumatic systems.

  It includes the Adafruit 16-channel PWM & Servo driver
 ****************************************************/

#ifndef _PneuDuino_h
#define _PneuDuino_h

#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

#include <Adafruit_PWMServoDriver.h>
#include "Arduino.h"

#define LED 13
#define CONNECT_BTN 0

#define LEFT 1
#define RIGHT 0
#define BOTH 2

class PneuDuino {
 public:
  PneuDuino();
  void begin(void);
//void reset(void);
  void update(void);
  void setLED(int pos, int value);
  void setAllLEDs(int value);

  void in(int address, int side);
  void out(int address, int side);
  int readPressure(int address);
  bool readButton(int number);
  int readPot();

  void reset(void);

  int getNodeAmount();

/* 
 * TO BE IMPLEMENTED:

  void hold(int address, int side);      
  void clear(int address, int side);
  void inflate(int address, int pressure);
  bool checkTouch(int address);
  String getState(int address);
  void recalibrate(int address);
*/



 private:
  Adafruit_PWMServoDriver leds;
  int LEDmap[12] = {8, 9, 10, 11, 12, 1, 2, 3, 4, 5, 6, 7};
  int nodes[13] =  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  int pressure[13] =  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

  bool touched[13] =  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  char states[13] = {'X', 'X', 'X', 'X', 'X', 'X', 'X', 'X', 'X', 'X', 'X', 'X', 'X'} ;

  void checkNodes(void);
  void checkAddressSpace(void);
  bool foundNode = false;

  bool io_btn1 = false;
  bool io_btn2 = false;
  int io_pot = -1;

 };

#endif
