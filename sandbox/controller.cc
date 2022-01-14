#include "avrlib/devices/bicolor_led_array.h"
#include "displaylib/SSD1322.h"
#include "hardware_config.h"


SSD1322<pinOLED_CS, pinOLED_DC, pinOLED_Reset> oled;
BicolorLedArray<pinCSLedLine> bla;

int main(void) {
	pinSingleLed::set_mode(DIGITAL_OUTPUT);
	pinSingleLed::High();	
  	oled.begin();
	pinSingleLed::Low();
	oled.setFont(u8g2_font_ncenB14_tr);
	oled.drawStr(0,20,"Privetik");
	oled.sendBuffer();
	bla.set_direct_pixel(0, 0b00001111);
	bla.set_direct_pixel(1, 0b11111111);
	bla.set_direct_pixel(2, 0b11110000);
  while (1) {
	  bla.ShiftOutPixels();
  }
}
