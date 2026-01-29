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


//
// Globals
//
uint16_t myADC0Result0;

__interrupt void INT_myCPUTIMER0_ISR(void)
{
        Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP1);

}
__interrupt void ADC_INTERRUPT(void)
{
    myADC0Result0 = ADC_readResult(ADCARESULT_BASE, ADC_SOC_NUMBER0);
    //
    // Clear the interrupt flag
    //
    ADC_clearInterruptStatus(ADCA_BASE, ADC_INT_NUMBER1);

    //
    // Check if overflow has occurred
    //
    if(true == ADC_getInterruptOverflowStatus(ADCA_BASE, ADC_INT_NUMBER1))
    {
        ADC_clearInterruptOverflowStatus(ADCA_BASE, ADC_INT_NUMBER1);
        ADC_clearInterruptStatus(ADCA_BASE, ADC_INT_NUMBER1);
    }

    //
    // Acknowledge the interrupt
    //
    Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP1);
}
//
// Main
//
void myCPUTIMER0_init(){
	CPUTimer_setEmulationMode(CPUTIMER0_BASE, CPUTIMER_EMULATIONMODE_STOPAFTERNEXTDECREMENT);
	CPUTimer_setPreScaler(CPUTIMER0_BASE, 1U);
	CPUTimer_setPeriod(CPUTIMER0_BASE, 1500U);
	CPUTimer_enableInterrupt(CPUTIMER0_BASE);
	CPUTimer_stopTimer(CPUTIMER0_BASE);

	CPUTimer_reloadTimerCounter(CPUTIMER0_BASE);
	CPUTimer_startTimer(CPUTIMER0_BASE);
}
void INTERRUPT_init(){
	
	// Interrupt Settings for INT_ADCA1
	// ISR need to be defined for the registered interrupts
	Interrupt_register(INT_ADCA1, &ADC_INTERRUPT);
	Interrupt_enable(INT_ADCA1);
	
	// Interrupt Settings for INT_TIMER0
	// ISR need to be defined for the registered interrupts
	Interrupt_register(INT_TIMER0, &INT_myCPUTIMER0_ISR);
	Interrupt_enable(INT_TIMER0);
}

void myADC0_init(){
	
	ADC_setVREF(ADCA_BASE, ADC_REFERENCE_INTERNAL, ADC_REFERENCE_3_3V);
	ADC_setPrescaler(ADCA_BASE, ADC_CLK_DIV_2_0);
	ADC_setInterruptPulseMode(ADCA_BASE, ADC_PULSE_END_OF_CONV);
	ADC_enableConverter(ADCA_BASE);
	DEVICE_DELAY_US(5000);

	ADC_disableBurstMode(ADCA_BASE);
	ADC_setSOCPriority(ADCA_BASE, ADC_PRI_ALL_ROUND_ROBIN);

	
    ADC_setupSOC(ADCA_BASE, ADC_SOC_NUMBER0, ADC_TRIGGER_CPU1_TINT0, ADC_CH_ADCIN2, 12U);
	ADC_setInterruptSOCTrigger(ADCA_BASE, ADC_SOC_NUMBER0, ADC_INT_SOC_TRIGGER_NONE);

    
	ADC_setInterruptSource(ADCA_BASE, ADC_INT_NUMBER1, ADC_SOC_NUMBER0);
	ADC_clearInterruptStatus(ADCA_BASE, ADC_INT_NUMBER1);
	ADC_disableContinuousMode(ADCA_BASE, ADC_INT_NUMBER1);
	ADC_enableInterrupt(ADCA_BASE, ADC_INT_NUMBER1);
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

    myADC0_init();
    myCPUTIMER0_init();
    INTERRUPT_init();


    while(1)
    {



    }
}

//
// End of File
//
