 /******************************************************************************
 *
 * Module: External EEPROM
 *
 * File Name: external_eeprom.h
 *
 * Description: Source file for the External EEPROM Memory
 *
 * Author: Islam Ehab
 *
 * Date: 28/9/2020
 *
 * Version: 1.0.0
 *******************************************************************************/

#include "i2c.h"
#include "external_eeprom.h"

/********************************************************************************
 [Function Name]:	EEPROM_init

 [Description]:		Used to initialize External EEPROM HW

 [Args]:			None

 [in]				None

 [out]				None

 [in/out]			None

 [Returns]:			None
**********************************************************************************/
void EEPROM_init(void)
{
	/* Just Initialize I2C Module */
	TWI_ConfigType twi_Struct;

	twi_Struct.Address		 = ADDRESS_OF_MC_IN_SLAVE_MODE;
	twi_Struct.BitRate		 = FAST_MODE_400KB;
	twi_Struct.InterruptMode = INTERRUPT_MODE_DISABLE;

	/* Initializa I2C Module */
	TWI_init(&twi_Struct);


//	    /* Bit Rate: 400.000 kbps using zero pre-scaler TWPS=00 and F_CPU=8Mhz */
//	    TWBR = 0x02;
//		TWSR = 0x00;
//
//	    /* Two Wire Bus address my address if any master device want to call me: 0x1 (used in case this MC is a slave device)
//	       General Call Recognition: Off */
//	    TWAR = 0b00000010; // my address = 0x01 :)
//
//	    TWCR = (1<<TWEN); /* enable TWI */

}



/********************************************************************************
 [Function Name]:	EEPROM_writeByte

 [Description]:		Function used to write byte on EEPROM

 [Args]:			u16addr, u8data

 [in]				u16addr: Address of segment needed to be written on it
 	 	 	 	 	u8data:	 Byte required to be written

 [out]				None

 [in/out]			None

 [Returns]:			Status of writing (SUCCESS if everything is fine)
**********************************************************************************/
uint8 EEPROM_writeByte(uint16 u16addr, uint8 u8data){

	/* Send start bit to start I2C Frame */
	TWI_start();

	/* Check  of the status of sending Start bit */
	if(TWI_getStatus() != TW_START){

		/* Start bit did NOT Transmitted succesfully */
		return ERROR;
	}

	/*
	 * In 24C16 EEPROM, LSB is the word address, MSB is the device address
	 * in the 8 bits of device address, Most significant 4 bits are 1010 (0xA)
	 * but the least significant 4 bits are the last 3 bits of memory address
	 * and the read/write bit (0 if write, 1 if read)
	 *
	 * so, we need to get most significant 3 bits from memory address
	 * so, masking the msb with 0x0700 and right shift them to get saved
	 * and the last bit is write (0)
	*/

    TWI_write((uint8)(0xA0 | ((u16addr & 0x0700)>>7)));

	/* Check of Sending Slave address and reciving its ACK Status */
	if(TWI_getStatus() != TW_MT_SLA_W_ACK){

		/* Error in sending slave address or receiving its ACK */
		return ERROR;
	}

	/* Send next byte which is the proper memory address */
	TWI_write((uint8)(u16addr));

	/* Check of transmitting data byte and receiving its ACK */
	if(TWI_getStatus() != TW_MT_DATA_ACK){

		/* Error in transmitting data byte or receiving its ACK */
		return ERROR;
	}

	/* Write Data byte on the EEPROM memory */
	TWI_write(u8data);

	/* Check of transmitting data byte and receiving its ACK */
	if(TWI_getStatus() != TW_MT_DATA_ACK){

		/* Error in transmitting data byte or receiving its ACK */
		return ERROR;
	}

	/* Sending Stop bit to end the frame */
	TWI_stop();

	/* Going to this point means that everything is fine */
	return SUCCESS;
}




/********************************************************************************
 [Function Name]:	EEPROM_readByte

 [Description]:		Function used to read byte on EEPROM

 [Args]:			u16addr, u8data

 [in]				u16addr: Address of segment needed to be written on it

 [out]				u8data:	 Pointer to an empty char to save the read byte on it

 [in/out]			None

 [Returns]:			Status of Reading (SUCCESS if everything is fine)
**********************************************************************************/
uint8 EEPROM_readByte(uint16 u16addr, uint8 *u8data)
{
	/* Send start bit to start I2C Frame */
	TWI_start();

	/* Check  of the status of sending Start bit */
	if(TWI_getStatus() != TW_START){

		/* Start bit did NOT Transmitted succesfully */
		return ERROR;
	}

	/*
	 * In 24C16 EEPROM, LSB is the word address, MSB is the device address
	 * in the 8 bits of device address, Most significant 4 bits are 1010 (0xA)
	 * but the least significant 4 bits are the last 3 bits of memory address
	 * and the read/write bit (0 if write, 1 if read)
	 *
	 * so, we need to get most significant 3 bits from memory address
	 * so, masking the msb with 0x0700 and right shift them to get saved
	 * and the last bit is write (0)
	*/

    TWI_write((uint8)((0xA0) | ((u16addr & 0x0700)>>7)));

	/* Check of Sending Slave address and reciving its ACK Status */
	if(TWI_getStatus() != TW_MT_SLA_W_ACK){

		/* Error in sending slave address or receiving its ACK */
		return ERROR;
	}

	/* Send next byte which is the proper memory address */
	TWI_write((uint8)(u16addr));

	/* Check of transmitting data byte and receiving its ACK */
	if(TWI_getStatus() != TW_MT_DATA_ACK){

		/* Error in transmitting data byte or receiving its ACK */
		return ERROR;
	}

	/* Sending Repeated start Bit */
	TWI_start();

	/* Check of sending repeated start bit properly */
	if(TWI_getStatus() != TW_REP_START){

		/* Error in sending reapted start bit */
		return ERROR;
	}

	/*
	 * In 24C16 EEPROM, LSB is the word address, MSB is the device address
	 * in the 8 bits of device address, Most significant 4 bits are 1010 (0xA)
	 * but the least significant 4 bits are the last 3 bits of memory address
	 * and the read/write bit (0 if write, 1 if read)
	 *
	 * so, we need to get most significant 3 bits from memory address
	 * so, masking the msb with 0x0700 and right shift them to get saved
	 * and the last bit is read (1)
	*/

    TWI_write((uint8)((0xA0) | ((u16addr & 0x0700)>>7) | 1));

	/* Check of Sending Slave address and reciving its ACK Status */
	if(TWI_getStatus() != TW_MT_SLA_R_ACK){

		/* Error in sending slave address or receiving its ACK */
		return ERROR;
	}

	/* Read required byte from EEPROM Memory without sending ACK */
	 *u8data = TWI_readWithNACK();

	/* Check of reading properly without sending ACK */
	 if(TWI_getStatus() != TW_MR_DATA_NACK){

		 /* Error in reading byte */
		 return ERROR;
	 }

	 /* Sending STOP Bit */
	 TWI_stop();

	/* Going to this point means that everything is fine */
	return SUCCESS;
}

