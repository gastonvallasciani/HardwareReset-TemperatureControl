/*
 * File:   temperatura.c
 * Author: Nico Christie
 *
 * Created on 4 de abril de 2016, 12:12
 */

#include "mcc_generated_files/i2c1.h"
#include "temperatura.h"

#define YES 1
#define NO	0

/**
 * Sensor de temperatura DS1631 (Maxim Integrated)
 * 
 * Dirección:
 * 
 *		BIT7	BIT6	BIT5	BIT4	BIT3	BIT2	BIT1	BIT0
 *		  1		  0		  0		  1		 A2		 A1		 A0		R/_W
 * 
 *		A0-A2	por harware puesto en 0 (GND)
 *		BIT0	indica si la operación es lectura (1) o escritura (0)
 * 
 * Las líneas de SDA y SCK tienen un pullup de 2.2k
 */

#define DEVICE_MASK_FAMILY			0b10010000	// 0x90
#define DEVICE_MASK_ADDRESS			0b00000000	// 0x00 (0b0000xxx0 All pins to GND)
#define DEVICE_MASK_READ			0b00000001	// 0x01
#define DEVICE_MASK_WRITE			0b00000000	// 0x00

#define DEVICE_ADDRESS				((DEVICE_MASK_FAMILY | DEVICE_MASK_ADDRESS) >> 1) // 7 Bit Address for I2C transmission

// Configuration register bit description
// Power-up state: 100011XX (XX = user defined)
#define CONFIG_MASK_BIT_DONE		(1 << 7) // (r)		Temperature conversion in progress (0) or Done (1)
#define CONFIG_MASK_BIT_THF			(1 << 6) // (r)		Measured temperature is below TH (0) or is over TH (1)
#define CONFIG_MASK_BIT_TLF			(1 << 5) // (r)		Measured temperature is over TL (0) or is below TL (1)
#define CONFIG_MASK_BIT_NVB			(1 << 4) // (r)		A write to EEPROM is in progress (1) or not (0))
#define CONFIG_MASK_BIT_RB1			(1 << 3) // (r/w)	Sets conversion resolution bit H
#define CONFIG_MASK_BIT_RB0			(1 << 2) // (r/w)	Sets conversion resolution bit L
#define CONFIG_MASK_BIT_POL			(1 << 1) // (r/w)	(*) T_OUT is active HIGH (1) or LOW (0)
#define CONFIG_MASK_BIT_1SHOT		(1 << 0) // (r/w)	(*) One-shot mode (1) or Continous Conversion mode (0)
#define CONFIG_MASK_WRITEABLES		0x0F	 //			MS 4 bits are status, LS 4 bits are configuration
// (*) Stored in EEPROM

#define CONFIG_RESOLUTION_09BITS	(0 << 3) | (0 << 2) // Conversion time 93.75ms
#define CONFIG_RESOLUTION_10BITS	(0 << 3) | (1 << 2) // Conversion time 187.5ms
#define CONFIG_RESOLUTION_11BITS	(1 << 3) | (0 << 2) // Conversion time 375ms
#define CONFIG_RESOLUTION_12BITS	(1 << 3) | (1 << 2) // Conversion time 750ms
#define CONFIG_RESOLUTION_MASK		(1 << 3) | (1 << 2)

#define RESOLUTION_INCREMENT_09BITS	0.5		// 0.5ºC per resolution bit
#define RESOLUTION_INCREMENT_10BITS	0.25	// 0.25ºC per resolution bit
#define RESOLUTION_INCREMENT_11BITS	0.125	// 0.125ºC per resolution bit
#define RESOLUTION_INCREMENT_12BITS	0.0625	// 0.0625ºC per resolution bit

#define POSITIVE					1
#define NEGATIVE					-1

#define RESOLUTION_SHIFT_09BITS		7
#define RESOLUTION_SHIFT_10BITS		6
#define RESOLUTION_SHIFT_11BITS		5
#define RESOLUTION_SHIFT_12BITS		4

// If the part is in one-shot mode (1SHOT = 1), only one
// conversion is performed. In continuous mode (1SHOT = 0),
// continuous temperature conversions are performed until
// a Stop Convert T command is issued.
#define COMMAND_START_CONVERT		0x51 // Initiates temperature conversion
#define COMMAND_STOP_CONVERT		0x22 // Stops temperature conversion
#define COMMAND_READ_TEMPERATURE	0xAA // Reads last converted temperature
#define COMMAND_ACCESS_TH			0xA1 // Reads or writes the 2-byte TH register
#define COMMAND_ACCESS_TL			0xA2 // Reads or writes the 2-byte TL register
#define COMMAND_ACCESS_CONFIG		0xAC //	Reads or writes the 1-byte configuration register
#define COMMAND_SOFTWARE_POR		0x54 // Initiaties a software power-on-reset (POR))

#define TEMPERATURE_MASK_SIGN		0x80 // MSBit of the 2 byte Read Temperature message
#define TEMPERATURE_MASK_MSByte		0x7F // MSByte of the 2 byte Read Temperature message
#define TEMPERATURE_MASK_LSByte		0xF0 // LSByte of the 2 byte Read Temperature message
#define TEMPERATURE_MASK_LSByte_12B	0xF0 // LSByte of the 2 byte Read Temperature message (12 bit resolution)
#define TEMPERATURE_MASK_LSByte_11B	0xE0 // LSByte of the 2 byte Read Temperature message (11 bit resolution)
#define TEMPERATURE_MASK_LSByte_10B	0xC0 // LSByte of the 2 byte Read Temperature message (10 bit resolution)
#define TEMPERATURE_MASK_LSByte_09B	0x80 // LSByte of the 2 byte Read Temperature message (09 bit resolution)

#define	SIZE_TEMPERATURE			2
#define SIZE_TH						2
#define SIZE_TL						2
#define SIZE_CONFIG					1

I2C1_MESSAGE_STATUS status = I2C1_MESSAGE_PENDING;

unsigned char DS_Write(tSensor_Temperatura*);
unsigned char DS_Read(tSensor_Temperatura*);

void DS_Init(tSensor_Temperatura* Device)
{
	Device->isConversionDone = NO;
	Device->Resolution = CONFIG_RESOLUTION_12BITS;
	Device->POL = 1;
	Device->OneShot = 1;
	DS_SetConfig(Device);
}

void DS_StartConversion(tSensor_Temperatura* Device)
{
	Device->pCount = 1;
	Device->msg_buffer_out[0] = COMMAND_START_CONVERT;	
	Device->Success = DS_Write(Device);
}
void DS_StopConversion(tSensor_Temperatura* Device)
{
	Device->pCount = 1;
	Device->msg_buffer_out[0] = COMMAND_STOP_CONVERT;
	Device->Success = DS_Write(Device);
}
void DS_ReadTemperature(tSensor_Temperatura* Device)
{
	Device->MSBTemperature = 0xFF;
	Device->LSBTemperature = 0xFF;

	Device->pCount = 1;
	Device->msg_buffer_out[0] = COMMAND_READ_TEMPERATURE;
	Device->Success = DS_Write(Device);
	if (Device->Success == NO) return;
	
	Device->pCount = SIZE_TEMPERATURE;
	Device->Success = DS_Read(Device);
	
	/* After each conversion, the digital temperature is stored as a 16-bit
	 * two?s complement number in the two-byte temperature register as shown
	 * in Figure 4. The sign bit (S) indicates if the temperature is positive
	 * or negative: for positive numbers S = 0 and for negative numbers S = 1.
	 * The Read Temperature command provides user access to the temperature
	 * register. Bits 3 through 0 of the temperature register are hardwired
	 * to 0. When the device is configured for 12-bit resolution, the 12 MSbs
	 * (bits 15 through 4) of the temperature register contain temperature data.
	 * For 11 bit resolution, the 11 MSbs (bits 15 through 5) of the temperature
	 * register contain data, and bit 4 is 0. Likewise, for 10-bit resolution,
	 * the 10 MSbs (bits 15 through 6) contain data, and for 9-bit the 9 MSbs 
	 * (bits 15 through 7) contain data, and all unused LSbs contain 0s. Table 4
	 * gives examples of 12-bit resolution output data and the corresponding
	 * temperatures
	 *		   BIT 15	BIT 14	BIT 13	BIT 12	BIT 11	BIT 10	BIT 9	BIT 8 
	 * MS BYTE S		2^6		2^5		2^4		2^3		2^2		2^1		2^0
	 *		   BIT 7	BIT 6	BIT 5	BIT 4	BIT 3	BIT 2	BIT 1	BIT 0
	 * LS BYTE 2^-1		2^-2	2^-3	2^-4	0		0		0		0	  */
	signed char Sign;
	unsigned char MSB, LSB;
	Sign = ((Device->msg_buffer_in[0] & TEMPERATURE_MASK_SIGN) == 0 ? POSITIVE : NEGATIVE);
	MSB = Device->msg_buffer_in[0] & TEMPERATURE_MASK_MSByte;
	LSB = Device->msg_buffer_in[1] & TEMPERATURE_MASK_LSByte;
	if (Sign == NEGATIVE) { MSB = ~MSB; LSB = ~LSB + 1; } // 2's complement	

	int Enteros = 100 * MSB;
	int Decimales = 100 * LSB / 256;
	// Si T: 23.15º -> Ents + Decs = 2315

	Device->MSBTemperature = MSB;
	Device->LSBTemperature = LSB;
	Device->Temperature = Sign * (((double)(Enteros + Decimales)) / 100);
}
/*void DS_GetHighThreshold(tSensor_Temperatura* Device)
{
	
}
void DS_SetHighThreshold(tSensor_Temperatura* Device, unsigned int TempValue)
{
	
}
void DS_GetLowThreshold(tSensor_Temperatura* Device)
{
	
}
void DS_SetLowThreshold(tSensor_Temperatura* Device, unsigned int TempValue)
{
	
}*/
void DS_GetConfig(tSensor_Temperatura* Device)
{
	Device->pCount = 1;
	Device->msg_buffer_out[0] = COMMAND_ACCESS_CONFIG;
	Device->Success = DS_Write(Device);
	if (Device->Success == NO) return;

	Device->pCount = SIZE_CONFIG;
	Device->Success = DS_Read(Device);

	// CONFIG_MASK_BIT_DONE		(r)		Temperature conversion in progress (0) or Done (1)
	// CONFIG_MASK_BIT_THF		(r)		Measured temperature is below TH (0) or is over TH (1)
	// CONFIG_MASK_BIT_TLF		(r)		Measured temperature is over TL (0) or is below TL (1)
	// CONFIG_MASK_BIT_NVB		(r)		A write to EEPROM is in progress (1) or not (0))
	// CONFIG_MASK_BIT_RB1		(r/w)	Sets conversion resolution bit H
	// CONFIG_MASK_BIT_RB0		(r/w)	Sets conversion resolution bit L
	// CONFIG_MASK_BIT_POL		(r/w)	(*) T_OUT is active HIGH (1) or LOW (0)
	// CONFIG_MASK_BIT_1SHOT	(r/w)	(*) One-shot mode (1) or Continuous Conversion mode (0)
	Device->isConversionDone = (Device->msg_buffer_in[0] & CONFIG_MASK_BIT_DONE) != 0;
	Device->AlarmH = (Device->msg_buffer_in[0] & CONFIG_MASK_BIT_THF) != 0;
	Device->AlarmL = (Device->msg_buffer_in[0] & CONFIG_MASK_BIT_TLF) != 0;
	Device->EEPROMBusy = (Device->msg_buffer_in[0] & CONFIG_MASK_BIT_NVB) != 0;
	Device->Resolution = (Device->msg_buffer_in[0] & CONFIG_RESOLUTION_MASK);
	Device->POL = (Device->msg_buffer_in[0] & CONFIG_MASK_BIT_POL) != 0;
	Device->OneShot = (Device->msg_buffer_in[0] & CONFIG_MASK_BIT_1SHOT) != 0;	
}
void DS_SetConfig(tSensor_Temperatura* Device)
{
	DS_StopConversion(Device);
	
	Device->pCount = 1 + SIZE_CONFIG;
	Device->msg_buffer_out[0] = COMMAND_ACCESS_CONFIG;
	Device->msg_buffer_out[1] = CONFIG_MASK_WRITEABLES & (Device->Resolution + (Device->POL << 1) + (Device->OneShot << 0));	
	Device->Success = DS_Write(Device);
}
/*void DS_Reset(tSensor_Temperatura* Device)
{
	Device->pCount = 1;
	Device->msg_buffer_out[0] = COMMAND_SOFTWARE_POR;
	Device->Success = DS_Write(Device);
}*/

unsigned char DS_Write(tSensor_Temperatura* Device)
{
	unsigned int timeOut = 0;
	while(status != I2C1_MESSAGE_FAIL)
	{
		I2C1_MasterWrite((unsigned char*)Device->msg_buffer_out, Device->pCount, DEVICE_ADDRESS | Device->Address, &status);

		// wait for the message to be sent or status has changed.
		while(status == I2C1_MESSAGE_PENDING);

		if (status == I2C1_MESSAGE_COMPLETE) break;

		// if status is  I2C1_MESSAGE_ADDRESS_NO_ACK,
		//               or I2C1_DATA_NO_ACK,
		// The device may be busy and needs more time for the last
		// write so we can retry writing the data, this is why we
		// use a while loop here
		if (timeOut >= 5) break;
		else timeOut++;
	}
	return (status == I2C1_MESSAGE_COMPLETE);
}
unsigned char DS_Read(tSensor_Temperatura* Device)
{
	status = I2C1_MESSAGE_PENDING;
	while(status != I2C1_MESSAGE_FAIL)
	{
		I2C1_MasterRead((unsigned char*)Device->msg_buffer_in, Device->pCount, DEVICE_ADDRESS | Device->Address, &status);

		// wait for the message to be sent or status has changed.
		while(status == I2C1_MESSAGE_PENDING);

		if (status == I2C1_MESSAGE_COMPLETE) break;

		// if status is  I2C1_MESSAGE_ADDRESS_NO_ACK,
		//               or I2C1_DATA_NO_ACK,
		// The device may be busy and needs more time for the last
		// write so we can retry writing the data, this is why we
		// use a while loop here
	}
	return (status == I2C1_MESSAGE_COMPLETE);
}