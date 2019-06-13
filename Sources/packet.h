/*! @file
 *
 *  @brief Routines to implement packet encoding and decoding for the serial port.
 *
 *  This contains the functions for implementing the "Tower to PC Protocol" 5-byte packets.
 *
 *  @author PMcL
 *  @date 2015-07-23
 */

#ifndef PACKET_H
#define PACKET_H

/*!< New types */
#include "types.h"

/*!< Packet structure */
#define PACKET_NB_BYTES 5

#pragma pack(push)
#pragma pack(1)
#pragma pack(pop)

typedef union
{
  uint8_t bytes[PACKET_NB_BYTES];     /*!< The packet as an array of bytes. */
  struct
  {
    uint8_t command;		      /*!< The packet's command. */
    union
    {
      struct
      {
        uint8_t parameter1;	      /*!< The packet's 1st parameter. */
        uint8_t parameter2;	      /*!< The packet's 2nd parameter. */
        uint8_t parameter3;	      /*!< The packet's 3rd parameter. */
      } separate;
      struct
      {
        uint16_t parameter12;         /*!< Parameter 1 and 2 concatenated. */
        uint8_t parameter3;
      } combined12;
      struct
      {
        uint8_t paramater1;
        uint16_t parameter23;         /*!< Parameter 2 and 3 concatenated. */
      } combined23;
    } parameters;
    uint8_t checksum;
  } packetStruct;
} TPacket;

extern TPacket Packet;

#define Packet_Command     Packet.packetStruct.command
#define Packet_Parameter1  Packet.packetStruct.parameters.separate.parameter1
#define Packet_Parameter2  Packet.packetStruct.parameters.separate.parameter2
#define Packet_Parameter3  Packet.packetStruct.parameters.separate.parameter3
#define Packet_Parameter12 Packet.packetStruct.parameters.combined12.parameter12
#define Packet_Parameter23 Packet.packetStruct.parameters.combined23.parameter23
#define Packet_Checksum    Packet.packetStruct.checksum

/*!<All macro for packets and commands are found in Tower Serial Communication Protocol.pdf */

/***********************************************************************************************************
 ************************************** TOWER TO PC COMMANDS ***********************************************
 **********************************************************************************************************/

/*!< What the tower should send to the PC upon startup */
#define TOWER_STARTUP_COMMAND 0x04
#define TOWER_STARTUP_PARAMETER1 0
#define TOWER_STARTUP_PARAMETER2 0
#define TOWER_STARTUP_PARAMETER3 0

/*!< What the tower should send to the PC when fetching the tower version */
#define TOWER_VERSION_COMMAND 0x09
#define TOWER_VERSION_PARAMETER1 'v' /*!< v for version */
#define TOWER_VERSION_PARAMETER2 1 /*!< major (version) */
#define TOWER_VERSION_PARAMETER3 0

/*!< Command to get or set the tower mode */
#define TOWER_MODE_COMMAND 0x0D

/*!< The first two bytes that the tower should send to the PC upon startup */
#define TOWER_NUMBER_COMMAND 0x0b
#define TOWER_NUMBER_PARAMETER1 1

//Command with acknowledgement bit 7
#define TOWER_STARTUP_ACK 0x84
#define TOWER_VERSION_ACK 0x89
#define TOWER_NUMBER_ACK 0x8b


/*!< Command to Flash - Program byte */
#define FLASH_PROGRAM_COMMAND 0x07
/*!< Parameter1 = address offset (0-8). offset of 0x08 will erase the entire FLASH sector */
#define FLASH_PROGRAM_PARAMETER2 0
/*!<  Parameter3 = data */


/*!< Command to Flash - Read byte */
#define FLASH_READ_COMMAND 0x08
/*!<  Parameter1 = address offset (0-7) */
#define FLASH_READ_PARAMETER2 0
#define FLASH_READ_PARAMETER3 0

/*!< Command to set RTC clock Time */
#define SET_TIME_COMMAND 0x0C

/*!< Command to set or get Protocol Mode*/
#define PROTOCOL_COMMAND 0x0A

#define PROTOCOL_GET 1
#define PROTOCOL_SET 2

#define PROTOCOL_ASYNC 0
#define PROTOCOL_SYNC 1

#define PROTOCOL_PARAMETER1 1
#define PROTOCOL_GET_PARAMETER2 0
#define PROTOCOL_PARAMETER3 0

/*!< Command to send accelerometer data */
#define ACCEL_COMMAND 0x10


/************************************************************************************************************
 * ************************************** PC TO TOWER COMMANDS **********************************************
 ***********************************************************************************************************/
/*!< Special - Get the startup value from tower upon startup */
#define GET_TOWER_STARTUP 0x04

/*!< Special - Get the tower version */
#define GET_TOWER_VERSION 0x09

/*!< Get or set the tower number from the PC */
#define TOWER_NUMBER_SET 2 /*!< PARAMETER1 to be set to 2 to set the tower number */
#define TOWER_NUMBER_GET 1 /*!< PARAMETER1 has to be set to 1 to get the tower number */

/*!< Get or set the tower mode  from the PC */
#define TOWER_MODE_SET 2 /*!< when PARAMETER1 is 2 --> SET TOWER MODE */
#define TOWER_MODE_GET 1 /*!< when PARAMETER1 is 1 --> GET TOWER MODE */

/*! @brief Initializes the packets by calling the initialization routines of the supporting software modules.
 *
 *  @param baudRate The desired baud rate in bits/sec.
 *  @param moduleClk The module clock rate in Hz.
 *  @return bool - TRUE if the packet module was successfully initialized.
 */
bool Packet_Init(const uint32_t baudRate, const uint32_t moduleClk);

/*! @brief Attempts to get a packet from the received data.
 *
 *  @return bool - TRUE if a valid packet was received.
 */
bool Packet_Get(void);

/*! @brief Builds a packet and places it in the transmit FIFO buffer.
 *
 *  @return bool - TRUE if a valid packet was sent.
 */
bool Packet_Put(const uint8_t command, const uint8_t parameter1, const uint8_t parameter2, const uint8_t parameter3);


/*! @brief Calculates the checksum of a packet
 *
 *  @return uint8_t - the checksum XOR bitwise
 */
uint8_t Checksum_Calculation(const uint8_t command, const uint8_t parameter1, const uint8_t parameter2, const uint8_t parameter3);

#endif
