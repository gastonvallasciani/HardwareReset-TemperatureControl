/**
  @Generated Pin Manager Header File

  @Company:
    Microchip Technology Inc.

  @File Name:
    pin_manager.h

  @Summary:
    This is the Pin Manager file generated using MPLAB(c) Code Configurator

  @Description:
    This header file provides implementations for pin APIs for all pins selected in the GUI.
    Generation Information :
        Product Revision  :  MPLAB(c) Code Configurator - 4.15
        Device            :  PIC18F46K22
        Version           :  1.01
    The generated drivers are tested against the following:
        Compiler          :  XC8 1.35
        MPLAB             :  MPLAB X 3.40

    Copyright (c) 2013 - 2015 released Microchip Technology Inc.  All rights reserved.

    Microchip licenses to you the right to use, modify, copy and distribute
    Software only when embedded on a Microchip microcontroller or digital signal
    controller that is integrated into your product or third party product
    (pursuant to the sublicense terms in the accompanying license agreement).

    You should refer to the license agreement accompanying this Software for
    additional information regarding your rights and obligations.

    SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
    EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF
    MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
    IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER
    CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR
    OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
    INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR
    CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF
    SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
    (INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.

*/


#ifndef PIN_MANAGER_H
#define PIN_MANAGER_H

#define INPUT   1
#define OUTPUT  0

#define HIGH    1
#define LOW     0

#define ANALOG      1
#define DIGITAL     0

#define PULL_UP_ENABLED      1
#define PULL_UP_DISABLED     0

// get/set LED_TEST aliases
#define LED_TEST_TRIS               TRISAbits.TRISA0
#define LED_TEST_LAT                LATAbits.LATA0
#define LED_TEST_PORT               PORTAbits.RA0
#define LED_TEST_ANS                ANSELAbits.ANSA0
#define LED_TEST_SetHigh()            do { LATAbits.LATA0 = 1; } while(0)
#define LED_TEST_SetLow()             do { LATAbits.LATA0 = 0; } while(0)
#define LED_TEST_Toggle()             do { LATAbits.LATA0 = ~LATAbits.LATA0; } while(0)
#define LED_TEST_GetValue()           PORTAbits.RA0
#define LED_TEST_SetDigitalInput()    do { TRISAbits.TRISA0 = 1; } while(0)
#define LED_TEST_SetDigitalOutput()   do { TRISAbits.TRISA0 = 0; } while(0)
#define LED_TEST_SetAnalogMode()  do { ANSELAbits.ANSA0 = 1; } while(0)
#define LED_TEST_SetDigitalMode() do { ANSELAbits.ANSA0 = 0; } while(0)

// get/set TEMP_SENSOR aliases
#define TEMP_SENSOR_TRIS               TRISAbits.TRISA2
#define TEMP_SENSOR_LAT                LATAbits.LATA2
#define TEMP_SENSOR_PORT               PORTAbits.RA2
#define TEMP_SENSOR_ANS                ANSELAbits.ANSA2
#define TEMP_SENSOR_SetHigh()            do { LATAbits.LATA2 = 1; } while(0)
#define TEMP_SENSOR_SetLow()             do { LATAbits.LATA2 = 0; } while(0)
#define TEMP_SENSOR_Toggle()             do { LATAbits.LATA2 = ~LATAbits.LATA2; } while(0)
#define TEMP_SENSOR_GetValue()           PORTAbits.RA2
#define TEMP_SENSOR_SetDigitalInput()    do { TRISAbits.TRISA2 = 1; } while(0)
#define TEMP_SENSOR_SetDigitalOutput()   do { TRISAbits.TRISA2 = 0; } while(0)
#define TEMP_SENSOR_SetAnalogMode()  do { ANSELAbits.ANSA2 = 1; } while(0)
#define TEMP_SENSOR_SetDigitalMode() do { ANSELAbits.ANSA2 = 0; } while(0)

// get/set RC6 procedures
#define RC6_SetHigh()    do { LATCbits.LATC6 = 1; } while(0)
#define RC6_SetLow()   do { LATCbits.LATC6 = 0; } while(0)
#define RC6_Toggle()   do { LATCbits.LATC6 = ~LATCbits.LATC6; } while(0)
#define RC6_GetValue()         PORTCbits.RC6
#define RC6_SetDigitalInput()   do { TRISCbits.TRISC6 = 1; } while(0)
#define RC6_SetDigitalOutput()  do { TRISCbits.TRISC6 = 0; } while(0)
#define RC6_SetAnalogMode() do { ANSELCbits.ANSC6 = 1; } while(0)
#define RC6_SetDigitalMode()do { ANSELCbits.ANSC6 = 0; } while(0)

// get/set RELAY2 aliases
#define RELAY2_TRIS               TRISDbits.TRISD6
#define RELAY2_LAT                LATDbits.LATD6
#define RELAY2_PORT               PORTDbits.RD6
#define RELAY2_ANS                ANSELDbits.ANSD6
#define RELAY2_SetHigh()            do { LATDbits.LATD6 = 1; } while(0)
#define RELAY2_SetLow()             do { LATDbits.LATD6 = 0; } while(0)
#define RELAY2_Toggle()             do { LATDbits.LATD6 = ~LATDbits.LATD6; } while(0)
#define RELAY2_GetValue()           PORTDbits.RD6
#define RELAY2_SetDigitalInput()    do { TRISDbits.TRISD6 = 1; } while(0)
#define RELAY2_SetDigitalOutput()   do { TRISDbits.TRISD6 = 0; } while(0)
#define RELAY2_SetAnalogMode()  do { ANSELDbits.ANSD6 = 1; } while(0)
#define RELAY2_SetDigitalMode() do { ANSELDbits.ANSD6 = 0; } while(0)

// get/set RELAY1 aliases
#define RELAY1_TRIS               TRISDbits.TRISD7
#define RELAY1_LAT                LATDbits.LATD7
#define RELAY1_PORT               PORTDbits.RD7
#define RELAY1_ANS                ANSELDbits.ANSD7
#define RELAY1_SetHigh()            do { LATDbits.LATD7 = 1; } while(0)
#define RELAY1_SetLow()             do { LATDbits.LATD7 = 0; } while(0)
#define RELAY1_Toggle()             do { LATDbits.LATD7 = ~LATDbits.LATD7; } while(0)
#define RELAY1_GetValue()           PORTDbits.RD7
#define RELAY1_SetDigitalInput()    do { TRISDbits.TRISD7 = 1; } while(0)
#define RELAY1_SetDigitalOutput()   do { TRISDbits.TRISD7 = 0; } while(0)
#define RELAY1_SetAnalogMode()  do { ANSELDbits.ANSD7 = 1; } while(0)
#define RELAY1_SetDigitalMode() do { ANSELDbits.ANSD7 = 0; } while(0)

/**
   @Param
    none
   @Returns
    none
   @Description
    GPIO and peripheral I/O initialization
   @Example
    PIN_MANAGER_Initialize();
 */
void PIN_MANAGER_Initialize (void);

/**
 * @Param
    none
 * @Returns
    none
 * @Description
    Interrupt on Change Handling routine
 * @Example
    PIN_MANAGER_IOC();
 */
void PIN_MANAGER_IOC(void);



#endif // PIN_MANAGER_H
/**
 End of File
*/