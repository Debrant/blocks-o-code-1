/* 
Jacob Mickiewicz 3/25/15
a program for an attiny85 to be an I2C slave device

On read returns function.
On write changes the current funtion or i2c adress.
*/
/********************************************************************************

USI TWI Slave driver.

Created by Donald R. Blake. donblake at worldnet.att.net
Adapted by Jochen Toppe, jochen.toppe at jtoee.com

---------------------------------------------------------------------------------

Created from Atmel source files for Application Note AVR312: Using the USI Module
as an I2C slave.

This program is free software; you can redistribute it and/or modify it under the
terms of the GNU General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE.  See the GNU General Public License for more details.

---------------------------------------------------------------------------------

Change Activity:

    Date       Description
   ------      -------------
  16 Mar 2007  Created.
  27 Mar 2007  Added support for ATtiny261, 461 and 861.
  26 Apr 2007  Fixed ACK of slave address on a read.
  04 Jul 2007  Fixed USISIF in ATtiny45 def
  12 Dev 2009  Added callback functions for data requests

********************************************************************************/
/********************************************************************************

Header file for the USI TWI Slave driver.

Created by Donald R. Blake
donblake at worldnet.att.net

---------------------------------------------------------------------------------

Created from Atmel source files for Application Note AVR312: Using the USI Module
as an I2C slave.

This program is free software; you can redistribute it and/or modify it under the
terms of the GNU General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE.  See the GNU General Public License for more details.

---------------------------------------------------------------------------------

Change Activity:

    Date       Description
   ------      -------------
  15 Mar 2007  Created.

********************************************************************************/



#ifndef _USI_TWI_SLAVE_H_
#define _USI_TWI_SLAVE_H_



/********************************************************************************

                                    includes

********************************************************************************/

#include <stdbool.h>



/********************************************************************************

                                   prototypes

********************************************************************************/

void    usiTwiSlaveInit( uint8_t );
void    usiTwiTransmitByte( uint8_t );
uint8_t usiTwiReceiveByte( void );
bool    usiTwiDataInReceiveBuffer( void );
void    (*_onTwiDataRequest)(void);
bool    usiTwiDataInTransmitBuffer(void);
uint8_t usiTwiAmountDataInReceiveBuffer(void);
// on_XXX handler pointers
void    (*usi_onRequestPtr)(void);
void    (*usi_onReceiverPtr)(uint8_t);


/********************************************************************************

                           driver buffer definitions

********************************************************************************/

// permitted RX buffer sizes: 1, 2, 4, 8, 16, 32, 64, 128 or 256

#ifndef TWI_RX_BUFFER_SIZE
#define TWI_RX_BUFFER_SIZE  ( 16 )
#endif
#define TWI_RX_BUFFER_MASK  ( TWI_RX_BUFFER_SIZE - 1 )

#if ( TWI_RX_BUFFER_SIZE & TWI_RX_BUFFER_MASK )
#  error TWI RX buffer size is not a power of 2
#endif

// permitted TX buffer sizes: 1, 2, 4, 8, 16, 32, 64, 128 or 256

#ifndef TWI_TX_BUFFER_SIZE
#define TWI_TX_BUFFER_SIZE ( 16 )
#endif
#define TWI_TX_BUFFER_MASK ( TWI_TX_BUFFER_SIZE - 1 )

#if ( TWI_TX_BUFFER_SIZE & TWI_TX_BUFFER_MASK )
#  error TWI TX buffer size is not a power of 2
#endif



#endif  // ifndef _USI_TWI_SLAVE_H_



/********************************************************************************
                                    includes
********************************************************************************/

#include <avr/io.h>
#include <avr/interrupt.h>

//#include "usiTwiSlave.h"
//#include "../common/util.h"


/********************************************************************************
                            device dependent defines
********************************************************************************/

#if defined( __AVR_ATtiny2313__ )
#  define DDR_USI             DDRB
#  define PORT_USI            PORTB
#  define PIN_USI             PINB
#  define PORT_USI_SDA        PB5
#  define PORT_USI_SCL        PB7
#  define PIN_USI_SDA         PINB5
#  define PIN_USI_SCL         PINB7
#  define USI_START_COND_INT  USISIF
#  define USI_START_VECTOR    USI_START_vect
#  define USI_OVERFLOW_VECTOR USI_OVERFLOW_vect
#endif

#if defined(__AVR_ATtiny84__) | \
     defined(__AVR_ATtiny44__)
#  define DDR_USI             DDRA
#  define PORT_USI            PORTA
#  define PIN_USI             PINA
#  define PORT_USI_SDA        PORTA6
#  define PORT_USI_SCL        PORTA4
#  define PIN_USI_SDA         PINA6
#  define PIN_USI_SCL         PINA4
#  define USI_START_COND_INT  USISIF
#  define USI_START_VECTOR    USI_START_vect
#  define USI_OVERFLOW_VECTOR USI_OVF_vect
#endif

#if defined( __AVR_ATtiny25__ ) | \
     defined( __AVR_ATtiny45__ ) | \
     defined( __AVR_ATtiny85__ )
#  define DDR_USI             DDRB
#  define PORT_USI            PORTB
#  define PIN_USI             PINB
#  define PORT_USI_SDA        PB0
#  define PORT_USI_SCL        PB2
#  define PIN_USI_SDA         PINB0
#  define PIN_USI_SCL         PINB2
#  define USI_START_COND_INT  USISIF
#  define USI_START_VECTOR    USI_START_vect
#  define USI_OVERFLOW_VECTOR USI_OVF_vect
#endif

#if defined( __AVR_ATtiny26__ )
#  define DDR_USI             DDRB
#  define PORT_USI            PORTB
#  define PIN_USI             PINB
#  define PORT_USI_SDA        PB0
#  define PORT_USI_SCL        PB2
#  define PIN_USI_SDA         PINB0
#  define PIN_USI_SCL         PINB2
#  define USI_START_COND_INT  USISIF
#  define USI_START_VECTOR    USI_STRT_vect
#  define USI_OVERFLOW_VECTOR USI_OVF_vect
#endif

#if defined( __AVR_ATtiny261__ ) | \
     defined( __AVR_ATtiny461__ ) | \
     defined( __AVR_ATtiny861__ )
#  define DDR_USI             DDRB
#  define PORT_USI            PORTB
#  define PIN_USI             PINB
#  define PORT_USI_SDA        PB0
#  define PORT_USI_SCL        PB2
#  define PIN_USI_SDA         PINB0
#  define PIN_USI_SCL         PINB2
#  define USI_START_COND_INT  USISIF
#  define USI_START_VECTOR    USI_START_vect
#  define USI_OVERFLOW_VECTOR USI_OVF_vect
#endif

#if defined( __AVR_ATmega165__ ) | \
     defined( __AVR_ATmega325__ ) | \
     defined( __AVR_ATmega3250__ ) | \
     defined( __AVR_ATmega645__ ) | \
     defined( __AVR_ATmega6450__ ) | \
     defined( __AVR_ATmega329__ ) | \
     defined( __AVR_ATmega3290__ )
#  define DDR_USI             DDRE
#  define PORT_USI            PORTE
#  define PIN_USI             PINE
#  define PORT_USI_SDA        PE5
#  define PORT_USI_SCL        PE4
#  define PIN_USI_SDA         PINE5
#  define PIN_USI_SCL         PINE4
#  define USI_START_COND_INT  USISIF
#  define USI_START_VECTOR    USI_START_vect
#  define USI_OVERFLOW_VECTOR USI_OVERFLOW_vect
#endif

#if defined( __AVR_ATmega169__ )
#  define DDR_USI             DDRE
#  define PORT_USI            PORTE
#  define PIN_USI             PINE
#  define PORT_USI_SDA        PE5
#  define PORT_USI_SCL        PE4
#  define PIN_USI_SDA         PINE5
#  define PIN_USI_SCL         PINE4
#  define USI_START_COND_INT  USISIF
#  define USI_START_VECTOR    USI_START_vect
#  define USI_OVERFLOW_VECTOR USI_OVERFLOW_vect
#endif



/********************************************************************************

                        functions implemented as macros

********************************************************************************/

#define SET_USI_TO_SEND_ACK( ) \
{ \
  /* prepare ACK */ \
  USIDR = 0; \
  /* set SDA as output */ \
  DDR_USI |= ( 1 << PORT_USI_SDA ); \
  /* clear all interrupt flags, except Start Cond */ \
  USISR = \
       ( 0 << USI_START_COND_INT ) | \
       ( 1 << USIOIF ) | ( 1 << USIPF ) | \
       ( 1 << USIDC )| \
       /* set USI counter to shift 1 bit */ \
       ( 0x0E << USICNT0 ); \
}

#define SET_USI_TO_READ_ACK( ) \
{ \
  /* set SDA as input */ \
  DDR_USI &= ~( 1 << PORT_USI_SDA ); \
  /* prepare ACK */ \
  USIDR = 0; \
  /* clear all interrupt flags, except Start Cond */ \
  USISR = \
       ( 0 << USI_START_COND_INT ) | \
       ( 1 << USIOIF ) | \
       ( 1 << USIPF ) | \
       ( 1 << USIDC ) | \
       /* set USI counter to shift 1 bit */ \
       ( 0x0E << USICNT0 ); \
}

#define SET_USI_TO_TWI_START_CONDITION_MODE( ) \
{ \
  USICR = \
       /* enable Start Condition Interrupt, disable Overflow Interrupt */ \
       ( 1 << USISIE ) | ( 0 << USIOIE ) | \
       /* set USI in Two-wire mode, no USI Counter overflow hold */ \
       ( 1 << USIWM1 ) | ( 0 << USIWM0 ) | \
       /* Shift Register Clock Source = External, positive edge */ \
       /* 4-Bit Counter Source = external, both edges */ \
       ( 1 << USICS1 ) | ( 0 << USICS0 ) | ( 0 << USICLK ) | \
       /* no toggle clock-port pin */ \
       ( 0 << USITC ); \
  USISR = \
        /* clear all interrupt flags, except Start Cond */ \
        ( 0 << USI_START_COND_INT ) | ( 1 << USIOIF ) | ( 1 << USIPF ) | \
        ( 1 << USIDC ) | ( 0x0 << USICNT0 ); \
}

#define SET_USI_TO_SEND_DATA( ) \
{ \
  /* set SDA as output */ \
  DDR_USI |=  ( 1 << PORT_USI_SDA ); \
  /* clear all interrupt flags, except Start Cond */ \
  USISR    =  \
       ( 0 << USI_START_COND_INT ) | ( 1 << USIOIF ) | ( 1 << USIPF ) | \
       ( 1 << USIDC) | \
       /* set USI to shift out 8 bits */ \
       ( 0x0 << USICNT0 ); \
}

#define SET_USI_TO_READ_DATA( ) \
{ \
  /* set SDA as input */ \
  DDR_USI &= ~( 1 << PORT_USI_SDA ); \
  /* clear all interrupt flags, except Start Cond */ \
  USISR    = \
       ( 0 << USI_START_COND_INT ) | ( 1 << USIOIF ) | \
       ( 1 << USIPF ) | ( 1 << USIDC ) | \
       /* set USI to shift out 8 bits */ \
       ( 0x0 << USICNT0 ); \
}

#define USI_RECEIVE_CALLBACK() \
{ \
    if (usi_onReceiverPtr) \
    { \
        if (usiTwiDataInReceiveBuffer()) \
        { \
            usi_onReceiverPtr(usiTwiAmountDataInReceiveBuffer()); \
        } \
    } \
}

#define ONSTOP_USI_RECEIVE_CALLBACK() \
{ \
    if (USISR & ( 1 << USIPF )) \
    { \
        USI_RECEIVE_CALLBACK(); \
    } \
}


#define USI_REQUEST_CALLBACK() \
{ \
    USI_RECEIVE_CALLBACK(); \
    if(usi_onRequestPtr) usi_onRequestPtr(); \
}

/********************************************************************************

                                   typedef's

********************************************************************************/

typedef enum
{
  USI_SLAVE_CHECK_ADDRESS                = 0x00,
  USI_SLAVE_SEND_DATA                    = 0x01,
  USI_SLAVE_REQUEST_REPLY_FROM_SEND_DATA = 0x02,
  USI_SLAVE_CHECK_REPLY_FROM_SEND_DATA   = 0x03,
  USI_SLAVE_REQUEST_DATA                 = 0x04,
  USI_SLAVE_GET_DATA_AND_SEND_ACK        = 0x05
} overflowState_t;



/********************************************************************************

                                local variables

********************************************************************************/

static uint8_t                  slaveAddress;
static volatile overflowState_t overflowState;


static uint8_t          rxBuf[ TWI_RX_BUFFER_SIZE ];
static volatile uint8_t rxHead;
static volatile uint8_t rxTail;

static uint8_t          txBuf[ TWI_TX_BUFFER_SIZE ];
static volatile uint8_t txHead;
static volatile uint8_t txTail;

// data requested callback
void (*_onTwiDataRequest)(void);



/********************************************************************************

                                local functions

********************************************************************************/



// flushes the TWI buffers

static
void
flushTwiBuffers(
  void
)
{
  rxTail = 0;
  rxHead = 0;
  txTail = 0;
  txHead = 0;
} // end flushTwiBuffers



/********************************************************************************

                                public functions

********************************************************************************/



// initialise USI for TWI slave mode

void
usiTwiSlaveInit(
  uint8_t ownAddress
)
{

  flushTwiBuffers( );

  slaveAddress = ownAddress;

  // In Two Wire mode (USIWM1, USIWM0 = 1X), the slave USI will pull SCL
  // low when a start condition is detected or a counter overflow (only
  // for USIWM1, USIWM0 = 11).  This inserts a wait state.  SCL is released
  // by the ISRs (USI_START_vect and USI_OVERFLOW_vect).

  // Set SCL and SDA as output
  DDR_USI |= ( 1 << PORT_USI_SCL ) | ( 1 << PORT_USI_SDA );
 
  // set SCL high
  PORT_USI |= ( 1 << PORT_USI_SCL );

  // set SDA high
  PORT_USI |= ( 1 << PORT_USI_SDA );

  // Set SDA as input
  DDR_USI &= ~( 1 << PORT_USI_SDA );

  USICR =
       // enable Start Condition Interrupt
       ( 1 << USISIE ) |
       // disable Overflow Interrupt
       ( 0 << USIOIE ) |
       // set USI in Two-wire mode, no USI Counter overflow hold
       ( 1 << USIWM1 ) | ( 0 << USIWM0 ) |
       // Shift Register Clock Source = external, positive edge
       // 4-Bit Counter Source = external, both edges
       ( 1 << USICS1 ) | ( 0 << USICS0 ) | ( 0 << USICLK ) |
       // no toggle clock-port pin
       ( 0 << USITC );

  // clear all interrupt flags and reset overflow counter

  USISR = ( 1 << USI_START_COND_INT ) | ( 1 << USIOIF ) | ( 1 << USIPF ) | ( 1 << USIDC );

} // end usiTwiSlaveInit


bool usiTwiDataInTransmitBuffer(void)
{

  // return 0 (false) if the receive buffer is empty
  return txHead != txTail;

} // end usiTwiDataInTransmitBuffer


// put data in the transmission buffer, wait if buffer is full

void
usiTwiTransmitByte(
  uint8_t data
)
{

  uint8_t tmphead;

  // calculate buffer index
  tmphead = ( txHead + 1 ) & TWI_TX_BUFFER_MASK;

  // wait for free space in buffer
  while ( tmphead == txTail );

  // store data in buffer
  txBuf[ tmphead ] = data;

  // store new index
  txHead = tmphead;

} // end usiTwiTransmitByte





// return a byte from the receive buffer, wait if buffer is empty

uint8_t
usiTwiReceiveByte(
  void
)
{

  // wait for Rx data
  while ( rxHead == rxTail );

  // calculate buffer index
  rxTail = ( rxTail + 1 ) & TWI_RX_BUFFER_MASK;

  // return data from the buffer.
  return rxBuf[ rxTail ];

} // end usiTwiReceiveByte



// check if there is data in the receive buffer

bool
usiTwiDataInReceiveBuffer(
  void
)
{

  // return 0 (false) if the receive buffer is empty
  return rxHead != rxTail;

} // end usiTwiDataInReceiveBuffer

uint8_t usiTwiAmountDataInReceiveBuffer(void)
{
    if (rxHead == rxTail)
    {
        return 0;
    }
    if (rxHead < rxTail)
    {
        // Is there a better way ?
        return ((int8_t)rxHead - (int8_t)rxTail) + TWI_RX_BUFFER_SIZE;
    }
    return rxHead - rxTail;
}
 
 


/********************************************************************************

                            USI Start Condition ISR

********************************************************************************/

ISR( USI_START_VECTOR )
{

  /*
  // This triggers on second write, but claims to the callback there is only *one* byte in buffer
  ONSTOP_USI_RECEIVE_CALLBACK();
  */
  /*
  // This triggers on second write, but claims to the callback there is only *one* byte in buffer
  USI_RECEIVE_CALLBACK();
  */

  // set default starting conditions for new TWI package
  overflowState = USI_SLAVE_CHECK_ADDRESS;

  // set SDA as input
  DDR_USI &= ~( 1 << PORT_USI_SDA );

  // wait for SCL to go low to ensure the Start Condition has completed (the
  // start detector will hold SCL low ) - if a Stop Condition arises then leave
  // the interrupt to prevent waiting forever - don't use USISR to test for Stop
  // Condition as in Application Note AVR312 because the Stop Condition Flag is
  // going to be set from the last TWI sequence
  while (
       // SCL his high
       ( PIN_USI & ( 1 << PIN_USI_SCL ) ) &&
       // and SDA is low
       !( ( PIN_USI & ( 1 << PIN_USI_SDA ) ) )
  );


  if ( !( PIN_USI & ( 1 << PIN_USI_SDA ) ) )
  {

    // a Stop Condition did not occur

    USICR =
         // keep Start Condition Interrupt enabled to detect RESTART
         ( 1 << USISIE ) |
         // enable Overflow Interrupt
         ( 1 << USIOIE ) |
         // set USI in Two-wire mode, hold SCL low on USI Counter overflow
         ( 1 << USIWM1 ) | ( 1 << USIWM0 ) |
         // Shift Register Clock Source = External, positive edge
         // 4-Bit Counter Source = external, both edges
         ( 1 << USICS1 ) | ( 0 << USICS0 ) | ( 0 << USICLK ) |
         // no toggle clock-port pin
         ( 0 << USITC );

  }
  else
  {
    // a Stop Condition did occur

    USICR =
         // enable Start Condition Interrupt
         ( 1 << USISIE ) |
         // disable Overflow Interrupt
         ( 0 << USIOIE ) |
         // set USI in Two-wire mode, no USI Counter overflow hold
         ( 1 << USIWM1 ) | ( 0 << USIWM0 ) |
         // Shift Register Clock Source = external, positive edge
         // 4-Bit Counter Source = external, both edges
         ( 1 << USICS1 ) | ( 0 << USICS0 ) | ( 0 << USICLK ) |
         // no toggle clock-port pin
         ( 0 << USITC );

  } // end if

  USISR =
       // clear interrupt flags - resetting the Start Condition Flag will
       // release SCL
       ( 1 << USI_START_COND_INT ) | ( 1 << USIOIF ) |
       ( 1 << USIPF ) |( 1 << USIDC ) |
       // set USI to sample 8 bits (count 16 external SCL pin toggles)
       ( 0x0 << USICNT0);


} // end ISR( USI_START_VECTOR )



/********************************************************************************

                                USI Overflow ISR

Handles all the communication.

Only disabled when waiting for a new Start Condition.

********************************************************************************/

ISR( USI_OVERFLOW_VECTOR )
{

  switch ( overflowState )
  {

    // Address mode: check address and send ACK (and next USI_SLAVE_SEND_DATA) if OK,
    // else reset USI
    case USI_SLAVE_CHECK_ADDRESS:
      if ( ( USIDR == 0 ) || ( ( USIDR >> 1 ) == slaveAddress) )
      {
         // callback
         if(_onTwiDataRequest) _onTwiDataRequest();
         if ( USIDR & 0x01 )
        {
          overflowState = USI_SLAVE_SEND_DATA;
        }
        else
        {
          overflowState = USI_SLAVE_REQUEST_DATA;
        } // end if
        SET_USI_TO_SEND_ACK( );
      }
      else
      {
        SET_USI_TO_TWI_START_CONDITION_MODE( );
      }
      break;

    // Master write data mode: check reply and goto USI_SLAVE_SEND_DATA if OK,
    // else reset USI
    case USI_SLAVE_CHECK_REPLY_FROM_SEND_DATA:
      if ( USIDR )
      {
        // if NACK, the master does not want more data
        SET_USI_TO_TWI_START_CONDITION_MODE( );
        return;
      }
      // from here we just drop straight into USI_SLAVE_SEND_DATA if the
      // master sent an ACK

    // copy data from buffer to USIDR and set USI to shift byte
    // next USI_SLAVE_REQUEST_REPLY_FROM_SEND_DATA
    case USI_SLAVE_SEND_DATA:
      USI_REQUEST_CALLBACK();
      // Get data from Buffer
      if ( txHead != txTail )
      {
        txTail = ( txTail + 1 ) & TWI_TX_BUFFER_MASK;
        USIDR = txBuf[ txTail ];
      }
      else
      {
        // the buffer is empty
        SET_USI_TO_READ_ACK( ); // This might be neccessary sometimes see http://www.avrfreaks.net/index.php?name=PNphpBB2&file=viewtopic&p=805227#805227
        SET_USI_TO_TWI_START_CONDITION_MODE( );
        return;
      } // end if
      overflowState = USI_SLAVE_REQUEST_REPLY_FROM_SEND_DATA;
      SET_USI_TO_SEND_DATA( );
      break;

    // set USI to sample reply from master
    // next USI_SLAVE_CHECK_REPLY_FROM_SEND_DATA
    case USI_SLAVE_REQUEST_REPLY_FROM_SEND_DATA:
      overflowState = USI_SLAVE_CHECK_REPLY_FROM_SEND_DATA;
      SET_USI_TO_READ_ACK( );
      break;

    // Master read data mode: set USI to sample data from master, next
    // USI_SLAVE_GET_DATA_AND_SEND_ACK
    case USI_SLAVE_REQUEST_DATA:
      overflowState = USI_SLAVE_GET_DATA_AND_SEND_ACK;
      SET_USI_TO_READ_DATA( );
      break;

    // copy data from USIDR and send ACK
    // next USI_SLAVE_REQUEST_DATA
    case USI_SLAVE_GET_DATA_AND_SEND_ACK:
      // put data into buffer
      // Not necessary, but prevents warnings
      rxHead = ( rxHead + 1 ) & TWI_RX_BUFFER_MASK;
      // check buffer size
      if (rxHead == rxTail) {
        // overrun
        rxHead = (rxHead + TWI_RX_BUFFER_SIZE - 1) & TWI_RX_BUFFER_MASK;
      } else {
        rxBuf[ rxHead ] = USIDR;
      }
      // next USI_SLAVE_REQUEST_DATA
      overflowState = USI_SLAVE_REQUEST_DATA;
      SET_USI_TO_SEND_ACK( );
      break;

  } // end switch

} // end ISR( USI_OVERFLOW_VECTOR )


extern "C"{
//#include "usiTwiSlave.h"
#include <inttypes.h>
#include <avr/interrupt.h>
}

#define I2C_SLAVE_ADDRESS 4 //in use it'll be set by another program (local bus code) can also be set over the i2c bus
#define BLOCK_FUNCTION 0x11 //in phase 2 will be set by another program (adc ->function code) can also be set over the i2c bus
/* list of functions by number
0x11 this test
*/

volatile uint8_t State; //this keeps track of the state or mode of the block
volatile uint8_t Block_Function = BLOCK_FUNCTION; //this is the lexical function 

void requestEvent()  //this runs when a read is detected for address
{  
  switch (State)
  {
  case 0: //reading the function, default
  default:
    usiTwiTransmitByte(Block_Function);
    break; 
  }
}

void receiveEvent(uint8_t HowMany) //this runs when a write is detected for address 
{                                  //flow is slave_write then state_# then value. 3 bytes to changes omething
    switch (State)
    {
      case 2: //changing slave address
        usiTwiSlaveInit(usiTwiReceiveByte());
        State = 0;
        break;
      case 1: //setting a new function
        Block_Function = usiTwiReceiveByte();
        State = 0;
        break;
      case 0: //setting a new state, default
      default:       
        State = usiTwiReceiveByte();
        break;
    }
}

void I2C_setup(uint8_t slave_add)
{
     usiTwiSlaveInit(slave_add);
}


void setup() 
{
     I2C_setup(I2C_SLAVE_ADDRESS);
     usi_onReceiverPtr = receiveEvent;
     usi_onRequestPtr = requestEvent;

}

void loop() 
{
    {
    if (!usi_onReceiverPtr)
    {
        // no onReceive callback, nothing to do...
        return;
    }
    if (!(USISR & ( 1 << USIPF )))
    {
        // Stop not detected
        return;
    }
    uint8_t amount = usiTwiAmountDataInReceiveBuffer();
    if (amount == 0)
    {
        // no data in buffer
        return;
    }
    usi_onReceiverPtr(amount);
}

}
