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

#define EPWM1_A 0
#define EPWM1_B 1

//
// Globals
//
uint16_t myADC0Result0;


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

void INTERRUPT_init(){
	
	// Interrupt Settings for INT_ADCA1
	// ISR need to be defined for the registered interrupts
	Interrupt_register(INT_ADCA1, &ADC_INTERRUPT);
	Interrupt_enable(INT_ADCA1);

}

void myADC0_init(){
	
	ADC_setVREF(ADCA_BASE, ADC_REFERENCE_INTERNAL, ADC_REFERENCE_3_3V);
	ADC_setPrescaler(ADCA_BASE, ADC_CLK_DIV_2_0);
	ADC_setInterruptPulseMode(ADCA_BASE, ADC_PULSE_END_OF_CONV);
	ADC_enableConverter(ADCA_BASE);
	DEVICE_DELAY_US(5000);

	ADC_disableBurstMode(ADCA_BASE);
	ADC_setSOCPriority(ADCA_BASE, ADC_PRI_ALL_ROUND_ROBIN);

	
    ADC_setupSOC(ADCA_BASE, ADC_SOC_NUMBER0, ADC_TRIGGER_EPWM1_SOCA, ADC_CH_ADCIN2, 12U);
	ADC_setInterruptSOCTrigger(ADCA_BASE, ADC_SOC_NUMBER0, ADC_INT_SOC_TRIGGER_NONE);

    	//
	// PPB Configuration: Configure high and low limits detection for ADCPPB
	//
	// Post Processing Block 1 Configuration
	// 		Configures a post-processing block (PPB) in the ADC.
	// 		PPB Number				: 1
	// 		SOC/EOC number			: 0
	// 		Events Sources Enabled	: ADC_EVT_TRIPHI,ADC_EVT_TRIPLO
	// 		Calibration Offset		: 0
	// 		Reference Offset		: 0
	// 		Two's Complement		: Disabled
	// 		Trip High Limit			: 3000
	// 		Trip Low Limit			: 1000
	// 		Clear PPB Event Flags	: Enabled
	//
	ADC_setupPPB(ADCA_BASE, ADC_PPB_NUMBER1, ADC_SOC_NUMBER0);
	ADC_enablePPBEvent(ADCA_BASE, ADC_PPB_NUMBER1, (ADC_EVT_TRIPHI | ADC_EVT_TRIPLO));
	ADC_disablePPBEvent(ADCA_BASE, ADC_PPB_NUMBER1, (ADC_EVT_ZERO));
	ADC_disablePPBEventInterrupt(ADCA_BASE, ADC_PPB_NUMBER1, (ADC_EVT_TRIPHI | ADC_EVT_TRIPLO | ADC_EVT_ZERO));
	ADC_setPPBCalibrationOffset(ADCA_BASE, ADC_PPB_NUMBER1, 0);
	ADC_setPPBReferenceOffset(ADCA_BASE, ADC_PPB_NUMBER1, 0);
	ADC_disablePPBTwosComplement(ADCA_BASE, ADC_PPB_NUMBER1);
	ADC_setPPBTripLimits(ADCA_BASE, ADC_PPB_NUMBER1, 3000, 1000);
	ADC_enablePPBEventCBCClear(ADCA_BASE, ADC_PPB_NUMBER1);

    
	ADC_setInterruptSource(ADCA_BASE, ADC_INT_NUMBER1, ADC_SOC_NUMBER0);
	ADC_clearInterruptStatus(ADCA_BASE, ADC_INT_NUMBER1);
	ADC_disableContinuousMode(ADCA_BASE, ADC_INT_NUMBER1);
	ADC_enableInterrupt(ADCA_BASE, ADC_INT_NUMBER1);
}
void XBAR_TRIP4_init(){
		
	// XBAR_setEPWMMuxConfig(XBAR_TRIP4, XBAR_EPWM_MUX00_ADCAEVT1);
	// XBAR_setEPWMMuxConfig(XBAR_TRIP4, XBAR_EPWM_MUX02_ADCAEVT2);
	// XBAR_setEPWMMuxConfig(XBAR_TRIP4, XBAR_EPWM_MUX04_ADCAEVT3);
	// XBAR_setEPWMMuxConfig(XBAR_TRIP4, XBAR_EPWM_MUX06_ADCAEVT4);
	// XBAR_enableEPWMMux(XBAR_TRIP4, XBAR_MUX00 | XBAR_MUX02 | XBAR_MUX04 | XBAR_MUX06);

    XBAR_setEPWMMuxConfig(XBAR_TRIP4, XBAR_EPWM_MUX00_ADCAEVT1);
	XBAR_enableEPWMMux(XBAR_TRIP4, XBAR_MUX00);

}
void EPWM_init(void)
{
    //
	// EPWM1 -> myEPWM1 Pinmux
	//
	GPIO_setPinConfig(GPIO_0_EPWM1_A);
	GPIO_setPadConfig(EPWM1_A, GPIO_PIN_TYPE_STD);
	GPIO_setQualificationMode(EPWM1_A, GPIO_QUAL_SYNC);

	GPIO_setPinConfig(GPIO_1_EPWM1_B);
	GPIO_setPadConfig(EPWM1_B, GPIO_PIN_TYPE_STD);
	GPIO_setQualificationMode(EPWM1_B, GPIO_QUAL_SYNC);

    EPWM_setClockPrescaler(EPWM1_BASE, EPWM_CLOCK_DIVIDER_1, EPWM_HSCLOCK_DIVIDER_1);	
    EPWM_setTimeBasePeriod(EPWM1_BASE, 2400);	
    EPWM_setTimeBaseCounter(EPWM1_BASE, 0);

    EPWM_setTimeBaseCounterMode(EPWM1_BASE, EPWM_COUNTER_MODE_UP_DOWN);	

    EPWM_setCounterCompareValue(EPWM1_BASE, EPWM_COUNTER_COMPARE_A, 1200);	
    EPWM_setCounterCompareShadowLoadMode(EPWM1_BASE, EPWM_COUNTER_COMPARE_A, EPWM_COMP_LOAD_ON_CNTR_ZERO);

    EPWM_setCounterCompareValue(EPWM1_BASE, EPWM_COUNTER_COMPARE_B, 1200);	
    EPWM_setCounterCompareShadowLoadMode(EPWM1_BASE, EPWM_COUNTER_COMPARE_B, EPWM_COMP_LOAD_ON_CNTR_ZERO);

    EPWM_setActionQualifierShadowLoadMode(EPWM1_BASE, EPWM_ACTION_QUALIFIER_A, EPWM_AQ_LOAD_ON_CNTR_ZERO);


    EPWM_setActionQualifierAction(EPWM1_BASE, EPWM_AQ_OUTPUT_A, EPWM_AQ_OUTPUT_LOW, EPWM_AQ_OUTPUT_ON_TIMEBASE_UP_CMPA);	
    EPWM_setActionQualifierAction(EPWM1_BASE, EPWM_AQ_OUTPUT_A, EPWM_AQ_OUTPUT_HIGH, EPWM_AQ_OUTPUT_ON_TIMEBASE_DOWN_CMPA);	

    EPWM_setActionQualifierAction(EPWM1_BASE, EPWM_AQ_OUTPUT_A, EPWM_AQ_OUTPUT_HIGH, EPWM_AQ_OUTPUT_ON_T1_COUNT_UP);	
    EPWM_setActionQualifierAction(EPWM1_BASE, EPWM_AQ_OUTPUT_A, EPWM_AQ_OUTPUT_HIGH, EPWM_AQ_OUTPUT_ON_T1_COUNT_DOWN);	
    EPWM_setActionQualifierAction(EPWM1_BASE, EPWM_AQ_OUTPUT_A, EPWM_AQ_OUTPUT_HIGH, EPWM_AQ_OUTPUT_ON_T2_COUNT_UP);	
    EPWM_setActionQualifierAction(EPWM1_BASE, EPWM_AQ_OUTPUT_A, EPWM_AQ_OUTPUT_HIGH, EPWM_AQ_OUTPUT_ON_T2_COUNT_DOWN);	

    EPWM_setActionQualifierShadowLoadMode(EPWM1_BASE, EPWM_ACTION_QUALIFIER_B, EPWM_AQ_LOAD_ON_CNTR_ZERO);
    	

    EPWM_setActionQualifierAction(EPWM1_BASE, EPWM_AQ_OUTPUT_B, EPWM_AQ_OUTPUT_LOW, EPWM_AQ_OUTPUT_ON_TIMEBASE_UP_CMPB);	
    EPWM_setActionQualifierAction(EPWM1_BASE, EPWM_AQ_OUTPUT_B, EPWM_AQ_OUTPUT_HIGH, EPWM_AQ_OUTPUT_ON_TIMEBASE_DOWN_CMPB);	

    EPWM_setActionQualifierAction(EPWM1_BASE, EPWM_AQ_OUTPUT_B, EPWM_AQ_OUTPUT_LOW, EPWM_AQ_OUTPUT_ON_T1_COUNT_UP);	
    EPWM_setActionQualifierAction(EPWM1_BASE, EPWM_AQ_OUTPUT_B, EPWM_AQ_OUTPUT_LOW, EPWM_AQ_OUTPUT_ON_T1_COUNT_DOWN);	
    EPWM_setActionQualifierAction(EPWM1_BASE, EPWM_AQ_OUTPUT_B, EPWM_AQ_OUTPUT_LOW, EPWM_AQ_OUTPUT_ON_T2_COUNT_UP);	
    EPWM_setActionQualifierAction(EPWM1_BASE, EPWM_AQ_OUTPUT_B, EPWM_AQ_OUTPUT_LOW, EPWM_AQ_OUTPUT_ON_T2_COUNT_DOWN);	

    EPWM_setRisingEdgeDelayCountShadowLoadMode(EPWM1_BASE, EPWM_RED_LOAD_ON_CNTR_ZERO);	
    EPWM_disableRisingEdgeDelayCountShadowLoadMode(EPWM1_BASE);	
    EPWM_setFallingEdgeDelayCountShadowLoadMode(EPWM1_BASE, EPWM_FED_LOAD_ON_CNTR_ZERO);	
    EPWM_disableFallingEdgeDelayCountShadowLoadMode(EPWM1_BASE);	

    EPWM_setTripZoneAction(EPWM1_BASE, EPWM_TZ_ACTION_EVENT_TZA, EPWM_TZ_ACTION_HIGH);	
    EPWM_setTripZoneAction(EPWM1_BASE, EPWM_TZ_ACTION_EVENT_TZB, EPWM_TZ_ACTION_HIGH);	
    EPWM_setTripZoneAction(EPWM1_BASE, EPWM_TZ_ACTION_EVENT_DCAEVT1, EPWM_TZ_ACTION_HIGH);	
    EPWM_setTripZoneAction(EPWM1_BASE, EPWM_TZ_ACTION_EVENT_DCAEVT2, EPWM_TZ_ACTION_HIGH);	
    EPWM_setTripZoneAction(EPWM1_BASE, EPWM_TZ_ACTION_EVENT_DCBEVT1, EPWM_TZ_ACTION_LOW);	
    EPWM_setTripZoneAction(EPWM1_BASE, EPWM_TZ_ACTION_EVENT_DCBEVT2, EPWM_TZ_ACTION_LOW);	

    EPWM_enableTripZoneSignals(EPWM1_BASE, EPWM_TZ_SIGNAL_CBC4);	

    EPWM_enableTripZoneInterrupt(EPWM1_BASE, EPWM_TZ_INTERRUPT_CBC);	

    EPWM_selectDigitalCompareTripInput(EPWM1_BASE, EPWM_DC_TRIP_TRIPIN4, EPWM_DC_TYPE_DCAH);	
    EPWM_selectDigitalCompareTripInput(EPWM1_BASE, EPWM_DC_TRIP_TRIPIN4, EPWM_DC_TYPE_DCAL);

    EPWM_setTripZoneDigitalCompareEventCondition(EPWM1_BASE, EPWM_TZ_DC_OUTPUT_A1, EPWM_TZ_EVENT_DCXH_HIGH);	
    EPWM_setTripZoneDigitalCompareEventCondition(EPWM1_BASE, EPWM_TZ_DC_OUTPUT_A2, EPWM_TZ_EVENT_DCXH_HIGH);	

    EPWM_selectDigitalCompareTripInput(EPWM1_BASE, EPWM_DC_TRIP_TRIPIN4, EPWM_DC_TYPE_DCBH);	
    EPWM_selectDigitalCompareTripInput(EPWM1_BASE, EPWM_DC_TRIP_TRIPIN4, EPWM_DC_TYPE_DCBL);	

    EPWM_setTripZoneDigitalCompareEventCondition(EPWM1_BASE, EPWM_TZ_DC_OUTPUT_B1, EPWM_TZ_EVENT_DCXH_HIGH);	
    EPWM_setTripZoneDigitalCompareEventCondition(EPWM1_BASE, EPWM_TZ_DC_OUTPUT_B2, EPWM_TZ_EVENT_DCXH_HIGH);	

    EPWM_enableADCTrigger(EPWM1_BASE, EPWM_SOC_A);	
    EPWM_setADCTriggerSource(EPWM1_BASE, EPWM_SOC_A, EPWM_SOC_TBCTR_ZERO);	
    EPWM_setADCTriggerEventPrescale(EPWM1_BASE, EPWM_SOC_A, 2);	

}



//
// Main
//
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
    XBAR_TRIP4_init();
    EPWM_init();
    //
    // Start ePWM1, enabling SOCA and putting the counter in up-count mode
    //
    EPWM_enableADCTrigger(EPWM1_BASE, EPWM_SOC_A);
    EPWM_setTimeBaseCounterMode(EPWM1_BASE, EPWM_COUNTER_MODE_UP_DOWN);


    INTERRUPT_init();


    while(1)
    {



    }
}

//
// End of File
//
