#ifndef PCA9555
#define PCA9555

#include <inttypes.h>
#include "avrlib/i2c/i2c.h"

#define PCA9555_DEVICE_ADDRESS 0x40

/**
 *  Eight possible device addresses. The rightmost (least significant)
 *  bit is A0 on the device.
 */
typedef enum {
  PCA9555_DEV_000               = 0b000,
  PCA9555_DEV_001               = 0b001,
  PCA9555_DEV_010               = 0b010,
  PCA9555_DEV_011               = 0b011,
  PCA9555_DEV_100               = 0b100,
  PCA9555_DEV_101               = 0b101,
  PCA9555_DEV_110               = 0b110,
  PCA9555_DEV_111               = 0b111,
} PCA9555_DEVICE;

/**
 *  Two ports on a PCA9555, port 0 and port 1.
 */
typedef enum {
  PCA9555_PORT_0                = 0,
  PCA9555_PORT_1                = 1,
} PCA9555_PORT;

/**
 * Registers in a PCA9555.
 */
typedef enum {
  PCA9555_INPUT_0               = 0,
  PCA9555_INPUT_1               = 1,
  PCA9555_OUTPUT_0              = 2,
  PCA9555_OUTPUT_1              = 3,
  PCA9555_POLARITY_INV_0        = 4,
  PCA9555_POLARITY_INV_1        = 5,
  PCA9555_DIRECTION_0           = 6,
  PCA9555_DIRECTION_1           = 7,
} PCA9555_REGISTER;

/**
 *  Initialize the PCA9555 driver.
 */
void PCA9555_init(void) {
  i2c_init();
}

/**
 *  Low level write command. Uses i2cmaster library to communicate with the
 *  device. Calls i2c_start_wait, which waits for ACK from the device, so if
 *  the specified device is not on the i2c bus, it will wait forever.
 *  @param dev 3 bit device address.
 *  @param reg Register to write.
 *  @param value Value to write.
 */
void PCA9555_write(PCA9555_DEVICE dev, PCA9555_REGISTER reg, uint8_t value) {
  i2c_start_wait(PCA9555_DEVICE_ADDRESS + dev + I2C_WRITE);
  i2c_write(reg);
  i2c_write(value);
  i2c_stop();
}

/**
 *  Low level read command. Uses i2cmaster library to communicate with the
 *  device. Calls i2c_start_wait, which waits for ACK from the device, so if
 *  the specified device is not on the i2c bus, it will wait forever.
 *  @param dev 3 bit device address.
 *  @param reg Register to read.
 *  @return value Value read from the device.
 */
uint8_t PCA9555_read(PCA9555_DEVICE dev, PCA9555_REGISTER reg) {
  uint8_t rval;
  i2c_start_wait(PCA9555_DEVICE_ADDRESS + dev + I2C_WRITE);
  i2c_write(reg);
  i2c_rep_start(PCA9555_DEVICE_ADDRESS + dev + I2C_READ);
  rval = i2c_readNak();
  i2c_stop();
  return rval;
}

/**
 *  Set the direction of a port. Implemented as a macro.
 *  @param dev 3 bit device address.
 *  @param port Write to port 0 or 1.
 *  @param dir 8 bit direction of the pins. 1 means input, 0 output.
 */
void PCA9555_dir(PCA9555_DEVICE dev, PCA9555_PORT port, uint8_t dir);

/**
 *  Invert the polarity of pins on a port. Take care when using this function
 *  as it inverts the current polarity settings, it is not an absolute value.
 *  If the PCA9555 has not been reset but the microcontroller has been, this
 *  might not contain the value you expect. This is complicated by the fact
 *  that this chip does not have any RESET line or i2c command that can be
 *  accessed from a microcontroller. Implemented as a macro.
 *  @param dev 3 bit device address.
 *  @param port Write to port 0 or 1.
 *  @param pol 8 bit polarity of the pins. 1 means invert, 0 no change. */
void PCA9555_pol(PCA9555_DEVICE dev, PCA9555_PORT port, uint8_t pol);

/**
 *  Set values on a port. Implemented as a macro.
 *  @param dev 3 bit device address.
 *  @param port Write to port 0 or 1.
 *  @param value New 8 bit value for the port.
 */
void PCA9555_set(PCA9555_DEVICE dev, PCA9555_PORT port, uint8_t value);

/**
 *  Get values from a port.
 *  @param dev 3 bit device address.
 *  @param port Write to port 0 or 1.
 *  @return 8 bit value on the port.
 */
uint8_t PCA9555_get(PCA9555_DEVICE dev, PCA9555_PORT port);

#define PCA9555_dir(dev,port,dir) PCA9555_write( \
  dev, \
  (port == PCA9555_PORT_0) ? PCA9555_DIRECTION_0 : PCA9555_DIRECTION_1, \
  dir \
)

#define PCA9555_pol(dev,port,pol) PCA9555_write( \
  dev, \
  (port == PCA9555_PORT_0) ? PCA9555_POLARITY_INV_0 : PCA9555_POLARITY_INV_1, \
  pol \
)

#define PCA9555_set(dev,port,value) PCA9555_write( \
  dev, \
  (port == PCA9555_PORT_0) ? PCA9555_OUTPUT_0 : PCA9555_OUTPUT_1, \
  value \
)

#define PCA9555_get(dev,port) PCA9555_read( \
  dev, \
  (port == PCA9555_PORT_0) ? PCA9555_INPUT_0 : PCA9555_INPUT_1 \
)

#endif /* [PCA9555] */

/**@}*/