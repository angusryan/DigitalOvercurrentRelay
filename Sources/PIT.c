/*! @file
 *
 *  @brief Routines for controlling Periodic Interrupt Timer (PIT) on the TWR-K70F120M.
 *
 *  This contains the functions for operating the periodic interrupt timer (PIT).
 *
 *  @author Lucien Tran & Angus Ryan
 *  @date 2019-05-02
 */

/*!
 *  @addtogroup PIT_module PIT module documentation
 *  @{
 */

#include "types.h"
#include "PIT.h"
#include "MK70F12.h"
#include "LEDs.h"
#include "OS.h"
#include "packet.h"

static uint32_t PIT_ModuleClk;
static void *PITArguments;
static void (*PITCallback)(void* PITArguments);

OS_ECB* PITSemaphore; //Declare Semaphore

bool PIT_Init(const uint32_t moduleClk, void (*userFunction)(void*), void* userArguments)
{
  PITCallback = userFunction; /*!< Make the user function equal to PIT callback and accessible as global variable*/
  PITArguments = userArguments; /*!< Make the user Argument a global variable*/
  PIT_ModuleClk = moduleClk; /*!< Make the module clock a global variable to access in PIT_Set*/

  SIM_SCGC6 |= SIM_SCGC6_PIT_MASK; /*!< Enable PIT clock gate p356*/

  PIT_MCR &= ~PIT_MCR_MDIS_MASK; /*!< Bit is 1 by default. Needs to be cleared to enable setup of PIT*/
  PIT_MCR |= PIT_MCR_FRZ_MASK; /*!< Bit Freeze is enabled - the counter is stopped during debug */
  /*!< using Timer Control Register 0  - Channel 0-3 but choosing 0*/
  PIT_TCTRL0 |= PIT_TCTRL_TIE_MASK; /*!< Enabling Interrupt TIE bit - p 1343 - Interrupt will be requested whenever Timer Interrupt flag is up*/
  /*!< IRQ RTC seconds = 68
   * 67%32 = 4 */
  /*!< Using NVICICPR2 & NVICISER2 from table and example in interrupt*/
  NVICICPR2 = (1 << 4);   /*!< Clear any pending interrupts with NVIC */
  NVICISER2 = (1 << 4);  /*!< Enable interrupts with NVIC */

  PITSemaphore = OS_SemaphoreCreate(0); //Create a Semaphore
  return true;

}

void PIT_Set(const uint32_t period, const bool restart)
{
  if(restart) /*!< if restart == TRUE, restart the timer is disabled, then, new value, then enabled*/
  {
    PIT_Enable(false);
  }
  // Read p 1345 of reference manual - equation found in example of PIT init
  /*!< clock period = 1/ PIT_module clk as it has it to be in terms of seconds*/
  uint32_t clockPeriod = 1000000000/PIT_ModuleClk; /*!< 1/25000000=40 in decimal = 0x28 in hexa*/
  uint32_t triggerLDVAL = (period/clockPeriod) -1;
  PIT_LDVAL0 = triggerLDVAL; /*!< Assign the trigger value at register0. Will assign a number to count down from*/
  if(restart) /*!< enabled if restart =true */
  {
    PIT_Enable(true);
  }
}

void PIT_Enable(const bool enable)
{
  if(enable)
  {
    PIT_TCTRL0 |= PIT_TCTRL_TEN_MASK; /*!< Enable PIT Timer at control register 0*/
  }
  else
  {
    PIT_TCTRL0 &= ~PIT_TCTRL_TEN_MASK; /*!< disable PIT Time at control Register 0*/
  }
}

void __attribute__ ((interrupt)) PIT_ISR(void)
{
  OS_ISREnter(); //Enter Interrupt
  /* Interrupt needs to be cleared at every ISR*/
  /*  !< Using Timer Flag Register 0 */
  PIT_TFLG0 |= PIT_TFLG_TIF_MASK; /*!< Clearing Timer Interrupt Flag after it is raised by writing 1 to it - p1344*/
  while(OS_SemaphoreSignal(PITSemaphore) != OS_NO_ERROR); //Signal I2C Semaphore (triggering I2C thread) and ensure it returns no error
  OS_ISRExit(); //Exit Interrupt
}

void PITThread(void* pData)
{

  for(;;)
  {
    OS_SemaphoreWait(PITSemaphore, 0);
    LEDs_Toggle(LED_GREEN);
  }
// handles PIT packets
}

/*!
* @}
*/
