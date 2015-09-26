#include <Wire.h>

#define STAT_R 4
#define STAT_G 10
#define STAT_B 13

#define RIGHT 7
#define LEFT 8

#define PRESSURE A0
#define LOW_END 239

#define DEBUG 0

#define ADDRESS 10

bool madeConnection = false;
char lastState = 'N';

int sensorArray[4] = {5, 6, 9, 3};

void setup()
{
  if (DEBUG) Serial.begin(9600);
  if (DEBUG) madeConnection = true;
  
  pinMode(STAT_R,  OUTPUT);
  pinMode(STAT_G, OUTPUT);
  pinMode(STAT_B, OUTPUT);
  pinMode(RIGHT, OUTPUT);
  pinMode(LEFT, OUTPUT);

  for (int i = 0; i < 4; i++){
    pinMode(sensorArray[i], OUTPUT);
    digitalWrite(sensorArray[i], LOW);
  }
  
  
  Wire.begin(ADDRESS);               // join i2c bus with address #
  Wire.onReceive(receiveEvent); // register event
  Wire.onRequest(requestEvent);
}

void loop()
{
  if (!madeConnection){
    showStatus(255,0,0);
  } else {
    showStatus(0,255,0);
    showPressure(analogRead(PRESSURE));
  }
    
   delay(100);
}
  

void changeState(char newState)
{
  if (newState == lastState) return;
  lastState = newState;
}

void left(bool state)
{
  leftValve(state);
}

void right(bool state)
{
  rightValve(state);
}

void both(bool state)
{
  leftValve(state);
  rightValve(state);
}


void leftValve(bool state){
  digitalWrite(LEFT, state);
}

void rightValve(bool state){
  digitalWrite(RIGHT, state);
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

void showPressure(int pressure)
{ 
  pressure = map(pressure, LOW_END, 1023, 0, 1023);
  int mapped;
  for (int i = 0; i < 4; i++){
    mapped = map(pressure, i*(1023/4), (i+1)*(1023/4), 0, 255);
    mapped = constrain(mapped, 0, 255);
    analogWrite(sensorArray[i], mapped);
  }
}





/*******************************************
 * 
 * 
 *        I2C Communication Stuff
 * 
 * 
 *******************************************/



// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void receiveEvent(int howMany)
{ 
  /*
  while(1 < Wire.available()) // loop through all but the last
  {
    char c = Wire.read(); // receive byte as a character
    //Serial.print(c);         // print the character
  }
  */
  
  char x = Wire.read();    // receive byte as an integer
  
  if (x == 'L') left(true);
  if (x == 'K') left(false);
  if (x == 'R') right(true);
  if (x == 'E') right(false);
  if (x == 'B') both(true);
  if (x == 'N') both(false);
  
  //holdPressure( byte(x) );
    
}


void requestEvent()
{
  madeConnection = true;
  
  byte package;
  package = analogRead(PRESSURE);
    
  Wire.write(package);
}
