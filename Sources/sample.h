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
  uint16union_t *numberOfTrips;
}TChannelsData;

typedef struct Sample {
  float voltageSamples[16];
  float voltageSamplesSqr[16];
  float totalVoltageSqr;
  float vRMS;
  float iRMS;
  float triptime;
  float frequencysamples[4];
}TSample;

#define ADC_RATE 3276.7
#define RAW_TO_VOLTAGE(X) (float) X / (float) ADC_RATE
#define VOLTAGE_TO_RAW(X) (int16_t) (float) X * (float) ADC_RATE

/*! @brief Sample module initiation
 *
 */
bool Sample_Init(TChannelsData* channelsdata);

/*! @brief Sliding Voltage function
 * @param Pointer to struct Sample, new voltage
 * @note function squares new voltages and stores them in an array
 */
bool Sliding_Voltage(TSample* sample, float voltage);

/*! @brief Voltage RMS function
 * @param Pointer to struct Sample
 * Determines voltage via formula
 */
bool Voltage_RMS(TSample* sample);

/*! @brief Calculate iRMS
 * Determines iRMS based on vRMS
 */
bool Current_RMS(TSample* sample);

/*! @brief Trip time calculation
 * Determine trip time via characteristic and current, look up in TripTime array
 */
bool Trip_Time_Calculation(TSample* sample, TChannelsData* channelsdata);

/*! @brief Frequency calculation
 * Determine frequency via the Voltage samples
 */
bool Frequency_Calculation(TSample* sample, TChannelsData* channelsdata, float currentsampletime, uint8_t numberofsamples);

