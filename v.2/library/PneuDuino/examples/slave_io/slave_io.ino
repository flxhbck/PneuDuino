/**********************************************************************************

    ____  ____     ___  __ __  ___    __ __  ____  ____    ___  
   |    \|    \   /  _]|  |  ||   \  |  |  ||    ||    \  /   \ 
   |  o  )  _  | /  [_ |  |  ||    \ |  |  | |  | |  _  ||     |
   |   _/|  |  ||    _]|  |  ||  D  ||  |  | |  | |  |  ||  O  |
   |  |  |  |  ||   [_ |  :  ||     ||  :  | |  | |  |  ||     |
   |  |  |  |  ||     ||     ||     ||     | |  | |  |  ||     |
   |__|  |__|__||_____| \__,_||_____| \__,_||____||__|__| \___/ 

   PneuDuino is a toolkit for controlling inflatable actuators
   and prototyping interactions with shape-changing systems.

   This is the code that runs on each pneumatic control board.
   It is here for reference and future improvements of functionality.
   DON'T TRY TO USE THIS UNLESS YOU KNOW WHAT YOU ARE DOING!!
   (sorry for screaming)

**********************************************************************************/

#include <Wire.h>

#define STAT_R 4
#define STAT_G 10
#define STAT_B 13

#define POT A0
#define BTN1 2
#define BTN2 3

#define DEBUG 0
#define ADDRESS 12  // only IO boards can be 12

bool madeConnection = false;
char lastState = 'N';


void setup()
{
  if (DEBUG) Serial.begin(9600);
  if (DEBUG) madeConnection = true;
  
  pinMode(STAT_R,  OUTPUT);
  pinMode(STAT_G, OUTPUT);
  pinMode(STAT_B, OUTPUT);
  
  pinMode(BTN1, INPUT);
  pinMode(BTN2, INPUT);
  
  Wire.begin(ADDRESS);               // join i2c bus with address #
  Wire.onRequest(requestEvent);
}

void loop()
{
  if (!madeConnection){
    showStatus(255,0,0);
  } else {
    showStatus(0,255,0);
  }
    
   delay(100);
}
  



/*******************************************
 * 
 * 
 *        Indicator functions
 * 
 * 
 *******************************************/


void showStatus(bool r, bool g, bool b)
{
  digitalWrite(STAT_R, !r);
  digitalWrite(STAT_G, !g);
  digitalWrite(STAT_B, !b);
}




/*******************************************
 * 
 * 
 *        I2C Communication Stuff
 * 
 * 
 *******************************************/


void requestEvent()
{
  madeConnection = true;
  
  byte package = (byte) map(analogRead(POT), 0, 1023, 63, 0);;
  package << 2;
  bitWrite(package, 1, digitalRead(BTN1));
  bitWrite(package, 0, digitalRead(BTN2));
    
  Wire.write(package);
}