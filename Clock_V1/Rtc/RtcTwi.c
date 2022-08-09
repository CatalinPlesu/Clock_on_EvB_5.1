#include "RtcTwi.h"

ISR(TWI_vect){

    // clears twint flag after all operations have been done
    TWCR |= (1 << TWINT);
}

void RtcTwiInit(void){
    TWBR = 250;

    TWCR |= (1 << TWINT); // clears int flag
    TWCR |= (1 << TWEN); // enable twi 
    TWCR |= (1 << TWIE); // enable twi interrupt
    /* TWCR |= (1 << TWSTA); // start bit */
    /* TWCR |= (1 << TWSTO); // stop bit */

    TWSR |= (1 << TWPS1); //prescaler value 16
}

uint8_t RtcTwiRead(void){
    return TWDR;
}

void RtcTwiWrite(uint8_t byte){
    TWDR = byte;
}
