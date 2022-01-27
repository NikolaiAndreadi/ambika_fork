#include "avrlib/devices/bicolor_led_array.h"
#include "avrlib/devices/oled/OledDisplay.h"
#include "hardware_config.h"
#include "avrlib/devices/pca9555.h"


OledDisplay<pinOLED_CS, pinOLED_DC, pinOLED_Reset> oled;
BicolorLedArray<pinCSLedLine> bla;
PCA9555_input<pinEncoderInterrupt, 0b000> enc;
PCA9555_input<pinButtonsInterrupt, 0b001> but;

int main(void) {
	pinSingleLed::set_mode(DIGITAL_OUTPUT);
	pinSingleLed::High();	
  	oled.begin();
	pinSingleLed::Low();
	//bla.SetPixel(0, 0b00001111);
	//bla.SetPixel(1, 0b11111111);
	//bla.SetPixel(2, 0b11110000);
	
	oled.setColor(1);

	while(1) {
		for (uint8_t i = 10; i<60; i++) {
			oled.print(i, i, "(Hello)");
			for (uint8_t iter=0; iter<32; iter++) {
				oled.update();
				//bla.Update();
			}
			//_delay_ms(10);
		}
		for (uint8_t i = 60; i>10; i--) {
			oled.print(120-i, i, "(Hello)");
			for (uint8_t iter=0; iter<32; iter++) {
				oled.update();
				//bla.Update();
			}
			//_delay_ms(10);
		}
		for (uint8_t i = 10; i<60; i++) {
			oled.print(120-i, i, "(Hello)");
			for (uint8_t iter=0; iter<32; iter++) {
				oled.update();
				//bla.Update();
			}
			//_delay_ms(10);
		}
		for (uint8_t i = 60; i>10; i--) {
			oled.print(i, i, "(Hello)");
			for (uint8_t iter=0; iter<32; iter++) {
				oled.update();
				//bla.Update();
			}
			//_delay_ms(10);
		}
  	}
}
