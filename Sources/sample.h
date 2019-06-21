//Perform calculations on samples to determine characteristics

#include "OS.h"

typedef enum
{
  INVERSE,
  VERY_INVERSE,
  EXTREMELY_INVERSE
} TIDMTCharacteristic;

typedef enum {
  ONE_PHASE,
  TWO_PHASE,
  THREE_PHASE
}TFaultType;

typedef struct Sample {
  int16_t * const VoltageSamples[16];
  int16_t * const VoltageSamplesSqr[16];
  uint8_t channelNb;
  OS_ECB* semaphore;
  float vRMS;
  float iRMS;
  float triptime;
  uint16union_t frequency;
  TIDMTCharacteristic IDMTCharacteristic;
  TFaultType faultType;
}TSample;

bool Sample_Init(void);

float Voltage_RMS(const TSample* const sample);

float Current_RMS(const TSample* const sample);

float TripTimeCalculation(const TSample* const sample);
