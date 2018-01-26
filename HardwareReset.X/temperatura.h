// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef TC_HEADER_TEMPERATURA_H
#define	TC_HEADER_TEMPERATURA_H

typedef struct {
    // Hardware Configuration
    volatile unsigned char Address;              // 3 bit Address mask (0x00000xxx)
    
    // Software Configuration
    volatile unsigned int TH;                    // Upper temperature alarm value
    volatile unsigned int TL;                    // Lower temperature alarm value
    volatile unsigned char OneShot;              // Conversion mode (1Shot or continuous)
    volatile unsigned char Resolution;           // 9-12 bits
    
    // Status
    volatile unsigned char MSBTemperature;       // MSB Last converted temperature value
    volatile unsigned char LSBTemperature;       // LSB Last converted temperature value
    volatile unsigned char isConversionDone;     // Last conversion request completed?
    volatile unsigned char AlarmH;               // Is temperature over TH?
    volatile unsigned char AlarmL;               // Is temperature below TL?
    volatile unsigned char EEPROMBusy;           // EEPROM operation in progress
    volatile unsigned char POL;                  // Thermostat output pin - T_OUT is active high (1) or active low (0)
    
    // Values
    volatile double Temperature;
    
    // Operations
    volatile unsigned char Success;              // Was the operation successful?
    volatile unsigned char pCount;               // Message Parameter Count
    volatile unsigned char msg_buffer_in[3];     // Incoming Message Buffer
    volatile unsigned char msg_buffer_out[3];    // Outgoing Message Buffer
}tSensor_Temperatura;

void DS_Init(tSensor_Temperatura*);
void DS_StartConversion(tSensor_Temperatura*);
void DS_StopConversion(tSensor_Temperatura*);
void DS_ReadTemperature(tSensor_Temperatura*);
void DS_GetConfig(tSensor_Temperatura*);
void DS_SetConfig(tSensor_Temperatura*);
//void DS_Reset(tSensor_Temperatura*);

#endif	/* TC_HEADER_TEMPERATURA_H */

