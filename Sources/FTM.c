/*! @file
 *
 *  @brief Routines to access the FTM on the TWR-K70F120M.
 *
 *  This contains the functions for operating the FTM.
 *
 *  @author Lucien Tran & Angus Ryan
 *  @date 2019-05-06
 */
/*!
**  @addtogroup FTM_module FTM module documentation
**  @{
*/


#include "FTM.h"
#include "MK70F12.h"
#include "PE_Types.h"
#include "OS.h"
#include "LEDs.h"




#define NUMBER_OF_CHANNELS 7 /*!< defining the number of channels available (0-7)*/
#define FIXED_FREQUENCY_CLOCK 2 /*!< assign the fixed frequency clock value to be set in FTMx_SC register (pg. 1210) */

void* FTM0_Arguments[NUMBER_OF_CHANNELS]; /*!< creating a pointer of type void, to store an arguement, in an array the size of number of channels */
void (*FTM0_Callback[NUMBER_OF_CHANNELS])(void *); /*!< Callback function*/

OS_ECB* FTMSemaphore;

bool FTM_Init()
{

  SIM_SCGC6 |= SIM_SCGC6_FTM0_MASK; /*!< Initiate FTM0 clock gate control (Bit 24) (pg. 349) */
  uint8_t irq = (62 % 32); /*!< variable to calculate bit location in NVIC registers (pg. 92).*/

  //Written in this particular order as per the hints (L3.4)
  FTM0_CNTIN = ~FTM_CNTIN_INIT_MASK; /*!< Initiate CNTIN value, settings all bits to 0 */
  FTM0_MOD |= FTM_MOD_MOD_MASK; /*!< Modulo Set, to cater for overflow */
  FTM0_CNT = ~FTM_CNT_COUNT_MASK; /*!< Setting counter value to FTM register for the count */
  FTM0_SC |= FTM_SC_CLKS(FIXED_FREQUENCY_CLOCK); /*!< Assigned fixed frequency clock (ref. Lab L3.2 #6) to CLK (bits 3 & 4) in status and control register (ref. Lab L3.4 Hints #1)*/

  /*!< Address = 0x4E = 138, Vector = 78, IRQ = 62. NVIC Non-IPR register = 1, NVIC IPR Register 15. (pg. 90) */
  NVICICPR1 = (1 << irq);  /*!< Clear any pending interrupts on FTM0 referenced from Interrupts Document (5.16) */
  NVICISER1 = (1 << irq);  /*!< Enable interrupts from FTM0 module referenced from Interrupts Document (5.16) */

  FTMSemaphore = OS_SemaphoreCreate(0);

  return true;

}

bool FTM_Set(const TFTMChannel* const aFTMChannel)
{

  if ((aFTMChannel->timerFunction == TIMER_FUNCTION_INPUT_CAPTURE) && (aFTMChannel->channelNb <= NUMBER_OF_CHANNELS))
  {
    FTM0_CnSC(aFTMChannel->channelNb) &= ~(FTM_CnSC_MSA_MASK | FTM_CnSC_MSB_MASK); /*!< Timer Function Output Compare sets MSA as 0 and MSB as 0 (pg. 1219)*/
    switch (aFTMChannel->ioType.inputDetection) /*!< Check value of inputDection for case statement */
    {
      case (TIMER_INPUT_OFF):
        FTM0_CnSC(aFTMChannel->channelNb) &= ~(FTM_CnSC_ELSA_MASK  | FTM_CnSC_ELSB_MASK ); /*!< 00 Off Configuration (Revert Channel Pin to GPIO)(pg. 1219) */
	break;

      case (TIMER_INPUT_RISING):
        FTM0_CnSC(aFTMChannel->channelNb) |= ((FTM_CnSC_ELSA_MASK) & ~(FTM_CnSC_ELSB_MASK)); /*!< 01 Rising Configuration (pg. 1219) */
	break;

      case (TIMER_INPUT_FALLING):
        FTM0_CnSC(aFTMChannel->channelNb) |= (~(FTM_CnSC_ELSA_MASK) & (FTM_CnSC_ELSB_MASK)); /*!< 10 Falling Configuration (pg. 1219) */
	break;

      case (TIMER_INPUT_ANY):
        FTM0_CnSC(aFTMChannel->channelNb) |= (FTM_CnSC_ELSA_MASK | FTM_CnSC_ELSB_MASK ); /*!< 11 Rising or Falling Configuration (pg. 1219) */
	break;

      default:
	return false; /*!< None of the cases specified, return false */
	break;
    }
  }

  else if ((aFTMChannel->timerFunction == TIMER_FUNCTION_OUTPUT_COMPARE) && (aFTMChannel->channelNb <= NUMBER_OF_CHANNELS))
  {
    FTM0_CnSC(aFTMChannel->channelNb) |= (FTM_CnSC_MSA_MASK) & ~(FTM_CnSC_MSB_MASK); /*!<  Timer Function Output Compare sets MSA as 1 and MSB as 0 (pg. 1219)*/
    switch (aFTMChannel->ioType.outputAction) /*!< Check value of outputAction for case statement */
    {
      case (TIMER_OUTPUT_DISCONNECT):
        FTM0_CnSC(aFTMChannel->channelNb) &= ~(FTM_CnSC_ELSA_MASK | FTM_CnSC_ELSB_MASK ); /*!< 00 Disconnect Configuration (Revert Channel Pin to GPIO)(pg. 1219) */
	break;

      case (TIMER_OUTPUT_TOGGLE):
        FTM0_CnSC(aFTMChannel->channelNb) |= ((FTM_CnSC_ELSA_MASK) & ~(FTM_CnSC_ELSB_MASK)); /*!<  01 Toggle Configuration (pg. 1219) */
	break;

      case (TIMER_OUTPUT_LOW):
        FTM0_CnSC(aFTMChannel->channelNb) |= (~(FTM_CnSC_ELSA_MASK) & (FTM_CnSC_ELSB_MASK));  /*!<  10 Low Configuration (pg. 1219) */
	break;

      case (TIMER_OUTPUT_HIGH):
        FTM0_CnSC(aFTMChannel->channelNb) |= (FTM_CnSC_ELSA_MASK | FTM_CnSC_ELSB_MASK); /*!< 11 High Configuration (pg. 1219) */
	break;

      default:
	return false; /*!< None of the cases specified, return false */
        break;
    }
  }

  else
  {
    return false; /*!< If not output compare or input capture then unexpected value, return false */
  }


  FTM0_Callback[aFTMChannel->channelNb] = aFTMChannel->callbackFunction;
  FTM0_Arguments[aFTMChannel->channelNb] = aFTMChannel->callbackArguments;
  return true;
}



bool FTM_StartTimer(const TFTMChannel* const aFTMChannel)
{

  if ((aFTMChannel->timerFunction == TIMER_FUNCTION_OUTPUT_COMPARE) && (aFTMChannel->channelNb <= NUMBER_OF_CHANNELS)) /*!< If Timer function set to output compare & set channel within range of channels (0-7)  */
  {
    FTM0_CnV(aFTMChannel->channelNb) = (FTM0_CNT + (aFTMChannel->delayCount)); /*!< Set the 16-bit output compare register to CNT + Delay into current assigned channel, catering for overflow (ref. Timing and Generation 6.5) */
    FTM0_CnSC(aFTMChannel->channelNb) &= ~FTM_CnSC_CHF_MASK; /*!< Clear the output compare flag (ref. Timing and Generation 6.5) */
    FTM0_CnSC(aFTMChannel->channelNb) |= FTM_CnSC_CHIE_MASK; /*!< Enable channel interrupts*/
    return true;
  }
  return false; /*!< Brief specifies only output compare to be used */
}



void __attribute__ ((interrupt)) FTM0_ISR(void)
{
  OS_ISREnter();
  uint8_t channel; /*!< declare local variable channel, used in for loop */
  for(channel=0;channel<=NUMBER_OF_CHANNELS; channel++) /*!< scroll through the channels to check if which interrupt flag enabled */
  {
    if ((FTM0_CnSC(channel) & FTM_CnSC_CHF_MASK)) /*!< Check current channel status and control register to see if current channel flag raised */
    {
      while (OS_SemaphoreSignal(FTMSemaphore) != OS_NO_ERROR);  //(*FTM0_Callback[channel])(FTM0_Arguments[channel]); /*!< Run the User Callback Function for channel with raised flag (in main, turning off Blue LED as requested in Lab 3 Hints) */
    }
    FTM0_CnSC(channel) &= ~FTM_CnSC_CHF_MASK; /*!< clear output flag */
  }
  OS_ISRExit();
}


void FTMThread(void* pData)
{
  for(;;)
  {
    OS_SemaphoreWait(FTMSemaphore, 0);
    LEDs_Off(LED_BLUE);
  }
}
/*!
* @}
*/
