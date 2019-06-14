/*! @file
 *
 *  @brief Routines to access the Flash on the TWR-K70F120M.
 *
 *  This contains the functions for operating the Flash.
 *
 *  @author Lucien Tran & Angus Ryan
 *  @date 2019-04-16
 */

/*!
**  @addtogroup Flash_module Flash module documentation
**  @{
*/

#include "Flash.h"
#include "types.h"
#include "MK70F12.h"
#include "packet.h"


typedef struct /*!< struct containing command, databytes and address required for Flash Common Command Object Registers */
{
 uint8_t FCCOB_Command;
 uint8_t dataByte0;
 uint8_t dataByte1;
 uint8_t dataByte2;
 uint8_t dataByte3;
 uint8_t dataByte4;
 uint8_t dataByte5;
 uint8_t dataByte6;
 uint8_t dataByte7;
 uint8_t address0;
 uint8_t address1;
 uint8_t address2;
} TFCCOB;


/****************************************************************************************************************
 * Private function declaration
 ***************************************************************************************************************/
static bool LaunchCommand(TFCCOB* commonCommandObject);
static bool WritePhrase(const uint32_t address, const uint64union_t phrase);
static bool EraseSector(const uint32_t address);
static bool ModifyPhrase(const uint32_t address, const uint64union_t phrase);

/****************************************************************************************************************
 * Private Macro Definitions
 ***************************************************************************************************************/
#define FCMD_WRITE 0x07 /* !< Command number to write a phrase */
#define FCMD_ERASE 0x09 /* !< Command number to bulk erase a sector */

bool Flash_Init(void)
{
//  not needed
//  SIM_SCGC3 |= SIM_SCGC3_NFC_MASK; /*!< enable NAND Flash Controller (bit 8) within system clock gate 3 register. (pg. 136, 343-344, 863) */
  while (!(FTFE_FSTAT & FTFE_FSTAT_CCIF_MASK)); /*!< Waiting for CCIF */
  return true;
}

bool Flash_AllocateVar(volatile void** variable, const uint8_t size)
{
  static uint8_t byteAlloc = 0xFF; /*!< 1111 1111 => means that it is empty */
  int addressOffset;
  switch(size) /* !< Switch case the check the size of the data and check how much of the Flash it needs to be allcoated and what mask to use */
  {
    case 1:
    {
      uint8_t mask = 0x01; /*!< 0000 0001 */
      for(addressOffset = FLASH_DATA_START; addressOffset <= FLASH_DATA_END; addressOffset++) /* !< Increases Offset to shift the 1s of the mask eveery time the Flash is found to be full*/
      {
	if((byteAlloc & (mask << addressOffset)) == (mask << addressOffset)) /*!< E.g. 1111 1111 & 0000 0001 == 0000 0001 */
	{
	  *variable = (void *) addressOffset; /*!< pointer is pointer to another address (double pointer) = address of the pointer = addressOffset */
	  byteAlloc = byteAlloc^(mask << addressOffset); /*!< Changing the variable to keep track the allocation in the FLASH */
	  return true;
	}
      }
    }

    case 2:
    {
      uint8_t mask = 0x03; /*!< 0000 0011 */
      for(addressOffset = FLASH_DATA_START; addressOffset <= FLASH_DATA_END; addressOffset+=2) /* !< Increases +2 Offset to shift the 1s of the mask eveery time the Flash is found to be full*/
      {
	if((byteAlloc & (mask << addressOffset)) == (mask << addressOffset)) /*!< E.g. 1111 1111 & 0000 0011 == 0000 0011 */
	{
	  *variable = (void *) addressOffset;
	  byteAlloc = byteAlloc^(mask << addressOffset);
	  return true;
	}
      }
    }

    case 4:
    {
      uint8_t mask = 0x0f; /*!< 0000 1111 */
      for(addressOffset = FLASH_DATA_START; addressOffset <= FLASH_DATA_END; addressOffset += 4) /* !< Increases + 4 Offset to shift the 1s of the mask eveery time the Flash is found to be full*/
      {
	if((byteAlloc & (mask << addressOffset)) == (mask<< addressOffset)) /*!< E.g. 1111 1111 & 0000 1111 == 0000 1111 */
	{
	  *variable = (void *) addressOffset;
	  byteAlloc = byteAlloc^(mask<< addressOffset);
	  return true;
	}
      }
    }
  }
  return false;
}

bool Flash_Write32(volatile uint32_t* const address, const uint32_t data)
{
  uint64union_t phrase; /* !< union to store the phrase */
  uint32_t index = (uint32_t) address; /* !< typecast address to be uint32_t (as it is supposed to be 24bits) */
  if((index/4) % 2 == 0)
  {
    phrase.s.Hi = _FW(index + 4);
    phrase.s.Lo = data;
    return ModifyPhrase(index, phrase); /* !< Fetching the other part of the phrase (work) and combine them to write */
  }
  else
  {
    phrase.s.Hi = data;
    phrase.s.Lo = _FW(address - 4);
    return ModifyPhrase(index - 4, phrase); /* !< Fetching the other part of the phrase (work) and combine them to write */
  }
}

bool Flash_Write16(volatile uint16_t* const address, const uint16_t data)
{
  /*!< read two things , modify what you need and then call write 32 */
  uint32union_t word; /* !< union to store the word */
  uint32_t index = (uint32_t) address; /* !< typecast address to be uint32_t (as it is supposed to be 24bits) */
  if(index % 4 == 0) /*!< If index is divisible by 4 with no remainder */
  {
    word.s.Hi = _FH(index + 2);
    word.s.Lo = data;
    return Flash_Write32(&(_FW(index)), word.l); /*!< combining the two data to send it to Write 32 */
  }
  else
  {
    word.s.Lo = _FH(index - 2);
    word.s.Hi = data;
    return Flash_Write32(&(_FW(index - 2)), word.l); /*!< combining the two data to send it to Write 32 */
  }
}

bool Flash_Write8(volatile uint8_t* const address, const uint8_t data)
{
  uint16union_t newData; /* !< union to store the 2 bytes = half-word */
  uint32_t index = (uint32_t) address; /* !< typecast address to be uint32_t (as it is supposed to be 24bits) */
  if((address < (uint8_t *) FLASH_DATA_START) || (address > (uint8_t *) FLASH_DATA_END)) /*!< Bad address */
  {
    return false;
  }
  else
  {
    if(index % 2 == 0) /*!< If Byte offset is even or equal to zero */
    {
      newData.s.Hi = _FB(index + 1);
      newData.s.Lo = data;
      return Flash_Write16(&(_FH(index)), newData.l); /* !< Combine the data in union and send it to Write16 */
    }
    else /*!< if byte offset is odd */
    {
      newData.s.Lo = _FB(index-1);
      newData.s.Hi = data;
      return Flash_Write16(&(_FH(index -1)), newData.l); /* !< Combine the data in union and send it to Write16 */
    }
  }
}

bool Flash_Erase(void)

{
  return EraseSector(FLASH_DATA_START);  /*!< EraseSector(address); */
}

/*! @brief Runs LaunchCommand to Erase or Write Data via Flash
 *
 *
 *  @param commonCommandObject
 *  @return bool - TRUE if Command has been run successfully
 *  @note waits to be called by command passing a pointer to a struct
 */
static bool LaunchCommand(TFCCOB* commonCommandObject)  /*!< Sets up command (pg. 806) */
{
  while (!(FTFE_FSTAT & FTFE_FSTAT_CCIF_MASK));  /*!< wait till the command is finished launching from any previous calls */

  if (FTFE_FSTAT & 0x30) /*!< check if flag violation */
  {
    /*!< clear old errors */
    FTFE_FSTAT = FTFE_FSTAT_ACCERR_MASK | FTFE_FSTAT_FPVIOL_MASK; /*!< write 0x30 to FSTAT REGISTER */
  }

  FTFE_FCCOB0 = commonCommandObject->FCCOB_Command;
  FTFE_FCCOB1 = commonCommandObject->address0;
  FTFE_FCCOB2 = commonCommandObject->address1;
  FTFE_FCCOB3 = commonCommandObject->address2;

  /*!< Big Endian sorting, find the appropriate table at p785-786 of reference manual*/
  FTFE_FCCOB8 = commonCommandObject->dataByte0;
  FTFE_FCCOB9 = commonCommandObject->dataByte1;
  FTFE_FCCOBA = commonCommandObject->dataByte2;
  FTFE_FCCOBB = commonCommandObject->dataByte3;
  FTFE_FCCOB4 = commonCommandObject->dataByte4;
  FTFE_FCCOB5 = commonCommandObject->dataByte5;
  FTFE_FCCOB6 = commonCommandObject->dataByte6;
  FTFE_FCCOB7 = commonCommandObject->dataByte7;
  /*!< if there's more parameters do this again. */

  //  if (!FTFE_FSTAT_MGSTAT0_MASK) { //flag raised if runtime error pg. 805)
  //      return false;
  //  }


  FTFE_FSTAT = FTFE_FSTAT_CCIF_MASK; /*!< same code, setting CCIF bit to clear (write to clear) -> FTFE_FSTAT = 0x80; */

  while (!(FTFE_FSTAT & FTFE_FSTAT_CCIF_MASK)); /*!< wait till the command is finished launching */

  return true;
}

/*! @brief Writes Phrase to Struct and passes to LaunchCommand to execute
 *
 *
 *  @param address, phrase
 *  @return bool - TRUE if launchcommand successfully writes
 *  @note Assumes that address and data is correctly set using Big Endian format
 */
static bool WritePhrase(const uint32_t address, const uint64union_t phrase)
{
  /*!< The next 3 lines divides the data to array of smaller data suitable for each byte to be processed */
  uint8_t* flashAddress = (uint8_t *) &address; /*!< https://stackoverflow.com/questions/6499183/converting-a-uint32-value-into-a-uint8-array4 */
  uint8_t* data = (uint8_t*) &phrase.l; /* !< Splitting the uint64union_t phrase into an array of uint8_t variables - MSB being highest index */
  TFCCOB Write; /*!< Declare Struct Write to store the data to write the phrase */

  Write.FCCOB_Command = FCMD_WRITE; /*!< assign the write command as defined in the const above as 0x09 (pg. 789, 808) */
  Write.address0 = flashAddress[2]; /*!< Flash Address [23:16] (pg. 789) */
  Write.address1 = flashAddress[1]; /*!< Flash Address [15:8] (pg. 789) */
  Write.address2 = flashAddress[0]; /*!< Flash Address [7:0] (pg. 789) */
  /*!< Preparing for Big Endian sorting - flip around - the MSB is stored in the lowest index */
  Write.dataByte0 = data[7];
  Write.dataByte1 = data[6];
  Write.dataByte2 = data[5];
  Write.dataByte3 = data[4];
  Write.dataByte4 = data[3];
  Write.dataByte5 = data[2];
  Write.dataByte6 = data[1];
  Write.dataByte7 = data[0];

  return LaunchCommand(&Write); /*!< Calling Launch Command to run Write Function. Passing Write Struct Address */
}

/*! @brief Responsible for setting values of a struct to Erase a Sector
 *
 *
 *  @param address
 *  @return bool - TRUE if launch command erases successfully
 *  @note Pass correct address to successfully erase a secotr, needed before writing to it.
 */
static bool EraseSector(const uint32_t address)
{
 TFCCOB Erase; /*!< Declare Struct Erase */
 Erase.FCCOB_Command = FCMD_ERASE; /*!< assign the erase command as defined in the const above as 0x09 (pg. 789, 808) */
 Erase.address0 = (address >> 16); /*!< Flash Address [23:16] (pg. 789) */
 Erase.address1 = (address >> 8); /*!< Flash Address [15:8] (pg. 789) */
 Erase.address2 = (address); /*!< Flash Address [7:0] (pg. 789) */
 return LaunchCommand(&Erase); /*!< Calling Launch Command to run Erase Function. Passing Erase Struct Address */
}

/*! @brief Modifies the phrase by erase the sector then writing to it again
 *
 *
 *  @param address, phrase
 *  @return bool - TRUE if EraseSector & Write Phrase has been run successfully
 *  @note Assumes that Packet_Init was called
 */
static bool ModifyPhrase(const uint32_t address, const uint64union_t phrase)
{
  /*!< reads out existing phrase, makes modifications and then call WritePhrase */
  if(EraseSector((uint32_t) FLASH_DATA_START)) /*!< calls to Erase Data in Sector before writing to it */
  {
    if(WritePhrase(address, phrase)) /*!< runs write phrase to set data into memory via flash */
    {
      return true; /*!< if successful */
    }
  }
  return false; /*!< if failed */
}


/*!
* @}
*/
