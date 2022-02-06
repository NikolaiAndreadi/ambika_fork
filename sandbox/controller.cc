#include "hardware_config.h"
#include "avrlib/spi.h"
#include "avrlib/devices/oled/SSD1322.h"
#include "avrlib/devices/pca9555.h"
#include "avrlib/devices/rotary_encoder_array_mod.h"

using namespace avrlib;

typedef SpiMasterPrimitive<> spi;
SSD1322<pinOLED_CS, pinOLED_DC, pinOLED_Reset, spi> oled;
EncoderArray<pinEncoderInterrupt, 0b000> enc;


int main(void) {
	// max 9 symbols
	spi::Init();
	pinSingleLed::set_mode(DIGITAL_OUTPUT);
	pinSingleLed::High();	
  	oled.Init();
	enc.Init();
	pinSingleLed::Low();
	
	while (1) {
		if (enc.Update()) {
			oled.ClearBuffer();
			oled.DrawText(150, 40, enc.ReadEncoderValue(7));
			oled.DrawText(100, 40, enc.ReadEncoderValue(6));
			oled.DrawText(50, 40, enc.ReadEncoderValue(5));
			oled.DrawText(00, 40, enc.ReadEncoderValue(4));
			oled.DrawText(150, 20, enc.ReadEncoderValue(3));
			oled.DrawText(100, 20, enc.ReadEncoderValue(2));
			oled.DrawText(50, 20, enc.ReadEncoderValue(1));
			oled.DrawText(0, 20, enc.ReadEncoderValue(0));
		}
		oled.Update();
	}
}
