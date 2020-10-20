 /******************************************************************************
 *
 * Module: External EEPROM
 *
 * File Name: external_eeprom.h
 *
 * Description: Header file for the External EEPROM Memory
 *
 * Author: Islam Ehab
 *
 * Date: 28/9/2020
 *
 * Version: 1.0.0
 *******************************************************************************/



#ifndef EXTERNAL_EEPROM_H_
#define EXTERNAL_EEPROM_H_

#include "std_types.h"

/*******************************************************************************
 *                      Preprocessor Macros                                    *
 *******************************************************************************/
#define ERROR 0
#define SUCCESS 1

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/


/********************************************************************************
 [Function Name]:	EEPROM_init

 [Description]:		Used to initialize External EEPROM HW

 [Args]:			None

 [in]				None

 [out]				None

 [in/out]			None

 [Returns]:			None
**********************************************************************************/
void EEPROM_init(void);


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
uint8 EEPROM_writeByte(uint16 u16addr,uint8 u8data);


/********************************************************************************
 [Function Name]:	EEPROM_readByte

 [Description]:		Function used to read byte on EEPROM

 [Args]:			u16addr, u8data

 [in]				u16addr: Address of segment needed to be written on it

 [out]				u8data:	 Pointer to an empty char to save the read byte on it

 [in/out]			None

 [Returns]:			Status of Reading (SUCCESS if everything is fine)
**********************************************************************************/
uint8 EEPROM_readByte(uint16 u16addr,uint8 *u8data);


#endif /* EXTERNAL_EEPROM_H_ */
