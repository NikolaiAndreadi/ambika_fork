#ifndef _I2C_H
#define _I2C_H

#include <avrlib/base.h>
#include <compat/twi.h>

#define I2C_WRITE 0
#define I2C_READ 1

namespace avrlib {

template <uint32_t frequency>
class I2cMaster {
  public:
    static bool Begin(uint8_t address) {
        // returns true if success
        uint8_t   twst;

	    TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN); // send START condition
	    while(!(TWCR & (1<<TWINT))); // wait until transmission completed

	    twst = TW_STATUS & 0xF8; // check Status Register, mask prescaler bits.
	    if ( (twst != TW_START) && (twst != TW_REP_START)) return false;

	    TWDR = address; // send device address
	    TWCR = (1<<TWINT) | (1<<TWEN);
	    while(!(TWCR & (1<<TWINT)));

	    twst = TW_STATUS & 0xF8; // check Status Register
	    if ( (twst != TW_MT_SLA_ACK) && (twst != TW_MR_SLA_ACK) ) return false;
	    return true;
    }

    static void End() { 
        TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTO); 
        while(TWCR & (1<<TWSTO));
    }

    I2cMaster() {
        TWSR = 0; // no prescaler
        TWBR = ((F_CPU/frequency)-16)/2;
    }

    static void Write(uint8_t data) {
	    TWDR = data; 
	    TWCR = (1<<TWINT) | (1<<TWEN);
	    while(!(TWCR & (1<<TWINT))); 
    }

    static uint8_t Read(bool continueRead = false) {
        if (continueRead)
            TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWEA);
        else
            TWCR = (1<<TWINT) | (1<<TWEN);
        
	    while(!(TWCR & (1<<TWINT)));    
        return TWDR;
    }

    DISALLOW_COPY_AND_ASSIGN(I2cMaster);
};

extern I2cMaster<400000> i2c;

}  // namespace avrlib

#endif // _I2C_H
