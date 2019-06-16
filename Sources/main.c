/* ###################################################################
**     Filename    : main.c
**     Project     : Digital Overcurrent Relay
**     Processor   : MK70FN1M0VMJ12
**     Version     : Driver 01.01
**     Compiler    : GNU C Compiler
**     Date/Time   : 2019-04-16, 10:23, # CodeGen: 0
**     Abstract    :
**         main module
**     Settings    :
**     Contents    :
**         public methods
**
** ###################################################################*/
/*!
** @file main.c
** @version 6.0
** @brief
**         main module.
**         This module is the main.
*/
/*!
**  @addtogroup main_module main module documentation
**  @{
*/
/*! @file
 *
 *  @brief Main on the TWR-K70F120M.
 *
 *  Is the Main Module.
 *
 *  @author Lucien Tran & Angus Ryan
 *  @date 2019-04-16
 */

/*!< Header Files */
#include "Cpu.h" /*!< CPU module - contains low level hardware initialization routines */
#include "OS.h"
#include "analog.h"

#include "Flash.h"
#include "packet.h"
#include "UART.h"
#include "LEDs.h"
#include "PIT.h"
#include "FTM.h"

/****************************************************************************************************************
 * Private function declaration
 ***************************************************************************************************************/
bool PacketHandler(void);
bool StartupPackets(void);
bool VersionPackets(void);
bool TowerNumberPackets(void);
bool TowerModePackets(void);
bool ProgramBytePackets(void);
bool ReadBytePackets(void);
bool TowerInit(void);
bool ClockInit(void);
bool TowerTimePackets(void);
bool ProtocolPackets(void);
void ThreadsInit(void);
void TowerInitThread(void* pData);
void PacketHandlerThread(void* pData);

/****************************************************************************************************************
 * Global variables and macro definitions
 ***************************************************************************************************************/
#define THREAD_STACK_SIZE 300 // Arbitrary thread stack size - big enough for stacking of interrupts and OS use.
#define NB_ANALOG_CHANNELS 2
const uint32_t BAUDRATE = 115200; /*!< Baud Rate specified in project */
const uint16_t STUDENT_ID = 0x1D6D; /*!< Student Number: 7533 */
const uint32_t PIT_Period = 1000000000; /*!< 1/1056Hz = 641025640 ns*/
const uint8_t ANALOG_THREAD_PRIORITIES[NB_ANALOG_CHANNELS] = {3, 4};
const uint8_t PACKET_ACK_MASK; /*!< Packet Acknowledgment mask, referring to bit 7 of the Packet */
static volatile uint16union_t *TowerNumber; /*!< declaring static TowerNumber Pointer */
static volatile uint16union_t *TowerMode; /*!< declaring static TowerMode Pointer */

/*! @brief Data structure used to pass Analog configuration to a user thread
 *
 */
typedef struct AnalogThreadData
{
  OS_ECB* semaphore;
  uint8_t channelNb;
} TAnalogThreadData;

OS_ECB* PacketHandlerSemaphore; //Declare a semaphore, to be signaled.

TFTMChannel FTMPacket =
{
  0, /*!< Channel being used */
  CPU_MCGFF_CLK_HZ_CONFIG_0, /*!< delay count: fixed frequency clock, mentioned in Timing and Generation Docs */
  TIMER_FUNCTION_OUTPUT_COMPARE, /*!< Brief specific: we are using OutputCompare*/
  TIMER_OUTPUT_LOW, /*!< Choose one functionality of output compare: low */
  NULL, /*!< Setting User Callback Function NOW UNUSED */
  (void*) 0, /*!< User callback arguments being passed  NOW UNUSED */
};

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
  }
};

typedef enum
{
  TOWER_INIT_PRI,
  UART_RX_PRI,
  UART_TX_PRI,
  ANALOG_CHANNEL_1,
  ANALOG_CHANNEL_2,
  PIT_PRI,
  FTM_PRI,
  PACKET_HANDLER_PRI
}TPRIORITIES;

static uint32_t AnalogThreadStacks[NB_ANALOG_CHANNELS][THREAD_STACK_SIZE] __attribute__ ((aligned(0x08)));
OS_THREAD_STACK(TowerInitStack, THREAD_STACK_SIZE);
OS_THREAD_STACK(FTMStack, THREAD_STACK_SIZE);
OS_THREAD_STACK(PITStack, THREAD_STACK_SIZE);
OS_THREAD_STACK(UARTRXStack, THREAD_STACK_SIZE);
OS_THREAD_STACK(UARTTXStack, THREAD_STACK_SIZE);
OS_THREAD_STACK(PacketHandlerStack, THREAD_STACK_SIZE);

void LPTMRInit(const uint16_t count)
{
  // Enable clock gate to LPTMR module
  SIM_SCGC5 |= SIM_SCGC5_LPTIMER_MASK;

  // Disable the LPTMR while we set up
  // This also clears the CSR[TCF] bit which indicates a pending interrupt
  LPTMR0_CSR &= ~LPTMR_CSR_TEN_MASK;

  // Enable LPTMR interrupts
  LPTMR0_CSR |= LPTMR_CSR_TIE_MASK;
  // Reset the LPTMR free running counter whenever the 'counter' equals 'compare'
  LPTMR0_CSR &= ~LPTMR_CSR_TFC_MASK;
  // Set the LPTMR as a timer rather than a counter
  LPTMR0_CSR &= ~LPTMR_CSR_TMS_MASK;

  // Bypass the prescaler
  LPTMR0_PSR |= LPTMR_PSR_PBYP_MASK;
  // Select the prescaler clock source
  LPTMR0_PSR = (LPTMR0_PSR & ~LPTMR_PSR_PCS(0x3)) | LPTMR_PSR_PCS(1);

  // Set compare value
  LPTMR0_CMR = LPTMR_CMR_COMPARE(count);

  // Initialize NVIC
  // see p. 91 of K70P256M150SF3RM.pdf
  // Vector 0x65=101, IRQ=85
  // NVIC non-IPR=2 IPR=21
  // Clear any pending interrupts on LPTMR
  NVICICPR2 = NVIC_ICPR_CLRPEND(1 << 21);
  // Enable interrupts from LPTMR module
  NVICISER2 = NVIC_ISER_SETENA(1 << 21);

  //Turn on LPTMR and start counting
  LPTMR0_CSR |= LPTMR_CSR_TEN_MASK;
}

void __attribute__ ((interrupt)) LPTimer_ISR(void)
{
  // Clear interrupt flag
  LPTMR0_CSR |= LPTMR_CSR_TCF_MASK;

  // Signal the analog channels to take a sample
  for (uint8_t analogNb = 0; analogNb < NB_ANALOG_CHANNELS; analogNb++)
    (void)OS_SemaphoreSignal(AnalogThreadData[analogNb].semaphore);
}

/*! @brief Samples a value on an ADC channel and sends it to the corresponding DAC channel.
 *
 */
void AnalogLoopbackThread(void* pData)
{
  // Make the code easier to read by giving a name to the typecast'ed pointer
  #define analogData ((TAnalogThreadData*)pData)

  for (;;)
  {
    int16_t analogInputValue;

    (void)OS_SemaphoreWait(analogData->semaphore, 0);
    // Get analog sample
    Analog_Get(analogData->channelNb, &analogInputValue);
    // Put analog sample
    Analog_Put(analogData->channelNb, analogInputValue);
  }
}

/*lint -save  -e970 Disable MISRA rule (6.3) checking. */
int main(void)
/*lint -restore Enable MISRA rule (6.3) checking. */
{
 // OS_DisableInterrupts(); /*!< Disable the interrupts using OS library */
  // Initialise low-level clocks etc using Processor Expert code
  PE_low_level_init();
  // Initialize the RTOS
  OS_Init(CPU_CORE_CLK_HZ, true);
  ThreadsInit(); /*!< Creates the Threads */
  PacketHandlerSemaphore = OS_SemaphoreCreate(0);
 // OS_EnableInterrupts(); /*!< Re-enabling interrupts after initiating  the adequate modules*/
  // Start multithreading - never returns!
  OS_Start();
}

void ThreadsInit()
{
  while (OS_ThreadCreate(TowerInitThread, NULL, &TowerInitStack[THREAD_STACK_SIZE-1], TOWER_INIT_PRI) != OS_NO_ERROR); //Tower Initiation thread
  while (OS_ThreadCreate(FTMThread, NULL, &FTMStack[THREAD_STACK_SIZE-1], FTM_PRI) != OS_NO_ERROR); //FTM Thread
  while (OS_ThreadCreate(PITThread, NULL, &PITStack[THREAD_STACK_SIZE-1], PIT_PRI) != OS_NO_ERROR); //PIT Thread
  while (OS_ThreadCreate(PacketHandlerThread, NULL, &PacketHandlerStack[THREAD_STACK_SIZE-1], PACKET_HANDLER_PRI) != OS_NO_ERROR); //Packet Handler Thread
  while (OS_ThreadCreate(UARTRXThread, NULL, &UARTRXStack[THREAD_STACK_SIZE-1], UART_RX_PRI) != OS_NO_ERROR); //UARTRX Thread
  while (OS_ThreadCreate(UARTTXThread, NULL, &UARTTXStack[THREAD_STACK_SIZE-1], UART_TX_PRI) != OS_NO_ERROR); //UARTTX Thread
  // Create threads for 2 analog loopback channels
  for (uint8_t threadNb = 0; threadNb < NB_ANALOG_CHANNELS; threadNb++)
  {
    while (OS_ThreadCreate(AnalogLoopbackThread, &AnalogThreadData[threadNb], &AnalogThreadStacks[threadNb][THREAD_STACK_SIZE - 1], ANALOG_THREAD_PRIORITIES[threadNb]) != OS_NO_ERROR);
  }
}

/*! @brief The Tower Initiation Thread
 *
 *  @note - Deletes upon completion of tower initiation
 *  @return enum - relating to error code
 */
void TowerInitThread(void* pData)
{

    OS_DisableInterrupts();
    (void)Analog_Init(CPU_BUS_CLK_HZ);
    // Generate the global analog semaphores
    for (uint8_t analogNb = 0; analogNb < NB_ANALOG_CHANNELS; analogNb++)
      AnalogThreadData[analogNb].semaphore = OS_SemaphoreCreate(0);
    LPTMRInit(10);
    // Initialise the low power timer to tick every 10 ms
    Flash_Init();
    /*!<  Allocate var for both Tower Number and Mode, if succcessful, FlashWrite16 them with the right values */
    bool towerModeInit = Flash_AllocateVar( (volatile void **) &TowerMode, sizeof(*TowerMode));
    bool towerNumberInit = Flash_AllocateVar((volatile void **) &TowerNumber, sizeof(*TowerNumber));
    if (towerModeInit && towerNumberInit && LEDs_Init())
    {
      if (TowerMode->l == 0xffff) /* when unprogrammed, value = 0xffff, announces in hint*/
      {
        Flash_Write16((volatile uint16_t *) TowerMode, 0x1); /*!< Parsing through the function: typecast volatile uint16_t pointer from uint16union_t pointer, and default towerMode = 1 */
      }
      if (TowerNumber->l == 0xffff) /* when unprogrammed, value = 0xffff, announces in hint*/
      {
        Flash_Write16((volatile uint16_t *) TowerNumber, STUDENT_ID); /*Like above, but with towerNumber set to our student ID = 7533*/
      }
      if (PIT_Init(CPU_CORE_CLK_HZ,NULL, NULL) && (Packet_Init(BAUDRATE, CPU_CORE_CLK_HZ)) && FTM_Init())
      { /*!< Initiate all required modules */
        PIT_Set(PIT_Period, true);
      }
    }
    FTM_Set(&FTMPacket); /*!< configure FTM0 functionality, passing in the declared struct address containing values at top of file */

    OS_EnableInterrupts();
    while(OS_SemaphoreSignal(PacketHandlerSemaphore) != OS_NO_ERROR);
    // Analog
    OS_ThreadDelete(TOWER_INIT_PRI);
}

/*! @brief The Packet Handler Thread
 *
 *  @note - Loops until interrupted by thread of higher priority
 */
void PacketHandlerThread(void* pData)
{

  OS_SemaphoreWait(PacketHandlerSemaphore, 0); //Wait until triggered by Semaphore Signal
  StartupPackets(); /*!< Sends Packets from Device to PC on Startup. */
  for(;;)
  {
    if (Packet_Get())
    {
      FTM_StartTimer(&FTMPacket); /*!< Start timer, calling interrupt User function (FTM0Callback) once completed.  */
      LEDs_On(LED_BLUE);
      PacketHandler(); /*!<  When a complete packet is finally formed, handle the packet accordingly */
    }
  }
}

/*! @brief Send the packets needed on startup

 *  @return bool - TRUE if packet has been sent successfully
 *  @note Assumes that Packet_Init was called
 */
bool StartupPackets(void)
{
  if (Packet_Put(TOWER_STARTUP_COMMAND, TOWER_STARTUP_PARAMETER1, TOWER_STARTUP_PARAMETER2, TOWER_STARTUP_PARAMETER3))
  {
    if (Packet_Put(TOWER_VERSION_COMMAND, TOWER_VERSION_PARAMETER1, TOWER_VERSION_PARAMETER2, TOWER_VERSION_PARAMETER3))
    {
      if (Packet_Put(TOWER_NUMBER_COMMAND, TOWER_NUMBER_PARAMETER1, TowerNumber->s.Lo, TowerNumber->s.Hi))
      {
        return (Packet_Put(TOWER_MODE_COMMAND,TOWER_MODE_GET, TowerMode->s.Lo, TowerMode->s.Hi));
      }
    }
  }
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

/*! @brief Send the tower number packet to the PC
 *
 *  @return bool - TRUE if packet has been sent successfully
 *  @note Assumes that Packet_Init was called
 */
bool TowerNumberPackets(void)
{
  if (Packet_Parameter1 == (uint8_t) 1)
  {
    // if Parameter1 = 1 - get the tower number and send it to PC
    return Packet_Put(TOWER_NUMBER_COMMAND, TOWER_NUMBER_GET, TowerNumber->s.Lo, TowerNumber->s.Hi);
  }
  else if (Packet_Parameter1 == (uint8_t) 2) // if Parameter1 =2 - write new TowerNumber to Flash and send it to interface
  {
    uint16union_t newTowerNumber; /*! < create a union variable to combine the two Parameters*/
    newTowerNumber.s.Lo = Packet_Parameter2;
    newTowerNumber.s.Hi = Packet_Parameter3;
    Flash_Write16((volatile uint16_t *) TowerNumber, newTowerNumber.l);
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
  if (Packet_Parameter1 == 1) // if paramater1 = 1 - get the towermode and send it to PC
  {
    return Packet_Put(TOWER_MODE_COMMAND,TOWER_MODE_GET, TowerMode->s.Lo, TowerMode->s.Hi);
  }
  else if (Packet_Parameter1 == 2) // if parameter1 = 2 - set the towermode, write to Flash and send it back to PC
  {
    uint16union_t newTowerMode; /* !< Create a union variable to combine parameter2 and 3*/
    newTowerMode.s.Lo = Packet_Parameter2;
    newTowerMode.s.Hi = Packet_Parameter3;
    Flash_Write16((volatile uint16_t *) TowerMode, newTowerMode.l);
    return Packet_Put(TOWER_MODE_COMMAND,TOWER_MODE_SET, TowerMode->s.Lo, TowerMode->s.Hi);
  }
  return false;
}

/*! @brief Handles the packet to program bytes in FLASH
 *
 *  @return bool - TRUE if packet has been sent and handled successfully
 *  @note Assumes that Packet_Init was called
 */
bool ProgramBytePackets(void)
{
  if (Packet_Parameter1 == 8)
  {
    return Flash_Erase(); /*! < if Parameter1  = 8 - erase the whole sector */
  }
  else if (Packet_Parameter1 > 8)
  {
    return false; //data sent is obsolete
  }
  else /*!< if offset (Parameter1) is between 0 and 7 inclusive, check the offset */
  {
    volatile uint8_t *address = (uint8_t *)(FLASH_DATA_START + Packet_Parameter1);
    return Flash_Write8(address, Packet_Parameter3); //Write in the Flash
  }
  return false;
}

/*! @brief Handles the packet to read bytes from FLASH
 *
 *  @return bool - TRUE if packet has been sent and handled successfully
 *  @note Assumes that Packet_Init was called
 */
bool ReadBytePackets(void)
{
  uint8_t readByte = _FB(FLASH_DATA_START + Packet_Parameter1); /* !< fetching the Byte at offset Parameter1 and send it to PCc*/
  return Packet_Put(FLASH_READ_COMMAND, Packet_Parameter1, 0x0, readByte);
}

/*! @brief Handles the packet RTC time - sends back ther packet to PC if setting time is successful
 *
 *  @return bool - TRUE if packet has been sent and handled successfully
 *  @note Assumes that Packet_Init and RTC_Init was called
 */
bool TowerTimePackets(void)
{
  /*!< Checking if input is valid, if not, return false */
  if (Packet_Parameter1 <= 23)
  {
    if (Packet_Parameter2 <= 59)
    {
      if (Packet_Parameter3 <= 59)
      {
	/*!< sets the time with packet parameters given by PC */

        /*!< returns the original packet to the PC if successful */
        return Packet_Put(SET_TIME_COMMAND, Packet_Parameter1, Packet_Parameter2, Packet_Parameter3);
      }
    }
  }
  return false;
}

/*! @brief Process the packet that has been received
 *
 *  @return bool - TRUE if the packet has been handled properly.
 *  @note Assumes that Packet_Init and Packet_Get was called
 */
bool PacketHandler(void)
{
  /*!<  Packet Handler used if Packet Get is successful*/
  bool actionSuccess;  /*!<  Acknowledge is false as long as the package isn't acknowledge or if it's not required */
  switch (Packet_Command & ~PACKET_ACK_MASK)
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

    case FLASH_PROGRAM_COMMAND:
      actionSuccess = ProgramBytePackets();
      break;

    case FLASH_READ_COMMAND:
      actionSuccess = ReadBytePackets();
      break;

    case SET_TIME_COMMAND:
      actionSuccess = TowerTimePackets();
      break;


  }

  if (Packet_Command & PACKET_ACK_MASK) /*!< if ACK bit is set, need to send back ACK packet if done successfully and NAK packet with bit7 cleared */
  {
    if (actionSuccess)
    {
      Packet_Put(Packet_Command, Packet_Parameter1, Packet_Parameter2, Packet_Parameter3);
    }
    else
    {
      Packet_Put((Packet_Command |=PACKET_ACK_MASK),Packet_Parameter1, Packet_Parameter2, Packet_Parameter3);
    }
  }
}





/*
** ###################################################################
**
**     This file was created by Processor Expert 10.5 [05.21]
**     for the Freescale Kinetis series of microcontrollers.
**
** ###################################################################
*/
/*!
* @}
*/

