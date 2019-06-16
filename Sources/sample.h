//Perform calculations on samples to determine characteristics

typedef enum
{
  INVERSE,
  VERY_INVERSE,
  EXTREMELY_INVERSE
} TIDMTCharacteristic;

typedef enum {
  a,
  b,
  c
}TFaultType;

typedef struct {
  float Voltage;
  float voltageRMS;
  float currentRMS;

  uint16union_t frequency;
  uint8_t current;
  uint8_t numberOfTrips;
  TIDMTCharacteristic IDMTCharacteristic;
  TFaultType faultType;

}TSample;

void Sample_Init(void) {

}

void Sliding_Voltage() {

}

void Current_RMS() {

}

void True_RMS(float Voltage) {

}
