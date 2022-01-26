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
	//oled.fill();
  	oled.print(0, 10, "!#$%&'()*+,-./0123456789:;<=>?");
	oled.print(0, 25, "ABCDEFGHIJKLMNOPQRSTUVWXYZ");
	oled.print(0, 40, "abcdefghijklmnopqrstuvwxyz");
	oled.immediateUpdate();
	while (1) {
		//oled.update();
		pinSingleLed::Toggle();	
		//_delay_ms(50);
  	}
}
