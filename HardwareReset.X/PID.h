// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef TC_PID_HEADER_TEMPLATE_H
#define	TC_PID_HEADER_TEMPLATE_H

#define PID_MANUAL		0
#define PID_AUTOMATIC	1

#define PID_DIRECT		0
#define PID_REVERSE		1

typedef struct {
    volatile int Min;
    volatile int Max;

    volatile double Kp;
    volatile double Ki;
    volatile double Kd;
    
    volatile double SetPoint;
} tPID;

int PID_Control(int);
void PID_SetPoint(int);
void PID_SetTunings(double, double, double);
void PID_SetSampleTime(int);
void PID_SetOutputLimits(int, int);
void PID_SetControllerDirection(int);
void PID_SetMode(int);
void PID_Initialize(void);

extern tPID PIDConfig;

#endif	/* TC_PID_HEADER_TEMPLATE_H */
