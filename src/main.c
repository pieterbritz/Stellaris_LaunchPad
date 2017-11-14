/*
 * main.c
 */
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/systick.h"
#include "driverlib/interrupt.h"
#include "stdio.h"
#include "Ultrasonic.h"

Ultrasonic_pin trigger = {GPIO_PORTB_BASE, GPIO_PIN_0, GPIO_DIR_MODE_OUT};
Ultrasonic_pin echo = {GPIO_PORTB_BASE, GPIO_PIN_1, GPIO_DIR_MODE_IN};
unsigned long g_ulCounter = 0;


//*****************************************************************************
//
// The interrupt handler for the for Systick interrupt.
//
//*****************************************************************************
void SysTickIntHandler(void)
{
    //
    // Update the Systick interrupt counter.
    //
    g_ulCounter++;
}


//*****************************************************************************
//
// The Init Systemtick interrupt.
//
//*****************************************************************************
static void Init_systemTick(void)
{
    SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN |
                   SYSCTL_XTAL_16MHZ);

    SysTickIntRegister(SysTickIntHandler);
    //
    // Set up the period for the SysTick timer.  The SysTick timer period will
    // be equal to the system clock, resulting in a period of 1 second.
    //
    SysTickPeriodSet(16000);

    //
    // Enable interrupts to the processor.
    //
    IntMasterEnable();

    //
    // Enable the SysTick Interrupt.
    //
    SysTickIntEnable();

    //
    // Enable SysTick.
    //
    SysTickEnable();


    //
    // Configure and enable the SysTick counter.
    //
    SysTickPeriodSet(1000);
    SysTickEnable();
}


int main(void) {

    Init_systemTick();
    Ultrasonic_Init(trigger, echo);
    unsigned long ulPrevCount = 0;

    while(1)
    {
        //
        // Check to see if systick interrupt count changed, and if so then
        // print a message with the count.
        //
        if(ulPrevCount != g_ulCounter)
        {
            //
            // Print the interrupt counter.
            //
            //UARTprintf("Number of interrupts: %d\r", g_ulCounter);
            ulPrevCount = g_ulCounter;
        }
    }
    return 0;
}
