#include "mcc_generated_files/mcc.h"
#include "tasks.h"
#include "PID.h"

#define OFF 0
#define ON 1

#define DIVIDER_PERIOD_mSeg			4000
#define DIVIDER_PERIOD				(DIVIDER_PERIOD_mSeg * _1mSeg)

#define PELTIER_MIN_DUTY			5
#define PELTIER_MAX_DUTY			80

void TMR2_Tick(void);
inline void TempAcquisition(void);
inline void TempUpdate(void);

uint16_t TempCounter = 0;
float TEMP_FLOAT;
unsigned int DutyPeltier;

inline void InitHardware()
{
    TMR2_SetInterruptHandler(TMR2_Tick);
    
    ADC_SelectChannel(TEMP_SENSOR);
    
    RELAY1_SetHigh();
    RELAY2_SetHigh();
    
    PID_SetPoint((int)(100 * 24));
	PID_SetTunings(0.1, 0.001, 0);
    PID_SetSampleTime(DIVIDER_PERIOD_mSeg);
	PID_SetOutputLimits(PELTIER_MIN_DUTY, PELTIER_MAX_DUTY);
	PID_SetControllerDirection(PID_DIRECT);
	PID_SetMode(PID_AUTOMATIC);
    
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
    
    if (DutyPeltier == 0) 
	{
		PELTIER_DUTY_SetLow(); 
	}
	else
	{
		static unsigned int PeltierCounter;
		if (PeltierCounter++ == 0)
		{
			PELTIER_DUTY_SetHigh();
		}
		else if (PeltierCounter == DutyPeltier)
		{
			PELTIER_DUTY_SetLow();
		}
		else if (PeltierCounter > 100)
		{
			PeltierCounter = 0;
		}
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
    TEMP_FLOAT = (((int)(TEMPERATURA))*5)/1024;
    TEMP_FLOAT = (TEMP_FLOAT/0.08)*100;
    
    DutyPeltier = PID_Control((int)(TEMP_FLOAT));;
    
    TEMPH = 0x00FF*TEMPERATURA;
    TEMPL = (0xFF00*TEMPERATURA)>>8;
    
    EUSART1_Write(1);
    EUSART1_Write(TEMPL);
    EUSART1_Write(TEMPH);
    EUSART1_Write(1);
    
}