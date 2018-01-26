/*
 * File:   devices.c
 * Author: Nico Christie
 *
 * Created on 02 de noviembre de 2016, 14:40
 */

// http://brettbeauregard.com/blog/2011/04/improving-the-beginners-pid-direction/

#include "PID.h"

tPID PIDConfig;

// working variables 
//unsigned long lastTime;
int Input, Output;
double ITerm, lastInput;
int SampleTime = 1000; // 1 sec
int controllerDirection = PID_DIRECT;
unsigned char inAuto = 0;

int PID_Control(int newInput)
{
	double error, dInput;
	
	Input = newInput;
	if(!inAuto) return PIDConfig.Min;
	//unsigned long now = lastTime + SampleTime;
	//int timeChange = (now - lastTime);
	//if (timeChange >= SampleTime)
	{
		// Compute all the working error variables
		error = PIDConfig.SetPoint - Input;
		ITerm += (PIDConfig.Ki * error);
		if (ITerm > PIDConfig.Max) ITerm = PIDConfig.Max;
		else if (ITerm < PIDConfig.Min) ITerm = PIDConfig.Min;
		dInput = (Input - lastInput);
		
		// Compute PID Output
		Output = (int)(PIDConfig.Kp * error + ITerm - PIDConfig.Kd * dInput);
		if (Output > PIDConfig.Max) Output = PIDConfig.Max;
		else if(Output < PIDConfig.Min) Output = PIDConfig.Min;
		
		// Remember some variables for next time
		lastInput = Input;
		//lastTime = now;
	}
	return (PIDConfig.Max - Output);
}

void PID_SetPoint(int newSetPoint)
{
	PIDConfig.SetPoint = newSetPoint;
}
void PID_SetTunings(double Kp, double Ki, double Kd)
{
	double SampleTimeInSec;
	
	if ((Kp < 0) || (Ki < 0) || (Kd < 0)) return;
	
	SampleTimeInSec = ((double)SampleTime)/1000;
	PIDConfig.Kp = Kp;
	PIDConfig.Ki = Ki * SampleTimeInSec;
	PIDConfig.Kd = Kd / SampleTimeInSec;
	
	if(controllerDirection == PID_REVERSE)
	{
		PIDConfig.Kp = (0 - PIDConfig.Kp);
		PIDConfig.Ki = (0 - PIDConfig.Ki);
		PIDConfig.Kd = (0 - PIDConfig.Kd);
	}
}
void PID_SetSampleTime(int newSampleTime)
{
	if (newSampleTime > 0)
	{
		double ratio = (double)newSampleTime / (double)SampleTime;
		PIDConfig.Ki *= ratio;
		PIDConfig.Kd /= ratio;
		SampleTime = (unsigned long)newSampleTime;
	}
}
void PID_SetOutputLimits(int Min, int Max)
{
	if(Min > Max) return;
	PIDConfig.Min = Min;
	PIDConfig.Max = Max;
	
	if (Output > PIDConfig.Max) Output = PIDConfig.Max;
	else if (Output < PIDConfig.Min) Output = PIDConfig.Min;
	
	if (ITerm > PIDConfig.Max) ITerm= PIDConfig.Max;
	else if (ITerm < PIDConfig.Min) ITerm= PIDConfig.Min;
}
void PID_SetControllerDirection(int Direction)
{
	controllerDirection = Direction;
}
void PID_SetMode(int Mode)
{
	unsigned char newAuto = (Mode == PID_AUTOMATIC);
	if (newAuto == !inAuto)
	{
		//we just went from manual to auto
		PID_Initialize();
	}
	inAuto = newAuto;
}
void PID_Initialize()
{
	lastInput = Input;
	ITerm = Output;
	if (ITerm > PIDConfig.Max) ITerm = PIDConfig.Max;
	else if (ITerm < PIDConfig.Min) ITerm = PIDConfig.Min;
}
