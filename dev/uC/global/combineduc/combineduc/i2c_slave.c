/* 
Jacob Mickiewicz 3/25/15
a program for an attiny85 to be an I2C slave device

On read returns function.
On write changes the current funtion or i2c adress.
*/

#include "usiTwiSlave.h"
#include <inttypes.h>
#include <avr/interrupt.h>


#define I2C_SLAVE_ADDRESS 4 //in use it'll be set by another program (local bus code) can also be set over the i2c bus
#define BLOCK_FUNCTION 0x11 //in phase 2 will be set by another program (adc ->function code) can also be set over the i2c bus
/* list of functions by number
0x11 this test
*/

volatile uint8_t State_i2c; //this keeps track of the state or mode of the block
volatile uint8_t Block_Function = BLOCK_FUNCTION; //this is the lexical function 

void requestEvent_i2c()  //this runs when a read is detected for address
{  
  switch (State_i2c)
  {
  case 0: //reading the function, default
  default:
    usiTwiTransmitByte(Block_Function);
    break; 
  }
}

void receiveEvent_i2c(uint8_t HowMany) //this runs when a write is detected for address 
{                                  //flow is slave_write then state_# then value. 3 bytes to changes omething
    switch (State_i2c)
    {
	  case 3: //reset
		usiTwiReceiveByte(); //trash 3rd byte
		break;
      case 2: //changing slave address
        usiTwiSlaveInit(usiTwiReceiveByte());
        State_i2c = 0;
        break;
      case 1: //setting a new function
        Block_Function = usiTwiReceiveByte();
        State_i2c = 0;
        break;
      case 0: //setting a new state, default
      default:       
        State_i2c = usiTwiReceiveByte();
        break;
    }
}

void I2C_setup(uint8_t slave_add)
{
     usiTwiSlaveInit(slave_add);
}


void setup_i2c() 
{
     I2C_setup(I2C_SLAVE_ADDRESS);
     usi_onReceiverPtr = receiveEvent_i2c;
     usi_onRequestPtr = requestEvent_i2c;

}

bool loop_i2c() 
{
	if(State_i2c==3)
	{
		return(false);
	}
    {
		if (!usi_onReceiverPtr)
		{
			// no onReceive callback, nothing to do...
			return(true);
		}
		if (!(USISR & ( 1 << USIPF )))
		{
			// Stop not detected
			return(true);
		}
		uint8_t amount = usiTwiAmountDataInReceiveBuffer();
		if (amount == 0)
		{
			// no data in buffer
			return(true);
		}
		usi_onReceiverPtr(amount);
	}
}