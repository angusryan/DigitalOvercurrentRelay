/*! @file
 *
 *  @brief Routines to access the LEDs on the TWR-K70F120M.
 *
 *  This contains the functions for operating the LEDs.
 *
 *  @author Lucien Tran & Angus Ryan
 *  @date 2019-04-16
 */

/*!
**  @addtogroup LEDs_module LEDs module documentation
**  @{
*/

#ifndef LEDS_H
#define LEDS_H

// new types
#include "types.h"
#include "MK70F12.h"

typedef enum
{
  LED_ORANGE = (1 << 11),
  LED_YELLOW = (1 << 28),
  LED_GREEN = (1 << 29),
  LED_BLUE = (1 << 10)
} TLED;

bool LEDs_Init(void)
{
  SIM_SCGC5 |= SIM_SCGC5_PORTA_MASK; /*!< Enables PORTA (bit 9) located in System Clock Gating Control Register 5 (pg. 347). */
  PORTA_PCR11 = PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK; /*!< Setting the MUX bits (8, 9, 10) in the PORTA_PCR 11,28,29,10 register to high. Also Enabled Drive Strength (pg. 316,317) */
  PORTA_PCR28 = PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK;
  PORTA_PCR29 = PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK;
  PORTA_PCR10 = PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK;

  GPIOA_PDDR = GPIO_PDDR_PDD(LED_ORANGE) | GPIO_PDDR_PDD(LED_GREEN) | GPIO_PDDR_PDD(LED_BLUE) | GPIO_PDDR_PDD(LED_YELLOW); /*!< Port Data Direction Register, setting LEDs as outputs (pg. 2153) */
  GPIOA_PSOR |= GPIO_PSOR_PTSO(LED_GREEN) | GPIO_PSOR_PTSO(LED_YELLOW) | GPIO_PSOR_PTSO(LED_BLUE) | GPIO_PSOR_PTSO(LED_ORANGE); /*!< Turning off LEDs */
}

void LEDs_On(const TLED color)
{
  GPIOA_PCOR |=  GPIO_PCOR_PTCO(color); /*!< Port Clear Output Register, setting passed color off (pg. 2152) */
}


void LEDs_Off(const TLED color)
{
  GPIOA_PSOR |= GPIO_PSOR_PTSO(color); /*!< Port Set Output Register, setting passed color off (pg. 2151) */
}


void LEDs_Toggle(const TLED color)
{
  GPIOA_PTOR |= GPIO_PTOR_PTTO(color); /*!< Port Toggle Output Register, setting passed color off/on depending on previous state (pg. 2152) */
}

/*!
* @}
*/
#endif
