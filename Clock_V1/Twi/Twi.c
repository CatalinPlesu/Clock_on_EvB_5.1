#include "Twi.h"

/* ISR(TWI_vect){ */

/*     // clears twint flag after all operations have been done */
/*     TWCR |= (1 << TWINT); */
/* } */

void TwiInit(void)
{
    PORTC |= (1 << PC0);    // SCL pull up resistor
    PORTC |= (1 << PC1);    // SDA pull up resistor

    TWBR = 250;

    TWCR |= (1 << TWINT); // clears int flag
    TWCR |= (1 << TWEN);  // enable twi
    TWCR |= (1 << TWIE);  // enable twi interrupt
    TWSR |= (1 << TWPS1); // prescaler value 16
}

void TwiClearInt(void)
{
    TWCR |= (1 << TWINT);
}

void TwiStart(void)
{
    TWCR |= (1 << TWSTA);      // start bit
}

void TwiStartClear(void)
{
    TWCR &= ~(1 << TWSTA);      // start bit
}

void TwiStop(void)
{
    TWCR |= (1 << TWSTO);      // stop bit
}

uint8_t TwiRead(void)
{
    return TWDR;
}

void TwiWrite(uint8_t byte)
{
    TWDR = byte;
}
