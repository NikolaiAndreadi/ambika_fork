// Copyright 2011 Emilie Gillet.
//
// Author: Emilie Gillet (emilie.o.gillet@gmail.com)
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
// -----------------------------------------------------------------------------
//
// Driver for an array of bicolor LEDs behind N cascaded shift registers.
// The pin1 of the N * 8 - 1 LEDs are connected to the first shift register
// outputs. The pin2 of all LEDs are connected to the last output of the last
// shift register in the cascade.
// 
//  +--------+
//  | 595    |
//  |     Q0 |-----[220R]----->|---+
//  |        |                     |
//  |     Q1 |-----[220R]----->|---+
//  |        |                     |
//  |     Q2 |-----[220R]----->|---+
//  |        |                     |
//  |     Q3 |-----[220R]----->|---+
//  |        |                     |
//  |     Q4 |-----[220R]----->|---+
//  |        |                     |
//  |     Q5 |-----[220R]----->|---+
//  |        |                     |
//  |     Q6 |-----[220R]----->|---+
//  |        |                     |
//  |     Q7 |---------------------+
//  |        |
//  +--------+
//
// When Q7, is low, Q0 to Q6 are controlling the first color.
// When Q7, is high, ~Q0 to ~Q6 are controlling the second color.
//
// By toggling Q7 rapidly, the two colors can be displayed simultaneously.

#ifndef AVRLIB_DEVICES_BICOLOR_LED_ARRAY_H_
#define AVRLIB_DEVICES_BICOLOR_LED_ARRAY_H_

#include <string.h>

#include "avrlib/spi.h"
#include "avrlib/op.h"

namespace avrlib {

template<typename CS>
class BicolorLedArray {
 public:
  static const uint8_t size = 7;
   
  BicolorLedArray() { 
    CS::set_mode(avrlib::DIGITAL_OUTPUT);
    CS::High();
		spi_::Init();
    Clear();
  }
  
  // Intensity is in AAAABBBB format, where AAAA is the intensity for the
  // color 1, and BBBB is the intensity for the color 2.
  static inline void SetPixel(uint8_t index, uint8_t intensity) {
    pixels[index] = intensity;
  }
  
  static inline void Clear() {
    memset(pixels, 0, size);
  }
  
  static inline void Update() {
    uint8_t package;
    uint8_t brightness;
    uint8_t threshold = refresh_cycle & 0x0f;
    // if half of counter is passed (0x20) - change the current direction  
    bool common_wire_high = refresh_cycle & 0x20 ? true : false;
      
    // we go from the highest pixel to the lowest one
    // 'cause we use 'shift left' to fill up the package byte
    for (uint8_t i=size; i!=0xff; i--, package <<= 1) {
      // extract led color from each byte
      if (common_wire_high)
        // if the common wire is high, we should invert the color
        // otherwise no current will flow when color is high too
        brightness = U8ShiftRight4(~pixels[i]); 
      else
        // cutoff the high half of byte
        brightness = pixels[i] & 0x0f; 

      // fill the bit of the package if the led should glow now,
      // or if the intensity is max (0xf)
      if (brightness > threshold || brightness == 0xf)
        package |= 1;      
    }

    // set up the direction
    if (common_wire_high)
        package|=1;
      
    // send the package to the shift register
    Begin();
    spi_::Send(package);
    End();

    // update the cycle counter
      ++refresh_cycle;
  }
  
 private:

  typedef avrlib::SpiMasterPrimitive<> spi_;

  static uint8_t pixels[size];
  static uint8_t refresh_cycle;

  static inline void Begin() {
    CS::Low();
  }
  
  static inline void End() {
    CS::High();
  }

  DISALLOW_COPY_AND_ASSIGN(BicolorLedArray);
};

template<typename CS>
uint8_t BicolorLedArray<CS>::pixels[size];

template<typename CS>
uint8_t BicolorLedArray<CS>::refresh_cycle;

}  // namespace avrlib

#endif   // AVRLIB_DEVICES_BICOLOR_LED_ARRAY_H_