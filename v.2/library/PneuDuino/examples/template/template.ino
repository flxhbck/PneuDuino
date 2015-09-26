/*************************************************************************

    ____  ____     ___  __ __  ___    __ __  ____  ____    ___  
   |    \|    \   /  _]|  |  ||   \  |  |  ||    ||    \  /   \ 
   |  o  )  _  | /  [_ |  |  ||    \ |  |  | |  | |  _  ||     |
   |   _/|  |  ||    _]|  |  ||  D  ||  |  | |  | |  |  ||  O  |
   |  |  |  |  ||   [_ |  :  ||     ||  :  | |  | |  |  ||     |
   |  |  |  |  ||     ||     ||     ||     | |  | |  |  ||     |
   |__|  |__|__||_____| \__,_||_____| \__,_||____||__|__| \___/ 

   PneuDuino is a toolkit for controlling inflatable actuators
   and prototyping interactions with shape-changing systems.

   Functions:

   begin() - should be called at the beginning of setup()
   update() - should be called at the beginning of loop()

   void in(address, side) - inflates bag of board with defined address
   void out(address, side) - deflates bag of board with defined address
   int getPressure(address) - returns pressure of bag at defined address
   int getDialState(address) - returns the position of the dial at defined address
   bool getButtonState(address, side) - returns the state of the button at defined address
   

*****************************************************************************/


#include <Wire.h>
#include <PneuDuino.h>

PneuDuino air = PneuDuino();

void setup() {
  Serial.begin(9600);
  air.begin(); // initialize the pneuDuino library. DON'T MOVE THIS!
  /* YOUR CODE HERE: */
  
}

void loop() {
  air.update(); // update the pneuduino library. DONT MOVE THIS!
  /* YOUR CODE HERE: */
  
}
