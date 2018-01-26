// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef TC_I2C_HEADER_TEMPLATE_H
#define	TC_I2C_HEADER_TEMPLATE_H

#include "devices.h"

#define I2C1_BASE_SLAVE_ADDRESS		(unsigned int)0x90
#define I2C1_SLAVE_ID               (unsigned int)0x02
#define I2C1_SLAVE_ID1              (unsigned int)0x03                                                      //gav
#define I2C1_SLAVE_ID2              (unsigned int)0x04                                                      //gav
#define I2C1_SHIFTED_SLAVE_ADDRESS	(unsigned int)((I2C1_BASE_SLAVE_ADDRESS >> 1) + I2C1_SLAVE_ID)          //gav
#define I2C1_SHIFTED_SLAVE_ADDRESS1	(unsigned int)((I2C1_BASE_SLAVE_ADDRESS >> 1) + I2C1_SLAVE_ID1)         //gav
#define I2C1_SHIFTED_SLAVE_ADDRESS2	(unsigned int)((I2C1_BASE_SLAVE_ADDRESS >> 1) + I2C1_SLAVE_ID2)         //gav
#define I2C1_SLAVE_MASK				(unsigned int)0xFF

void I2C_GetNewMessages(void);
void I2C_ProcessMessages(void);

void I2C_Send(tPacket*);

void I2C_ResetBus(void);

#endif	/* TC_I2C_HEADER_TEMPLATE_H */
