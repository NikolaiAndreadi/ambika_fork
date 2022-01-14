#ifndef HARDWARE_CONFIG_H_
#define HARDWARE_CONFIG_H_

#include "avrlib/gpio.h"

using namespace avrlib;

using pinCSLedLine = Gpio<PortC, 5>;
using pinSingleLed = Gpio<PortC, 4>;

using pinOLED_Reset = Gpio<PortC, 6>;
using pinOLED_DC = Gpio<PortC, 7>;
using pinOLED_CS = Gpio<PortA, 7>;

using pinCScard = Gpio<PortA, 6>;

using pinVoice1 = Gpio<PortA, 0>;
using pinVoice2 = Gpio<PortA, 1>;
using pinVoice3 = Gpio<PortA, 2>;
using pinVoice4 = Gpio<PortA, 3>;
using pinVoice5 = Gpio<PortA, 4>;
using pinVoice6 = Gpio<PortA, 5>;

using pinSpecial = Gpio<PortB, 4>;

using pinEncoderInterrupt = Gpio<PortD, 2>;
using pinButtonsInterrupt = Gpio<PortD, 3>;

// SPI
using IOClockLine = Gpio<PortB, 7>;
using IOInputLine = Gpio<PortB, 6>;
using IOOutputLine = Gpio<PortB, 5>;


#endif /* HARDWARE_CONFIG_H_ */