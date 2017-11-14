// Ultrasonic - Library for HR-SC04 Ultrasonic Ranging Module.
// GitHub: https://github.com/JRodrigoTech/Ultrasonic-HC-SR04

#ifndef Ultrasonic_h
#define Ultrasonic_h

#include "driverlib/gpio.h"

#define CM 1
#define INC 0

typedef struct
{
	unsigned long port;
	unsigned char pin;
	unsigned long pinIO;
} Ultrasonic_pin;


void Ultrasonic_Init(Ultrasonic_pin TP, Ultrasonic_pin EP);
long Ultrasonic_Timing();
long Ultrasonic_Ranging(int sys);


#endif
