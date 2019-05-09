//This code is to use with L3G4200 triple axis gyro
//Modified by SurtrTech

#include <avr/io.h>
#include <avr/interrupt.h>  /* for sei() */
#include <util/delay.h>     /* for _delay_ms() */
#include <avr/pgmspace.h>   /* required by usbdrv.h */
#include <Wire.h>
#include "usbdrv.h"

#define RQ_CMD         0

#define CTRL_REG1 0x20
#define CTRL_REG2 0x21
#define CTRL_REG3 0x22
#define CTRL_REG4 0x23
#define CTRL_REG5 0x24

int L3G4200D_Address = 105; //I2C address of the L3G4200D

int x;
int y;
int z;
int cmd = 5;

usbMsgLen_t usbFunctionSetup(uint8_t data[8])
{
    usbRequest_t *rq = (void *)data;

    /* declared as static so they stay valid when usbFunctionSetup returns */
    static uint8_t switch_state;  
  static uint16_t light;
    static uint8_t gyro_state;

    if (rq->bRequest == RQ_CMD)
    {
        gyro_state = cmd;

        /* point usbMsgPtr to the data to be returned to host */
        usbMsgPtr = &gyro_state;

        /* return the number of bytes of data to be returned to host */
        return 1;
    }
    /* default for not implemented requests: return no data back to host */
    return 0;
}

void setup(){

    usbInit();

    /* enforce re-enumeration, do this while interrupts are disabled! */
    usbDeviceDisconnect();
    _delay_ms(300);
    usbDeviceConnect();

    /* enable global interrupts */
    sei();


Wire.begin();
  Serial.begin(38400);
  Serial.println("connected");
  
  Serial.println("starting up L3G4200D");
  
  setupL3G4200D(2000); // Configure L3G4200 - 250, 500 or 2000 deg/sec
  delay(1500); //wait for the sensor to be ready 
}

void loop(){
  cmd = 5;
  getGyroValues(); // This will update x, y, and z with new values
  if((abs(x) > abs(y)) && (abs(x) > 600)){
    if(x > 0){
      cmd = 1; //forward
    }
    else{
      cmd = 2; //backward
    }
  }
  else if((abs(x) < abs(y)) && (abs(y) > 600)){
    if(y > 0){
      cmd = 3; //left
    }
    else{
      cmd = 4; //right
    }
  }

  //Just here to slow down the serial to make it more readable
  usbPoll();
}

void getGyroValues(){

byte xMSB = readRegister(L3G4200D_Address, 0x29);
 byte xLSB = readRegister(L3G4200D_Address, 0x28);
 x = ((xMSB << 8) | xLSB);

byte yMSB = readRegister(L3G4200D_Address, 0x2B);
 byte yLSB = readRegister(L3G4200D_Address, 0x2A);
 y = ((yMSB << 8) | yLSB);

byte zMSB = readRegister(L3G4200D_Address, 0x2D);
 byte zLSB = readRegister(L3G4200D_Address, 0x2C);
 z = ((zMSB << 8) | zLSB);
}

int setupL3G4200D(int scale){
 //From Jim Lindblom of Sparkfun's code

// Enable x, y, z and turn off power down:
 writeRegister(L3G4200D_Address, CTRL_REG1, 0b00001111);

// If you'd like to adjust/use the HPF, you can edit the line below to configure CTRL_REG2:
 writeRegister(L3G4200D_Address, CTRL_REG2, 0b00000000);

// Configure CTRL_REG3 to generate data ready interrupt on INT2
 // No interrupts used on INT1, if you'd like to configure INT1
 // or INT2 otherwise, consult the datasheet:
 writeRegister(L3G4200D_Address, CTRL_REG3, 0b00001000);

// CTRL_REG4 controls the full-scale range, among other things:

if(scale == 250){
 writeRegister(L3G4200D_Address, CTRL_REG4, 0b00000000);
 }else if(scale == 500){
 writeRegister(L3G4200D_Address, CTRL_REG4, 0b00010000);
 }else{
 writeRegister(L3G4200D_Address, CTRL_REG4, 0b00110000);
 }

// CTRL_REG5 controls high-pass filtering of outputs, use it
 // if you'd like:
 writeRegister(L3G4200D_Address, CTRL_REG5, 0b00000000);
}

void writeRegister(int deviceAddress, byte address, byte val) {
 Wire.beginTransmission(deviceAddress); // start transmission to device 
 Wire.write(address); // send register address
 Wire.write(val); // send value to write
 Wire.endTransmission(); // end transmission
}

int readRegister(int deviceAddress, byte address){

int v;
 Wire.beginTransmission(deviceAddress);
 Wire.write(address); // register to read
 Wire.endTransmission();

Wire.requestFrom(deviceAddress, 1); // read a byte

while(!Wire.available()) {
 usbPoll();
 }

v = Wire.read();
 return v;
}
