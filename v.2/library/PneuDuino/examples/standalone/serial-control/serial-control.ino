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

   Every slave board has a RGB led on the top right corner that communicates
   the state of this board.

   GREEN - baord running and connecting
   RED - board running, but not connected
   BLUE - board not running yet (usually after reset or power) or broken :(

   The boards addresses range from 1-12. 12 is reserved for IO Boards.
   There can ONLY BE ONE IO BOARD per system.

   The library offers functions that allow for controlling the valves and 
   accessing the pressure sensor :

   begin() - should be called at the beginning of setup()
   update() - should be called at the beginning of loop()

   void in(address, side) - inflates bag of board with defined address
   void out(address, side) - deflates bag of board with defined address
   int readPressure(address) - returns pressure of bag at defined address. 
                              Notice that a non-deflated bag still experiences 
                              athmospheric pressure and thus does not return 0.
   bool readButton(side) - returns button state of IO Board. 0 is top, 1 is bottom.
   int readPot() - returns potentiometer value of IO Board. 0 - 15.

**********************************************************************************/


#include <Wire.h>
#include <PneuDuino.h>

char serialCommand = ' ';
PneuDuino air = PneuDuino();

void setup() {
  Serial.begin(9600);
  air.begin(); // initialize the pneuDuino library. DON'T MOVE THIS!
  /* YOUR SETUP-CODE HERE: */

  
}


void loop() {
  air.update(); // update the pneuduino library. DONT MOVE THIS!

  if (Serial.available() > 0) {     // use this to trigger things
    serialCommand = Serial.read();  // just send one letter            
    Serial.print("received: ");
    Serial.println(serialCommand);    
  }

  /* YOUR CODE HERE: */

  /* 
   *  This is just an example of how to trigger a valve with your keyboard.
   *  Just send A through the Serial Monitor and the valve opens
   *  Send a different letter and the valve closes again.
   *  Change this code as you like.
   */
   
  if (serialCommand == 'A'){
    air.in(10, RIGHT);
  } else {
    air.out(10, RIGHT);
  }

}
