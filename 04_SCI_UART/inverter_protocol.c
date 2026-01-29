//===============================================================
// FILE: inverter_protocol_minimal.c
// MCU: TI F280039C (C2000 DriverLib)
// Purpose: Minimal Inverter Telemetry Binary Frame with Boost Temp
// Frame: [0x5A 0x5A] + DATA + [0xA5 0xA5]
//===============================================================

#include "driverlib.h"
#include "device.h"

//===============================================================
// FRAME CONSTANTS
//===============================================================
#define FRAME_START_1   0x5A
#define FRAME_START_2   0x5A
#define FRAME_END_1     0xA5
#define FRAME_END_2     0xA5

#define FRAME_MAX_SIZE  256

//===============================================================
// GLOBAL TELEMETRY PARAMETERS (RAW SCALED VALUES)
//===============================================================

// Power (Scale = 1 W)
uint16_t gGridPower   = 1000;  // 1000 W
uint16_t gOutputPower = 950;   // 950 W
uint16_t gPowerLoad   = 900;   // 900 W

// R Phase (Voltage scale=0.1V, Current scale=0.01A, Freq scale=0.01Hz, Power scale=1W)
uint16_t gR_GridVoltage = 2300;  // 230.0 V
uint16_t gR_GridCurrent = 100;   // 1.00 A
uint16_t gR_GridFreq    = 5000;  // 50.00 Hz
uint16_t gR_OutputPower = 300;   // 300 W

// S Phase
uint16_t gS_GridVoltage = 2310;  // 231.0 V
uint16_t gS_GridCurrent = 110;   // 1.10 A
uint16_t gS_GridFreq    = 5000;  // 50.00 Hz
uint16_t gS_OutputPower = 320;   // 320 W

// T Phase
uint16_t gT_GridVoltage = 2290;  // 229.0 V
uint16_t gT_GridCurrent = 105;   // 1.05 A
uint16_t gT_GridFreq    = 5000;  // 50.00 Hz
uint16_t gT_OutputPower = 330;   // 330 W

// PV Inputs (Voltage scale=0.1V, Current scale=0.01A, Power scale=1W)
uint16_t gPV1_Voltage = 4000;  // 400.0 V
uint16_t gPV1_Current = 200;   // 2.00 A
uint16_t gPV1_Power   = 800;   // 800 W

uint16_t gPV2_Voltage = 4100;  // 410.0 V
uint16_t gPV2_Current = 180;   // 1.80 A
uint16_t gPV2_Power   = 740;   // 740 W

uint16_t gPV3_Voltage = 4200;  // 420.0 V
uint16_t gPV3_Current = 150;   // 1.50 A
uint16_t gPV3_Power   = 630;   // 630 W

uint16_t gPV4_Voltage = 4300;  // 430.0 V
uint16_t gPV4_Current = 170;   // 1.70 A
uint16_t gPV4_Power   = 730;   // 730 W

// Temperatures (Scale = 0.1 °C)
uint16_t gChargerTemp  = 350;  // 35.0 °C
uint16_t gInverterTemp = 400;  // 40.0 °C
uint16_t gAmbientTemp  = 300;  // 30.0 °C
uint16_t gBoostTemp    = 450;  // 45.0 °C  <-- New Boost Temperature

// Energy Counters (Scale = 1 Wh)
uint32_t gDayGeneration   = 10000;   // 10 kWh
uint32_t gTotalGeneration = 5000000; // 5000 kWh

// Error List (bitmask or error codes)
uint16_t gErrorList[8] = {0};

// Extra Bytes Counter
uint8_t gExtraBytes = 0;

//===============================================================
// FRAME BUFFERS
//===============================================================
uint8_t  frame8[FRAME_MAX_SIZE];
uint16_t frame16[FRAME_MAX_SIZE];
uint16_t frameLength = 0;

//===============================================================
// UTILITY FUNCTIONS
//===============================================================
static void append16(uint16_t *idx, uint16_t val)
{
    frame8[(*idx)++] = (val >> 8) & 0xFF;
    frame8[(*idx)++] = val & 0xFF;
}

static void append32(uint16_t *idx, uint32_t val)
{
    frame8[(*idx)++] = (val >> 24) & 0xFF;
    frame8[(*idx)++] = (val >> 16) & 0xFF;
    frame8[(*idx)++] = (val >> 8)  & 0xFF;
    frame8[(*idx)++] = val & 0xFF;
}

//===============================================================
// BUILD FRAME
//===============================================================
void BuildInverterFrame(void)
{
    uint16_t i;
    uint16_t idx = 0;

    // Start Frame
    frame8[idx++] = FRAME_START_1;
    frame8[idx++] = FRAME_START_2;

    // Power
    append16(&idx, gGridPower);   // 1 W
    append16(&idx, gOutputPower); // 1 W
    append16(&idx, gPowerLoad);   // 1 W

    // R Phase
    append16(&idx, gR_GridVoltage);  // 0.1 V
    append16(&idx, gR_GridCurrent);  // 0.01 A
    append16(&idx, gR_GridFreq);     // 0.01 Hz
    append16(&idx, gR_OutputPower);  // 1 W

    // S Phase
    append16(&idx, gS_GridVoltage);  // 0.1 V
    append16(&idx, gS_GridCurrent);  // 0.01 A
    append16(&idx, gS_GridFreq);     // 0.01 Hz
    append16(&idx, gS_OutputPower);  // 1 W

    // T Phase
    append16(&idx, gT_GridVoltage);  // 0.1 V
    append16(&idx, gT_GridCurrent);  // 0.01 A
    append16(&idx, gT_GridFreq);     // 0.01 Hz
    append16(&idx, gT_OutputPower);  // 1 W

    // PV Inputs
    append16(&idx, gPV1_Voltage); // 0.1 V
    append16(&idx, gPV1_Current); // 0.01 A
    append16(&idx, gPV1_Power);   // 1 W

    append16(&idx, gPV2_Voltage); // 0.1 V
    append16(&idx, gPV2_Current); // 0.01 A
    append16(&idx, gPV2_Power);   // 1 W

    append16(&idx, gPV3_Voltage); // 0.1 V
    append16(&idx, gPV3_Current); // 0.01 A
    append16(&idx, gPV3_Power);   // 1 W

    append16(&idx, gPV4_Voltage); // 0.1 V
    append16(&idx, gPV4_Current); // 0.01 A
    append16(&idx, gPV4_Power);   // 1 W

    // Temperatures
    append16(&idx, gChargerTemp);  // 0.1 °C
    append16(&idx, gInverterTemp); // 0.1 °C
    append16(&idx, gAmbientTemp);  // 0.1 °C
    append16(&idx, gBoostTemp);    // 0.1 °C  <-- Added Boost Temp

    // Energy Counters
    append32(&idx, gDayGeneration);   // 1 Wh
    append32(&idx, gTotalGeneration); // 1 Wh

    // Error List
    for(i = 0; i < 8; i++) append16(&idx, gErrorList[i]);

    // Extra bytes count
    frame8[idx++] = gExtraBytes;

    // End Frame
    frame8[idx++] = FRAME_END_1;
    frame8[idx++] = FRAME_END_2;

    frameLength = idx;
}

//===============================================================
// Convert to 16-bit for SCI
//===============================================================
void ConvertFrame8to16(void)
{
    for(uint16_t i = 0; i < frameLength; i++)
        frame16[i] = frame8[i];
}

//===============================================================
// SEND FRAME
//===============================================================
void SendInverterFrame(void)
{
    ConvertFrame8to16();
    SCI_writeCharArray(SCIA_BASE, frame16, frameLength);
    while(SCI_isTransmitterBusy(SCIA_BASE));
}

//===============================================================
// MAIN PROTOCOL TASK
//===============================================================
void ProtocolTask(void)
{
    BuildInverterFrame();
    SendInverterFrame();
}
