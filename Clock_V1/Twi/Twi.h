#ifndef TWI_H_
#define TWI_H_

#include <avr/io.h>
#include <stdint.h>

/* status codes */

#define INVALID_STATE                           0x00

/* master transmitter mode */
#define TWI_START_CONDITION_TRANSMITTED             0x08
#define TWI_REPEATED_START_CONDITION_TRANSMITTED    0x10
#define TWI_SLA_W_ACK                               0x18
#define TWI_SLA_W_NACK                              0x20
#define TWI_DATA_SENT_ACK                           0x28
#define TWI_DATA_SENT_NACK                          0x30
#define TWI_ARBITRATION_LOST                        0x38
#define TWI_MASTER_TRANSMIT_OK                      0x01

/* master receiver mode */
#define TWI_START_CONDITION_TRANSMITTED             0x08
#define TWI_REPEATED_START_CONDITION_TRANSMITTED    0x10
#define TWI_ARBITRATION_LOST                        0x38
#define TWI_SLA_R_ACK                               0x40
#define TWI_SLA_R_NACK                              0x48
#define TWI_DATA_RECEIVED_ACK                       0x50
#define TWI_DATA_RECEIVED_NACK                      0x58
#define TWI_MASTER_RECEIVE_OK                       0x02


void TwiInit(void);

void TwiClearInt(void);
void TwiStart(void);
void TwiStop(void);

uint8_t TwiRead(void);
void TwiWrite(uint8_t byte);

#endif
