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

typedef struct ChannelsData {
  uint16union_t frequency;
  TIDMTCharacteristic IDMTCharacteristic;
  TFaultType faultType;
  uint16union_t *NumberOfTrips;
}TChannelsData;

typedef struct Sample {
  int16_t * const VoltageSamples[16];
  int16_t * const VoltageSamplesSqr[16];
  uint8_t channelNb;
  OS_ECB* semaphore;
  float vRMS;
  float iRMS;
  float triptime;
}TSample;

bool Sample_Init(void);

float Voltage_RMS(const TSample* const sample);

float Current_RMS(const TSample* const sample);

float TripTimeCalculation(const TSample* const sample, const TChannelsData* const channelsdata);
