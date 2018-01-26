#include "mcc_generated_files/mcc.h"
#include "tasks.h"

#define OFF 0
#define ON 1


void TMR2_Tick(void);
inline void TempAcquisition(void);
inline void TempUpdate(void);

uint16_t TempCounter = 0;

inline void InitHardware()
{
    TMR2_SetInterruptHandler(TMR2_Tick);
    
    ADC_SelectChannel(TEMP_SENSOR);
    
    RELAY1_SetHigh();
    RELAY2_SetHigh();
    
    ADC_StartConversion();
    
}

inline void MainTask()
{
    TempUpdate();
}

void TMR2_Tick(void)
{
    TempCounter++;
    static volatile unsigned uint16_t seg = 0;
    static volatile unsigned uint16_t min = 0;
    static volatile unsigned uint16_t Hard_ON = 0;
    static volatile unsigned uint16_t Hard_OFF = 0;
    static volatile unsigned uint16_t State = OFF;
    
    if (++seg >= 1000)
        {   
            LED_TEST_Toggle();  
            if ((++Hard_OFF >= 10))
            {
                if (State == OFF)
                {
                    RELAY2_SetLow();
                    RELAY1_SetLow();
                    State = ON;
                }
                Hard_OFF = 0;
                
            }
            if (++min >= 60)
            {
                if ((++Hard_ON >= 60))
                {
                    Hard_ON=0;
                    if(State == ON)
                    {
                        State = OFF;
                        RELAY2_SetHigh();
                        RELAY1_SetHigh();
                    }   
                }
                min = 0;
            }
            seg = 0;
        }
}

inline void TempUpdate(void)
{
    if (TempCounter > _1Seg)
    {
        TempAcquisition();
        TempCounter = 0;
    }
}
inline void TempAcquisition(void)
{
    static uint16_t TEMPERATURA=0;
    static uint16_t TEMPH=0;
    static uint16_t TEMPL=0;
    
    TEMPERATURA = ADC_GetConversion(TEMP_SENSOR);
    
    TEMPH = 0x00FF*TEMPERATURA;
    TEMPL = (0xFF00*TEMPERATURA)>>8;
    
    EUSART1_Write(1);
    EUSART1_Write(TEMPL);
    EUSART1_Write(TEMPH);
    EUSART1_Write(1);
    
}