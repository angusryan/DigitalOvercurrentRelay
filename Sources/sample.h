//Perform calculations on samples to determine characteristics

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

typedef struct {
  int16_t * const VoltageSamples[16];
  float vRMS;
  float iRMS;
  uint16union_t frequency;
  uint8_t current;
  uint8_t numberOfTrips;
  TIDMTCharacteristic IDMTCharacteristic;
  TFaultType faultType;
}TSample;

bool Sample_Init(void);

float Sliding_Voltage();

float Current_RMS(float vRMS);

void TripTimeCalculations(float iRMS);
