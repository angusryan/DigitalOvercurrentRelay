//Perform calculations on samples to determine characteristics
#include "math.h"
#include "Cpu.h"
#include "types.h"
#include "sample.h"

struct TSample sample[3];

void Sample_Init(void) {
  //set LED high?
  //return true?
  return true;
}

void Sliding_Voltage() {
  uint8_t i = 0;
  for(i=0; i<16;i++ ) {
    sample[0].vRMS = 0.00;
  }
  // raw value from analog get
  //convert to voltage
 // VRMS from 16 samples of voltages
}

void Current_RMS(float vRMS) {
  //iRMS = vRMS/0.350
}

void Trip(float iRMS) {
  //Time to Trip = k/(iRMS^a-1)
  //if iRMS < 1.03 -> time = infinite
  //PIT to output 5v after a certain amount of time
}

/*A true RMS meter works by taking the square of the instantaneous value of the input voltage or current,
  averaging this value over time, and then displaying the square root of this average.*/

//One PIT firing every 16 times per 50Hz Cycle to store current instaneous voltage at that moment
//Second PIT firing for the trip after dedicated time period
//3 channel - 3 structs referencing each channel and its values?
//
