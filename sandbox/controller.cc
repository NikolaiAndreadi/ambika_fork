#include <stdlib.h>
#include "avrlib/devices/bicolor_led_array.h"
#include "displaylib/SSD1322.h"
#include "hardware_config.h"
#include "avrlib/devices/pca9555.h"


SSD1322<pinOLED_CS, pinOLED_DC, pinOLED_Reset> oled;
BicolorLedArray<pinCSLedLine> bla;

int main(void) {
	uint8_t val = 0;
	char tmp[10];
	sei();
	PCA9555_init();
	PCA9555_dir(PCA9555_DEV_010, PCA9555_PORT_0, 0b11111111);
	PCA9555_dir(PCA9555_DEV_010, PCA9555_PORT_1, 0b11111111);
	PCA9555_dir(PCA9555_DEV_000, PCA9555_PORT_0, 0b11111111);
	PCA9555_dir(PCA9555_DEV_000, PCA9555_PORT_1, 0b11111111);
	pinSingleLed::set_mode(DIGITAL_OUTPUT);
	pinSingleLed::High();	
  	oled.begin();
	pinSingleLed::Low();
	oled.setFont(u8g2_font_6x12_mr);
	oled.drawStr(0,20,"Privetik");
	oled.sendBuffer();
	bla.SetPixel(0, 0b00001111);
	bla.SetPixel(1, 0b11111111);
	bla.SetPixel(2, 0b11110000);
  while (1) {
	  oled.clearBuffer();
	  val = PCA9555_get(PCA9555_DEV_010, PCA9555_PORT_0);
	  itoa(val, tmp, 2);
	  oled.drawUTF8(0, 10, tmp);
	  val = PCA9555_get(PCA9555_DEV_010, PCA9555_PORT_1);
	  itoa(val, tmp, 2);
	  oled.drawUTF8(0, 20, tmp);
	  val = PCA9555_get(PCA9555_DEV_000, PCA9555_PORT_0);
	  itoa(val, tmp, 2);
	  oled.drawUTF8(0, 30, tmp);
	  val = PCA9555_get(PCA9555_DEV_000, PCA9555_PORT_1);
	  itoa(val, tmp, 2);
	  oled.drawUTF8(0, 40, tmp);

	  bla.Update();
	  oled.updateDisplay();
  }
}
