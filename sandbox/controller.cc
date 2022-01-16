#include <stdlib.h>
#include "avrlib/devices/bicolor_led_array.h"
#include "avrlib/devices/oled/SSD1322.h"
#include "hardware_config.h"
#include "avrlib/devices/pca9555.h"


SSD1322<pinOLED_CS, pinOLED_DC, pinOLED_Reset> oled;
BicolorLedArray<pinCSLedLine> bla;
PCA9555_input<pinEncoderInterrupt, 0b000> enc;
PCA9555_input<pinButtonsInterrupt, 0b001> but;

int main(void) {
	char tmp[10];
	pinSingleLed::set_mode(DIGITAL_OUTPUT);
	pinSingleLed::High();	

	enc.Init();
	but.Init();
  	oled.begin();
	pinSingleLed::Low();
	oled.setFont(u8g2_font_6x12_mr);
	oled.drawStr(0,20,"Privetik");
	oled.sendBuffer();

	bla.SetPixel(0, 0b00000001);
	bla.SetPixel(1, 0b00010001);
	bla.SetPixel(2, 0b00010000);

  	while (1) {
		oled.clearBuffer();
		enc.Update();
		but.Update();

		itoa(enc.GetData().bytes[0], tmp, 2);
		oled.drawUTF8(0, 10, tmp);
		itoa(enc.GetData().bytes[1], tmp, 2);
		oled.drawUTF8(0, 20, tmp);

		itoa(but.GetData().bytes[0], tmp, 2);
		oled.drawUTF8(0, 30, tmp);
		itoa(but.GetData().bytes[1], tmp, 2);
		oled.drawUTF8(0, 40, tmp);

		bla.Update();
		oled.updateDisplay();
  }
}
