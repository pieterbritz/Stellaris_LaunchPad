/*
 * main.c
 */
#include <stdint.h>
#include "lm4f120h5qr.h"

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
volatile unsigned long g_ulCounter = 0;
uint8_t oddCount = 3;


//*****************************************************************************
// The interrupt handler for the for Systick interrupt.
//*****************************************************************************
void SysTickIntHandler(void)
{
    // Update the Systick interrupt counter.
    g_ulCounter++;
}


//*****************************************************************************
// The Init Systemtick interrupt.
//*****************************************************************************
static void Init_systemTick(void)
{
    SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN |
                   SYSCTL_XTAL_16MHZ);

    SysTickIntRegister(SysTickIntHandler);

    // Set up the period for the SysTick timer.  The SysTick timer period will
    // be equal to the system clock, resulting in a period of 1 second.
    SysTickPeriodSet(16000);

    // Enable interrupts to the processor.
    IntMasterEnable();

    // Enable the SysTick Interrupt.
    SysTickIntEnable();

    // Enable SysTick.
    SysTickEnable();

    // Configure and enable the SysTick counter.
    SysTickPeriodSet(1000);
    SysTickEnable();
}

int Init_LED()
{
    // General-Purpose Input/Output Run Mode Clock Gating Control (RCGCGPIO), pg. 315
    SYSCTL_RCGCGPIO_R |= 0x20U; // activate the clock for port f

    // GPIO Lock (GPIOLOCK), pg. 645
    GPIO_PORTF_LOCK_R = 0x4C4F434BU; // unlock the lock register

    // GPIO Commit (GPIOCR), pg. 646
    GPIO_PORTF_CR_R = 0xFF; // enable commit for PORT F

    // GPIO Analog Mode Select (GPIOASMSEL), pg. 648
    GPIO_PORTF_AMSEL_R = 0x00U; // disable analog functionality

    //  GPIO Port Control (GPIOPCTL), pg. 649
    GPIO_PORTF_PCTL_R = 0x00000000U; // configure port f as GPIO

    // GPIO Direction (GPIODIR), pg. 624
    GPIO_PORTF_DIR_R = 0x0EU; // 0xE = 01110 make PF3, PF2, and PF1 output

    // GPIO Alternate Function Select (GPIOAFSEL), pg. 633
    GPIO_PORTF_AFSEL_R = 0x00U; // disable alternate functions

    // GPIO Digital Enable (GPIODEN), pg. 643
    GPIO_PORTF_DEN_R = 0xFFU; // enable digital on all pins in PORTF

    return 0;
}

void wait(void)
{
    int i;
    for(i=0;i<1000000;i++);
}

int GetOddCount(void)
{
	if ((oddCount + 2) < 0x0F)
		oddCount += 2;
	else
		oddCount = 3;

	return oddCount;
}

int main(void) {

    Init_systemTick();
    Init_LED();
    //Ultrasonic_Init(trigger, echo);
    unsigned long ulPrevCount = 0;

    while(1)
    {
        // Check to see if systick interrupt count changed, and if so then
        // print a message with the count.
        if(ulPrevCount != g_ulCounter)
        {
            // Print the interrupt counter.
            //UARTprintf("Number of interrupts: %d\r", g_ulCounter);
            ulPrevCount = g_ulCounter;
            if(g_ulCounter %= 2)
            {
                GPIO_PORTF_DATA_R = GetOddCount();
                wait();
            }
            else
            {
                GPIO_PORTF_DATA_R = GetOddCount();
                //GPIO_PORTF_DATA_R &= ~0x0E;
                wait();
            }
        }

        //GPIO_PORTF_DATA_R &= ~0x0E; // 0E = 00001110, so ~0E = 11110001 turn off the three leds
        //wait();
        //GPIO_PORTF_DATA_R |= 0x02; // turn on the red led
        //wait();
    }
    return 0;
}
