// Ultrasonic - Library for HR-SC04 Ultrasonic Ranging Module.
// GitHub: https://github.com/JRodrigoTech/Ultrasonic-HC-SR04

#include <stdint.h>
#include "Ultrasonic.h"
#include "gpio.h"
#include "inc/hw_memmap.h"
#include "inc/hw_timer.h"
#include "inc/hw_ints.h"
#include "driverlib/timer.h"
#include "driverlib/interrupt.h"
#include "driverlib/sysctl.h"

#define LOW     0
#define HIGH    1

static Ultrasonic_pin Trig_pin;
static Ultrasonic_pin Echo_pin;
static unsigned long Time_out;
static unsigned long duration,distance_cm,distance_inc;

static unsigned long g_ulIntFlag = 0;

void Timer0BIntHandler(void)
{
    // Clear the timer interrupt flag.
    TimerIntClear(TIMER0_BASE, TIMER_TIMB_TIMEOUT);

    // Set a flag to indicate that the interrupt occurred.
    g_ulIntFlag = 1;

    // Deregister Timer 0 B Interupt handler.
    TimerIntUnregister(TIMER0_BASE, TIMER_B);
}

void delayMicroseconds(unsigned int delay)
{
	TimerIntRegister(TIMER0_BASE, TIMER_B, Timer0BIntHandler);

    // The Timer0 peripheral must be enabled for use.
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);

    // The Timer0 peripheral must be enabled for use.
    TimerConfigure(TIMER0_BASE, TIMER_CFG_16_BIT_PAIR |
                   TIMER_CFG_B_ONE_SHOT);

    // Set the Timer0B load value to 1us.
    TimerLoadSet(TIMER0_BASE, TIMER_B, (SysCtlClockGet() / 1000000) * delay);

    // Enable processor interrupts.
    IntMasterEnable();

    // Configure the Timer0B interrupt for timer timeout.
    TimerIntEnable(TIMER0_BASE, TIMER_TIMB_TIMEOUT);

    // Enable the Timer0B interrupt on the processor (NVIC).
    IntEnable(INT_TIMER0B);

    // Enable Timer0B.
    TimerEnable(TIMER0_BASE, TIMER_B);

    // Wait for interrupt to occur
    while(!g_ulIntFlag)
    {
    }
}


static long pulseIn(Ultrasonic_pin pin, uint8_t level, unsigned long timeout)
{

    return 0;
}


void Ultrasonic_Init(Ultrasonic_pin TP, Ultrasonic_pin EP)
{
    SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN |
                   SYSCTL_XTAL_16MHZ);

    GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_0);
    GPIOPinTypeGPIOInput(GPIO_PORTB_BASE, GPIO_PIN_1);
    Trig_pin=TP;
    Echo_pin=EP;
    Time_out=3000;  // 3000 µs = 50cm // 30000 µs = 5 m

}

long Ultrasonic_Timing()
{
    GPIOPinWrite(Trig_pin.port, Trig_pin.pin, LOW);
    delayMicroseconds(2);
    GPIOPinWrite(Trig_pin.port, Trig_pin.pin, HIGH);
    delayMicroseconds(10);
    GPIOPinWrite(Trig_pin.port, Trig_pin.pin, LOW);
    duration = pulseIn(Echo_pin,HIGH,Time_out);
    if ( duration == 0 )
    {
        duration = Time_out;
    }
    return duration;
}

long Ultrasonic_Ranging(int sys)
{
    Ultrasonic_Timing();
    if (sys)
    {
        distance_cm = duration /29 / 2 ;
        return distance_cm;
    }
    else
    {
        distance_inc = duration / 74 / 2;
        return distance_inc;
    }
}
