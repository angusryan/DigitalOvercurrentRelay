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

struct Charteristic { float k, a; };
struct Charteristic Inverse            = {.k = 0.14 , .a = 0.02};
struct Charteristic Very_Inverse       = {.k = 13.5 , .a = 1};
struct Charteristic Extremely_Inverse  = {.k = 80   , .a = 2};

typedef struct {
  float VoltageSamples[16];
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

void Trip(float iRMS);
