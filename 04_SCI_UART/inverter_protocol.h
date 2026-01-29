#ifndef INVERTER_PROTOCOL_H_
#define INVERTER_PROTOCOL_H_



static void append16(uint16_t *idx, uint16_t val);
static void append32(uint16_t *idx, uint32_t val);
void BuildInverterFrame(void);
void ConvertFrame8to16(void);
void SendInverterFrame(void);
void ProtocolTask(void);



#endif /*INVERTER_PROTOCOL_H_*/
