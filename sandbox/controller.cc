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
	
	while(1) {
		for (uint8_t i = 10; i<60; i++) {
			oled.clearBuffer();
			oled.drawChunkBorders();
			oled.fill();
			oled.print(i, i, "(Hejjo)");
			for (uint8_t iter=0; iter<255; iter++) {
				oled.update();
			}
			_delay_ms(500);
		}
		for (uint8_t i = 60; i>10; i--) {
			oled.clearBuffer();
			oled.drawChunkBorders();
			oled.fill();
			oled.print(120-i, i, "(Hejjo)");
			for (uint8_t iter=0; iter<255; iter++) {
				oled.update();
			}
			_delay_ms(500);
		}
		for (uint8_t i = 10; i<60; i++) {
			oled.clearBuffer();
			oled.drawChunkBorders();
			oled.fill();
			oled.print(120-i, i, "(Hejjo)");
			for (uint8_t iter=0; iter<255; iter++) {
				oled.update();
			}
			_delay_ms(500);
		}
		for (uint8_t i = 60; i>10; i--) {
			oled.clearBuffer();
			oled.drawChunkBorders();
			oled.fill();
			oled.print(i, i, "(Hejjo)");
			for (uint8_t iter=0; iter<255; iter++) {
				oled.update();
			}
			_delay_ms(500);
		}
  	}
}
