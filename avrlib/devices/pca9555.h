#ifndef _PCA9555_H
#define _PCA9555_H

#include <avrlib/i2c/i2c.h>
#include <avrlib/base.h>

namespace avrlib {

template <typename InterruptPin, uint8_t user_addr>
class PCA9555_input {
  private:
    Word data;
    static const uint8_t hw_addr = 0x40;
    static constexpr uint8_t addr = hw_addr + (user_addr << 1);
  public:
    PCA9555_input() { 
      InterruptPin::set_mode(DIGITAL_INPUT); 
    }

    void Init() {
      // invert reg values (using pullups)
      i2c.Begin(addr + I2C_WRITE);
      i2c.Write(4);
      i2c.Write(0xff);
      i2c.Write(0xff);
      i2c.End();
    }

    void Update () {
      if (InterruptPin::is_high()) 
        return;
      i2c.Begin(addr + I2C_WRITE);
      i2c.Write(0); // read from zeroth register
      i2c.Begin(addr +  I2C_READ);
      data.bytes[0] = i2c.Read(true); // read zeroth reg
      data.bytes[1] = i2c.Read(); // read first register
      i2c.End();
    }

    Word GetData() { return data; }

    DISALLOW_COPY_AND_ASSIGN(PCA9555_input);
};

} // namespace avrlib

#endif // _PCA9555_H