//#############################################################################
//
// FILE:   empty_driverlib_main.c
//
//! \addtogroup driver_example_list
//! <h1>Empty Project Example</h1> 
//!
//! This example is an empty project setup for Driverlib development.
//!
//
//#############################################################################
//
//
// $Copyright:
// Copyright (C) 2025 Texas Instruments Incorporated - http://www.ti.com/
//
// Redistribution and use in source and binary forms, with or without 
// modification, are permitted provided that the following conditions 
// are met:
// 
//   Redistributions of source code must retain the above copyright 
//   notice, this list of conditions and the following disclaimer.
// 
//   Redistributions in binary form must reproduce the above copyright
//   notice, this list of conditions and the following disclaimer in the 
//   documentation and/or other materials provided with the   
//   distribution.
// 
//   Neither the name of Texas Instruments Incorporated nor the names of
//   its contributors may be used to endorse or promote products derived
//   from this software without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
// $
//#############################################################################

//
// Included Files
//
#include "driverlib.h"
#include "device.h"
#include "board.h"
#include "c2000ware_libraries.h"
#include "math.h"
#include "stdio.h"

#define PI          3.14159265
#define NUM_SAMPLES 100
#define DAC_MAX     4095.0
#define DAC_REF     3.3

// Desired sine amplitude (3V peak unipolar)
#define SINE_AMPLITUDE_V 3.0
#define SINE_OFFSET_V    0.0  // unipolar (0 to +3V)

volatile uint16_t sineTable[NUM_SAMPLES];
volatile uint16_t index = 0;
uint16_t sine_value;

//
// Main
//
__interrupt void INT_myCPUTIMER0_ISR(void)
{
     
    sine_value = (uint16_t)sineTable[index];
    DAC_setShadowValue(DACA_BASE, sine_value);
    index++;
    if (index >= NUM_SAMPLES) index = 0;
    GPIO_togglePin(0);
    Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP1);

}

void myDAC0_init(){
	//
	// Set DAC reference voltage.
	//
	DAC_setReferenceVoltage(DACA_BASE, DAC_REF_ADC_VREFHI);
	//
	// Set DAC gain mode.
	//
	DAC_setGainMode(DACA_BASE, DAC_GAIN_TWO);
	//
	// Set DAC load mode.
	//
	DAC_setLoadMode(DACA_BASE, DAC_LOAD_SYSCLK);
	//
	// Enable the DAC output
	//
	DAC_enableOutput(DACA_BASE);
	//
	// Set the DAC shadow output
	//
	DAC_setShadowValue(DACA_BASE, 373U);
	//
	// Lock write-access to DAC Register
	//
	DAC_lockRegister(DACA_BASE, (DAC_LOCK_OUTPUT));

	//
	// Delay for buffered DAC to power up.
	//
	DEVICE_DELAY_US(5000);
}
void myCPUTIMER0_init(){
	CPUTimer_setEmulationMode(CPUTIMER0_BASE, CPUTIMER_EMULATIONMODE_STOPAFTERNEXTDECREMENT);
	CPUTimer_setPreScaler(CPUTIMER0_BASE, 1U);
	CPUTimer_setPeriod(CPUTIMER0_BASE, 23900U);
	CPUTimer_enableInterrupt(CPUTIMER0_BASE);
	CPUTimer_stopTimer(CPUTIMER0_BASE);

	CPUTimer_reloadTimerCounter(CPUTIMER0_BASE);
	CPUTimer_startTimer(CPUTIMER0_BASE);

    // Interrupt Settings for INT_myCPUTIMER0
	// ISR need to be defined for the registered interrupts
	Interrupt_register(INT_TIMER0, &INT_myCPUTIMER0_ISR);
	Interrupt_enable(INT_TIMER0);
}
void main(void)
{

    //
    // Initialize device clock and peripherals
    //
    Device_init();

    //
    // Disable pin locks and enable internal pull-ups.
    //
    Device_initGPIO();

    //
    // Initialize PIE and clear PIE registers. Disables CPU interrupts.
    //
    Interrupt_initModule();

    //
    // Initialize the PIE vector table with pointers to the shell Interrupt
    // Service Routines (ISR).
    //
    Interrupt_initVectorTable();

    //
    // PinMux and Peripheral Initialization
    //
    Board_init();

    //
    // C2000Ware Library initialization
    //
    C2000Ware_libraries_init();

    //
    // Enable Global Interrupt (INTM) and real time interrupt (DBGM)
    //
    EINT;
    ERTM;

   for (int i = 0; i < NUM_SAMPLES; i++)
    {
        float theta = 2.0 * PI * i / NUM_SAMPLES;
        float sine_val = (SINE_AMPLITUDE_V / 2.0) * (1 + sinf(theta)); // 0–3V
        sineTable[i] = (uint16_t)((sine_val / DAC_REF) * DAC_MAX);
    }

    while(1)
    {

    }
}

//
// End of File
//
