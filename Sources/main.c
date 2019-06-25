/* ###################################################################
 **     Filename    : main.c
 **     Project     : Digital Overcurrent Relay
 **     Processor   : MK70FN1M0VMJ12
 **     Version     : Driver 01.01
 **     Compiler    : GNU C Compiler
 **     Date/Time   : 2019-06-25, 13:27, # CodeGen: 0
 **     Abstract    :
 **         Main module.
 **         This module contains user's application code.
 **     Settings    :
 **     Contents    :
 **         No public methods
 **
 ** ###################################################################*/
/*!
 ** @file main.c
 ** @version 6.0
 ** @brief
 **         Main module.
 **         This module contains user's application code.
 */
/*!
 **  @addtogroup main_module main module documentation
 **  @{
 */
/* MODULE main */


#include "Cpu.h" // CPU module - contains low level hardware initialization routines
#include "OS.h"
#include "analog.h"
#include "math.h"
#include "UART.h"
#include "packet.h"
#include "Flash.h"
#include "PIT.h"
#include "LEDs.h"
#include "sample.h"

// Global variables and macro definitions
#define THREAD_STACK_SIZE 100 // Arbitrary thread stack size - big enough for stacking of interrupts and OS use.
#define NB_ANALOG_CHANNELS 3 //declare number of channels , in this case 3 -> 0, 1, 2

const uint32_t BAUDRATE = 115200; /*!< Baud Rate specified in project */
const uint16_t STUDENT_ID = 0x22E2; /*!< Student Number: 7533 */
const uint32_t PIT_Period = 1250000; //1000000000; /*!< 1/1056Hz = 641025640 ns*/
const uint8_t PACKET_ACK_MASK = 0x80; /*!< Packet Acknowledgment mask, referring to bit 7 of the Packet */
const uint8_t ANALOG_THREAD_PRIORITIES[NB_ANALOG_CHANNELS] = {3, 4, 5};
static const float SAMPLE_TIME = 1.25; //declared static global const used for the sample time
static volatile uint16union_t *TowerNumber; /*!< declaring static TowerNumber Pointer */
static volatile uint16union_t *TowerMode; /*!< declaring static TowerMode Pointer */


OS_ECB* PacketHandlerSemaphore; //Declare a semaphore, to be signaled.

typedef enum
{
  TOWER_INIT_PRI,
  UART_RX_PRI,
  UART_TX_PRI,
  ANALOG_CHANNEL_1,
  ANALOG_CHANNEL_2,
  ANALOG_CHANNEL_3,
  PIT_PRI,
  PACKET_HANDLER_PRI
}TPRIORITIES;

// Prototypes functions
bool TowerInit(void);
bool PacketHandler(void);
bool StartupPackets(void);
bool VersionPackets(void);
bool TowerNumberPackets(void);
bool TowerModePackets(void);
bool DORInformationPackets(void);
void ThreadsInit(void);
void PITCallback(void);
void ResetDOR(void);

// Thread stacks
static uint32_t AnalogThreadStacks[NB_ANALOG_CHANNELS][THREAD_STACK_SIZE] __attribute__ ((aligned(0x08)));
OS_THREAD_STACK(TowerInitStack, THREAD_STACK_SIZE);
OS_THREAD_STACK(PITStack, THREAD_STACK_SIZE);
OS_THREAD_STACK(UARTRXStack, THREAD_STACK_SIZE);
OS_THREAD_STACK(UARTTXStack, THREAD_STACK_SIZE);
OS_THREAD_STACK(PacketHandlerStack, THREAD_STACK_SIZE);


/*! @brief Data structure used to pass Analog configuration to a user thread
 *
 */
typedef struct AnalogThreadData
{
  OS_ECB* semaphore;
  uint8_t channelNb;
} TAnalogThreadData;

TSample Sample[NB_ANALOG_CHANNELS]; //Declare global struct array of size 3 (for 3 channels) -> Sample
TChannelsData ChannelsData; //Declare global struct ChannelsData

/*! @brief Analog thread configuration data
 *
 */
static TAnalogThreadData AnalogThreadData[NB_ANALOG_CHANNELS] =
{
  {
    .semaphore = NULL,
    .channelNb = 0
  },
  {
    .semaphore = NULL,
    .channelNb = 1
  },
  {
    .semaphore = NULL,
    .channelNb = 2
  }
};

/*lint -save  -e970 Disable MISRA rule (6.3) checking. */
int main(void)
/*lint -restore Enable MISRA rule (6.3) checking. */
{
  // Initialise low-level clocks etc using Processor Expert code
  PE_low_level_init();
  // Initialize the RTOS
  OS_Init(CPU_CORE_CLK_HZ, true);
  // Create module initialisation thread
  ThreadsInit();
  PacketHandlerSemaphore = OS_SemaphoreCreate(0);
  // Start multithreading - never returns!
  OS_Start();
}

/*! @brief Initialises modules.
 *
 */
static void TowerInitThread(void* pData)
{
  OS_DisableInterrupts();
  // Analog
  (void)Analog_Init(CPU_BUS_CLK_HZ);

  // Generate the global analog semaphores
  for (uint8_t analogNb = 0; analogNb < NB_ANALOG_CHANNELS; analogNb++)
    AnalogThreadData[analogNb].semaphore = OS_SemaphoreCreate(0);

  /*!<  Allocate var for both Tower Number and Mode, if succcessful, FlashWrite16 them with the right values */
  Flash_Init();
  bool towerModeInit = Flash_AllocateVar( (volatile void **) &TowerMode, sizeof(*TowerMode));
  bool towerNumberInit = Flash_AllocateVar((volatile void **) &TowerNumber, sizeof(*TowerNumber));

  if(towerModeInit && towerNumberInit)
  {
    if(TowerMode->l == 0xffff) /* when unprogrammed, value = 0xffff, announces in hint*/
    {
      Flash_Write16((volatile uint16_t *) TowerMode, 0x1); /*!< Parsing through the function: typecast volatile uint16_t pointer from uint16union_t pointer, and default towerMode = 1 */
    }
    if(TowerNumber->l == 0xffff) /* when unprogrammed, value = 0xffff, announces in hint*/
    {
      Flash_Write16((volatile uint16_t *) TowerNumber, STUDENT_ID); /*Like above, but with towerNumber set to our student ID = 7533*/
    }
  }
  LEDs_Init();
  Sample_Init(&ChannelsData);
  PIT_Init(CPU_BUS_CLK_HZ, (void*) &PITCallback, NULL);
  PIT_Set(PIT_Period, true);
  Packet_Init(BAUDRATE, CPU_BUS_CLK_HZ);
  OS_EnableInterrupts();
  while(OS_SemaphoreSignal(PacketHandlerSemaphore) != OS_NO_ERROR);
  // We only do this once - therefore delete this thread
  OS_ThreadDelete(OS_PRIORITY_SELF);
}

/*! @brief Samples a value on an ADC channel and sends it to the corresponding DAC channel.
 *
 */
void AnalogLoopbackThread(void* pData)
{
  #define analogData ((TAnalogThreadData*)pData) // Make the code easier to read by giving a name to the typecast'ed pointer
  float currentiRMS;
  float remainderTimePercentage;
  uint16_t counter; //Initialize 16-bit counter variable;
  uint8_t loopcounter;
  uint8_t i =1;
  int16_t analogInputValue;
  int16_t intiRMS;
  int16_t intcurrentiRMS;
  bool firstset = false;
  bool trippedTimer = false;
  bool reset = false;

  for (;;)
  {
    (void)OS_SemaphoreWait(analogData->semaphore, 0);//wait until Semaphore signaled by PIT Callback
    OS_DisableInterrupts(); //Disable Interrupts
    Analog_Get(analogData->channelNb, &analogInputValue); //Get a sample, returning raw voltage value, storing in structure
    Sliding_Voltage(&Sample[analogData->channelNb], RAW_TO_VOLTAGE(analogInputValue)); //Store retreived analogInputValue into my sliding window array.
    Voltage_RMS(&Sample[analogData->channelNb]); //Store vRMS in structure, to be used to determine iRMS
    Current_RMS(&Sample[analogData->channelNb]); //Store iRMS in structure, to be used to determine if circuit must be tripped
    intiRMS = ((Sample[analogData->channelNb].iRMS)*1000); //turn iRMS into an integer so can discard 4 decimal values as they fluctuate to easily between samples
    intcurrentiRMS = (currentiRMS*1000); //calculate the integer from originally stored previous iRMS
    if ((Sample[analogData->channelNb].iRMS >= 1.03) && (intiRMS != intcurrentiRMS))  //Trip circuit if current is above 1.03 amps.
    {
      LEDs_On(LED_BLUE); //turn on blue LED to represent trip time has been set
      currentiRMS = Sample[analogData->channelNb].iRMS; //set the iRMS that triggered the trip as the new current iRMS to recheck on next loop.
      if (counter > 0) //if counter is > 0, trip time has previously been set therefore implement inverse timing
      {
        remainderTimePercentage = 1.00-(counter/((1000/SAMPLE_TIME)*(Sample[analogData->channelNb].triptime))); //1.00-(past time/orignal time) to find percentage completed, then minus to find remaining percentage
        Trip_Time_Calculation(Sample, &ChannelsData); //fetch Trip time, dependant on IDMT Characteristic set
        counter = ((1000/SAMPLE_TIME)*Sample[analogData->channelNb].triptime)*(remainderTimePercentage); //based on percentage multiplied by new trip time, set the counter to new value
      }
      else
      {
        Trip_Time_Calculation(Sample, &ChannelsData); //fetch Trip time, dependant on IDMT Characteristic set
        counter = (1000/SAMPLE_TIME)*Sample[analogData->channelNb].triptime; //set up counter to trip when timer up. In our case 1.25ms /
        Analog_Put(0, VOLTAGE_TO_RAW(5)); //FOR WHEN THE TIMER IS RUNNING output 5 volts on Channel 1 (via DAC).
        trippedTimer = true;
      }
    }
    if (trippedTimer) //Need to count down the counter, decrementing every 1.25m
    {
      counter--; //if timer has been set, count down the counter.
    }
    if (counter == 0 && trippedTimer) //TRIP: Set channel 1 to 5V as trip time completed
    {
      LEDs_Off(LED_BLUE); //show no longer timing
      LEDs_On(LED_GREEN); //show the circuit has been tripped
      ChannelsData.numberOfTrips++; //increment trip counter
      Analog_Put(0, 0); //set Channel 1 to 0V via the DAC to show no longer counting down on timer.
      Analog_Put(1, VOLTAGE_TO_RAW(5)); //set Channel 2 to 5V to show circuit has been tripped.
      trippedTimer = false;
      reset = true;
    }
    if (reset && (Sample[analogData->channelNb].iRMS < 1.03)) //if its time to reset, iRMS is at a save voltage, set to 0.
    {
      ResetDOR(); //run Reset DOR
      LEDs_Off(LED_GREEN); //show tripped circuit is now back to normal (no longer tripped)
      reset = false;
    }
    if ((Sample[analogData->channelNb].voltageSamples[i-1] > 0) && (Sample[analogData->channelNb].voltageSamples[i] < 0))
    {
      if (firstset)
      {
        Sample[analogData->channelNb].frequencysamples[0] = Sample[analogData->channelNb].voltageSamples[i-1];
        Sample[analogData->channelNb].frequencysamples[1] = Sample[analogData->channelNb].voltageSamples[i];
        firstset = false;
      }
      else {
        Sample[analogData->channelNb].frequencysamples[2] = Sample[analogData->channelNb].voltageSamples[i-1];
        Sample[analogData->channelNb].frequencysamples[3] = Sample[analogData->channelNb].voltageSamples[i];
        Frequency_Calculation(Sample, &ChannelsData, SAMPLE_TIME, loopcounter);
        loopcounter = 0;
        firstset = true;
      }
    }
    if (i>15)
    {
      i=1;
    }
    i++;
    loopcounter++;
    OS_EnableInterrupts(); //Enable Interrupts again

  }
}

/*! @brief Reset DOR Function
 * @note Designed to set the trip channel to 0V when circuit no longer tripped
 */
void ResetDOR() {
  Analog_Put(1, 0);
}

/*! @brief The Packet Handler Thread
 *
 *  @note - Loops until interrupted by thread of higher priority
 */
void PacketHandlerThread(void* pData)
{
  OS_SemaphoreWait(PacketHandlerSemaphore, 0); //Wait until triggered by Semaphore Signal
  StartupPackets();
  for(;;)
  {
    if (Packet_Get())
    {
      PacketHandler(); /*!<  When a complete packet is finally formed, handle the packet accordingly */
    }
  }
}

/*! @brief Thread Create for RTOS
 * Creates the threads.
 */
void ThreadsInit()
{
  while (OS_ThreadCreate(TowerInitThread, NULL, &TowerInitStack[THREAD_STACK_SIZE-1], TOWER_INIT_PRI) != OS_NO_ERROR); //Tower Initiation thread
  while (OS_ThreadCreate(UARTRXThread, NULL, &UARTRXStack[THREAD_STACK_SIZE-1], UART_RX_PRI) != OS_NO_ERROR); //UARTRX Thread
  while (OS_ThreadCreate(UARTTXThread, NULL, &UARTTXStack[THREAD_STACK_SIZE-1], UART_TX_PRI) != OS_NO_ERROR); //UARTTX Thread
  while (OS_ThreadCreate(PITThread, NULL, &PITStack[THREAD_STACK_SIZE-1], PIT_PRI) != OS_NO_ERROR); //PIT Thread
  // Create threads for 3 analog loopback channels
  for (uint8_t threadNb = 0; threadNb < NB_ANALOG_CHANNELS; threadNb++)
  {
    while (OS_ThreadCreate(AnalogLoopbackThread, &AnalogThreadData[threadNb], &AnalogThreadStacks[threadNb][THREAD_STACK_SIZE - 1], ANALOG_THREAD_PRIORITIES[threadNb]) != OS_NO_ERROR); //Create 3 analog threads for the 3 channels
  }
  while (OS_ThreadCreate(PacketHandlerThread, NULL, &PacketHandlerStack[THREAD_STACK_SIZE-1], PACKET_HANDLER_PRI) != OS_NO_ERROR); //Packet Handler Thread
}

/*! @brief Process the packet that has been received
 *
 *  @return bool - TRUE if the packet has been handled properly.
 *  @note Assumes that Packet_Init and Packet_Get was called
 */
bool PacketHandler(void)
{ /*!<  Packet Handler used after Packet Get */
  bool actionSuccess;  /*!<  Acknowledge is false as long as the package isn't acknowledge or if it's not required */
  switch(Packet_Command & ~PACKET_ACK_MASK)
  {
    case TOWER_STARTUP_COMMAND:
      actionSuccess = StartupPackets();
      break;

    case TOWER_VERSION_COMMAND:
      actionSuccess = VersionPackets();
      break;

    case TOWER_NUMBER_COMMAND:
      actionSuccess = TowerNumberPackets();
      break;

    case TOWER_MODE_COMMAND:
      actionSuccess = TowerModePackets();
      break;

    case DOR_INFORMATION_COMMAND:
      actionSuccess = DORInformationPackets();
      break;
  }
  if(Packet_Command & PACKET_ACK_MASK) /*!< if ACK bit is set, need to send back ACK packet if done successfully and NAK packet with bit7 cleared */
  {
    if(actionSuccess)
    {
      Packet_Put(Packet_Command, Packet_Parameter1, Packet_Parameter2, Packet_Parameter3);
    }
    else
    {
      Packet_Put((Packet_Command |=PACKET_ACK_MASK),Packet_Parameter1, Packet_Parameter2, Packet_Parameter3);
    }
  }
}

/*! @brief Send the packets needed on startup

 *  @return bool - TRUE if packet has been sent successfully
 *  @note Assumes that Packet_Init was called
 */
bool StartupPackets(void)
{
  if(Packet_Put(TOWER_STARTUP_COMMAND, TOWER_STARTUP_PARAMETER1, TOWER_STARTUP_PARAMETER2, TOWER_STARTUP_PARAMETER3))
  {
    if(Packet_Put(TOWER_VERSION_COMMAND, TOWER_VERSION_PARAMETER1, TOWER_VERSION_PARAMETER2, TOWER_VERSION_PARAMETER3))
    {
      if(Packet_Put(TOWER_NUMBER_COMMAND, TOWER_NUMBER_GET, TowerNumber->s.Lo, TowerNumber->s.Hi))
      {
        return Packet_Put(TOWER_MODE_COMMAND,TOWER_MODE_GET, TowerMode->s.Lo, TowerMode->s.Hi);
      }
    }
  }
}

/*! @brief Sends the packets related to DOR Information
 * Can send IDMT characteristic, number of faults, currents, frequency
 */
bool DORInformationPackets(void)
{
  if (Packet_Parameter1 == (uint8_t) 0) //Requested IDMT Characteristic
  {
    if (Packet_Parameter2 == (uint8_t) 0) //IDMT Get
    {
      return Packet_Put(DOR_INFORMATION_COMMAND,DOR_CHARACTERISTIC_PARAMETER1, DOR_CHARACTERISTIC_GET, ChannelsData.IDMTCharacteristic);
    }
    else if (Packet_Parameter2 == (uint8_t) 1) //IDMT Set
    {
      ChannelsData.IDMTCharacteristic = Packet_Parameter3;
     return Packet_Put(DOR_INFORMATION_COMMAND,DOR_CHARACTERISTIC_PARAMETER1, DOR_CHARACRERISTIC_SET, ChannelsData.IDMTCharacteristic);
    }
  }
  else if (Packet_Parameter1 == (uint8_t) 1) //Requested currents
  {
    int16union_t iRMS[3];
    for (uint8_t k=0; k<3; k++)
    {
      int16_t intvoltage = Sample[k].iRMS;
      float wholevoltage = (Sample[k].iRMS*100);
      iRMS[k].s.Lo = ((intvoltage*100)-wholevoltage);
      iRMS[k].s.Hi = intvoltage;
    }
    if (Packet_Put(DOR_CURRENT_COMMAND,DOR_PHASEA_PARAMETER1,iRMS[0].s.Lo, iRMS[0].s.Hi)) //PHASEA
    {
      if (Packet_Put(DOR_CURRENT_COMMAND,DOR_PHASEB_PARAMETER1,iRMS[1].s.Lo, iRMS[1].s.Hi)) //PHASEB
      {
        return Packet_Put(DOR_CURRENT_COMMAND,DOR_PHASEC_PARAMETER1,iRMS[2].s.Lo, iRMS[2].s.Hi); //PHASEC
      }
    }
  }
  else if (Packet_Parameter1 == (uint8_t) 2) //Requested frequency
  {
    return Packet_Put(DOR_INFORMATION_COMMAND,DOR_FREQUENCY_PARAMETER1, ChannelsData.frequency.s.Lo,  ChannelsData.frequency.s.Hi); //UNSURE IF . OR ->
  }
  else if (Packet_Parameter1 == (uint8_t) 3) //Requested number of times tripped
  {
    return Packet_Put(DOR_INFORMATION_COMMAND,DOR_TRIPPED_PARAMETER1, ChannelsData.numberOfTrips->s.Lo, ChannelsData.numberOfTrips->s.Hi);
  }
  else if (Packet_Parameter1 == (uint8_t) 4) //Requested fault type
  {
    return Packet_Put(DOR_INFORMATION_COMMAND,DOR_FAULT_PARAMETER1, ChannelsData.faultType, DOR_FAULT_PARAMETER3);
  }
}

/*! @brief Send the tower number packet to the PC
 *
 *  @return bool - TRUE if packet has been sent successfully
 *  @note Assumes that Packet_Init was called
 */
bool TowerNumberPackets(void)
{
  if(Packet_Parameter1 == (uint8_t) 1)
  {
    // if Parameter1 = 1 - get the tower number and send it to PC
    return Packet_Put(TOWER_NUMBER_COMMAND, TOWER_NUMBER_GET, TowerNumber->s.Lo, TowerNumber->s.Hi);
  }
  else if(Packet_Parameter1 == (uint8_t) 2) // if Parameter1 =2 - write new TowerNumber to Flash and send it to interface
  {
    uint16union_t newTowerNumber; /*! < create a union variable to combine the two Parameters*/
    newTowerNumber.s.Lo = Packet_Parameter2;
    newTowerNumber.s.Hi = Packet_Parameter3;
    //Flash_Write16((volatile uint16_t *) TowerNumber, newTowerNumber.l);
    return Packet_Put(TOWER_NUMBER_COMMAND, TOWER_NUMBER_SET, TowerNumber->s.Lo, TowerNumber->s.Hi);
  }
}

/*! @brief Send the tower mode packet to the PC
 *
 *  @return bool - TRUE if packet has been sent successfully
 *  @note Assumes that Packet_Init was called
 */
bool TowerModePackets(void)
{
  if(Packet_Parameter1 == 1) // if paramater1 = 1 - get the towermode and send it to PC
  {
    return Packet_Put(TOWER_MODE_COMMAND,TOWER_MODE_GET, TowerMode->s.Lo, TowerMode->s.Hi);
  }
  else if (Packet_Parameter1 == 2) // if parameter1 = 2 - set the towermode, write to Flash and send it back to PC
  {
    uint16union_t newTowerMode; /* !< Create a union variable to combine parameter2 and 3*/
    newTowerMode.s.Lo = Packet_Parameter2;
    newTowerMode.s.Hi = Packet_Parameter3;
    //Flash_Write16((volatile uint16_t *) TowerMode, newTowerMode.l);
    return Packet_Put(TOWER_MODE_COMMAND,TOWER_MODE_SET, TowerMode->s.Lo, TowerMode->s.Hi);
  }
  return false;
}

/*! @brief Send the version packet to the PC
 *
 *  @return bool - TRUE if packet has been sent successfully
 *  @note Assumes that Packet_Init was called
 */
bool VersionPackets(void)
{
  return Packet_Put(TOWER_VERSION_COMMAND,TOWER_VERSION_PARAMETER1,TOWER_VERSION_PARAMETER2, TOWER_VERSION_PARAMETER3);
}

/*! @brief Triggered during interrupt, signals the analog thread semaphores for all 3 channels
 *
 *  @return void
 *  @note Assumes that PIT_Init called
 */
void PITCallback(void)
{
  for (uint8_t analogNb = 0; analogNb < NB_ANALOG_CHANNELS; analogNb++) //Cycle through 3 channels (0-2) to signal 3 threads
  {
    OS_SemaphoreSignal(AnalogThreadData[analogNb].semaphore); //Signal Semaphore
  }
}

/*!
 ** @}
 */
