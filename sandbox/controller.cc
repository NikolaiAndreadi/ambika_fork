#include "avrlib/devices/oled/SSD1322.h"
#include "hardware_config.h"
#include "avrlib/spi.h"

typedef SpiMasterPrimitive<> spi;
SSD1322<pinOLED_CS, pinOLED_DC, pinOLED_Reset, spi> oled;

int main(void) {
	spi::Init();
	pinSingleLed::set_mode(DIGITAL_OUTPUT);
	pinSingleLed::High();	
  	oled.Init();
	pinSingleLed::Low();

	while(1) {
		for (uint8_t i = 10; i<60; i++) {
			oled.DrawText(i, i, "(");
			for (uint8_t iter=0; iter<32; iter++) {
				oled.Update();
			}
		}
  	}
}
