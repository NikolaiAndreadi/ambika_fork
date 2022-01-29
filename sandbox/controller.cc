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
	oled.DrawHLine(15, 15, 50);
	oled.UpdateAll();
}
