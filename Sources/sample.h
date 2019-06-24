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
  float VoltageSamples[16];
  float VoltageSamplesSqr[16];
  float TotalVoltageSqr;
  float vRMS;
  float iRMS;
  float triptime;
}TSample;

#define ADC_RATE 3276.7
#define RAW_TO_VOLTAGE(X) (float) X / (float) ADC_RATE
#define VOLTAGE_TO_RAW(X) (int16_t) (float) X * (float) ADC_RATE

bool Sample_Init(TChannelsData* channelsdata);

bool Sliding_Voltage(TSample* sample, float voltage);

bool Voltage_RMS(TSample* sample);

bool Current_RMS(TSample* sample);

bool TripTimeCalculation(TSample* sample, TChannelsData* channelsdata);


