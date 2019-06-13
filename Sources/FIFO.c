/*! @file
 *
 *  @brief Routines to access the FIFO on the TWR-K70F120M.
 *
 *  This contains the functions for operating the FIFO.
 *
 *  @author Lucien Tran & Angus Ryan
 *  @date 2019-04-16
 */
/*!
**  @addtogroup FIFO_module FIFO module documentation
**  @{
*/

#include "FIFO.h"
#include "Cpu.h"
#include "OS.h"


#define FIFO_SIZE 256 /*!<  Number of bytes in a FIFO */


bool FIFO_Init(TFIFO * const fifo) /*!<  Initiate the FIFO */
{
  OS_DisableInterrupts();
  fifo->Start = fifo->End = 0; /*!<  Make both head and tails of the FIFO = 0, therefore empty */
  fifo->NbBytes = 0; /*!< Number of the bytes taken by the Buffer is equal to 0 when it is empty */
  fifo->EmptyFIFOSemaphore = OS_SemaphoreCreate(0);
  fifo->FullFIFOSemaphore = OS_SemaphoreCreate(0);
  OS_EnableInterrupts();
  return true;
}

bool FIFO_Put(TFIFO * const fifo, const uint8_t data)
{
  OS_DisableInterrupts();
  if (fifo->NbBytes == FIFO_SIZE) /*!<  If FIFO is full */
  {
    OS_SemaphoreWait(fifo->FullFIFOSemaphore, 0);
  } /*!<  FIFO was full, so deleting oldest data to replace with new one - will lose data every time FIFO-Put is called and it is full to avoid CPU interrupt error */
  else
  {
    fifo->Buffer[fifo->End] = data; /*!<  Storing data in the new element of the array */
    fifo->End++; /*!< Create a potential new element in the Buffer Array to store the data */
    if (fifo->End == FIFO_SIZE)
    {
      fifo->End = 0; /*!<  Check for wrap around */
    }
    fifo->NbBytes++; /*!< Increase the number of bytes = new data is written - to keep track of the number of bytes stored */
    OS_SemaphoreSignal(fifo->EmptyFIFOSemaphore);
    OS_EnableInterrupts();
    return true; /*!< Successful */
  }
}


bool FIFO_Get(TFIFO * const fifo, uint8_t * const dataPtr)
{
  OS_DisableInterrupts();
  if (fifo->NbBytes == 0) /*!< Empty if PutPt == GetPt */
  {
      OS_SemaphoreWait(fifo->EmptyFIFOSemaphore, 0);
  }
  else
  {
    *dataPtr = fifo->Buffer[fifo->Start]; /*!< value at the address is equal to the character */
    fifo->Start++;
    if (fifo->Start == FIFO_SIZE)
    {
      fifo->Start = 0; /*!<  Check for wrap around */
    }
    fifo->NbBytes--; /*!< Getting the value is getting rid of the oldest value - Number of bytes decreases */
    OS_SemaphoreSignal(fifo->FullFIFOSemaphore);
    OS_EnableInterrupts();
    return true; /*!<  Successful */
  }
}

/*!
* @}
*/
