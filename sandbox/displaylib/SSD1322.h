#ifndef SSD1322_HH
#define SSD1322_HH

#include "u8g2/u8g2.h"
#include "avrlib/time.h"
#include "avrlib/spi.h"

// U8G2 defines
// define U8G2_16BIT
// undef U8G2_WITH_INTERSECTION
// undef U8G2_USE_DYNAMIC_ALLOC
// undef U8G2_WITH_CLIP_WINDOW_SUPPORT
// undef U8G2_WITH_FONT_ROTATION
// undef U8G2_WITH_UNICODE


class U8G2 {
 protected: 
	u8g2_t u8g2;
	
 public:
    U8G2() = default;
	u8g2_uint_t tx, ty;
	
	u8g2_t *getU8g2(void) { return &u8g2; }
	
	void initDisplay(void) { u8g2_InitDisplay(&u8g2); }
      
  	void clearDisplay(void) { u8g2_ClearDisplay(&u8g2); }
    
  	void setPowerSave(uint8_t is_enable) { u8g2_SetPowerSave(&u8g2, is_enable); }
    
  	void setFlipMode(uint8_t mode) { u8g2_SetFlipMode(&u8g2, mode); }

  	void setContrast(uint8_t value) { u8g2_SetContrast(&u8g2, value); }
    
  	void setDisplayRotation(const u8g2_cb_t *u8g2_cb) { u8g2_SetDisplayRotation(&u8g2, u8g2_cb); }
	  
	bool begin(void) {
      initDisplay(); 
      clearDisplay(); 
      setPowerSave(0); 
      return 1;
    }
	
	u8g2_uint_t getDisplayHeight(void) { return u8g2_GetDisplayHeight(&u8g2); }
  	u8g2_uint_t getDisplayWidth(void) { return u8g2_GetDisplayWidth(&u8g2); }
	
	void sendBuffer(void) { u8g2_SendBuffer(&u8g2); }
  	void clearBuffer(void) { u8g2_ClearBuffer(&u8g2); }    
	uint8_t *getBufferPtr(void) { return u8g2_GetBufferPtr(&u8g2); }
  	uint8_t getBufferTileHeight(void) { return u8g2_GetBufferTileHeight(&u8g2); }
  	uint8_t getBufferTileWidth(void) { return u8g2_GetBufferTileWidth(&u8g2); }
	void setBufferAutoClear(uint8_t mode)  { u8g2_SetAutoPageClear(&u8g2, mode); }
		
	void updateDisplayArea(uint8_t  tx, uint8_t ty, uint8_t tw, uint8_t th) { u8g2_UpdateDisplayArea(&u8g2, tx, ty, tw, th); }
  	void updateDisplay(void) { u8g2_UpdateDisplay(&u8g2); }
	  
	/* u8g2_hvline.c */
	void setDrawColor(uint8_t color_index) { u8g2_SetDrawColor(&u8g2, color_index); }
	uint8_t getDrawColor(void) { return u8g2_GetDrawColor(&u8g2); }
	void drawPixel(u8g2_uint_t x, u8g2_uint_t y) { u8g2_DrawPixel(&u8g2, x, y); }
	void drawHLine(u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t w) { u8g2_DrawHLine(&u8g2, x, y, w); }
	void drawVLine(u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t h) { u8g2_DrawVLine(&u8g2, x, y, h); }
	void drawHVLine(u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t len, uint8_t dir) { u8g2_DrawHVLine(&u8g2, x, y, len, dir); }
    
	/* u8g2_box.c */
	void drawFrame(u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t w, u8g2_uint_t h) { u8g2_DrawFrame(&u8g2, x, y, w, h); }
	void drawRFrame(u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t w, u8g2_uint_t h, u8g2_uint_t r) { u8g2_DrawRFrame(&u8g2, x, y, w, h,r); }
	void drawBox(u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t w, u8g2_uint_t h) { u8g2_DrawBox(&u8g2, x, y, w, h); }
	void drawRBox(u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t w, u8g2_uint_t h, u8g2_uint_t r) { u8g2_DrawRBox(&u8g2, x, y, w, h,r); }
	
	/* u8g2_circle.c */
	void drawCircle(u8g2_uint_t x0, u8g2_uint_t y0, u8g2_uint_t rad, uint8_t opt = U8G2_DRAW_ALL) { u8g2_DrawCircle(&u8g2, x0, y0, rad, opt); }
	void drawDisc(u8g2_uint_t x0, u8g2_uint_t y0, u8g2_uint_t rad, uint8_t opt = U8G2_DRAW_ALL) { u8g2_DrawDisc(&u8g2, x0, y0, rad, opt); }     
	void drawEllipse(u8g2_uint_t x0, u8g2_uint_t y0, u8g2_uint_t rx, u8g2_uint_t ry, uint8_t opt = U8G2_DRAW_ALL) { u8g2_DrawEllipse(&u8g2, x0, y0, rx, ry, opt); }
	void drawFilledEllipse(u8g2_uint_t x0, u8g2_uint_t y0, u8g2_uint_t rx, u8g2_uint_t ry, uint8_t opt = U8G2_DRAW_ALL) { u8g2_DrawFilledEllipse(&u8g2, x0, y0, rx, ry, opt); }    

	/* u8g2_line.c */
	void drawLine(u8g2_uint_t x1, u8g2_uint_t y1, u8g2_uint_t x2, u8g2_uint_t y2) { u8g2_DrawLine(&u8g2, x1, y1, x2, y2); }
	  
	/* u8g2_font.c */
	void setFont(const uint8_t  *font) {u8g2_SetFont(&u8g2, font); }
	void setFontMode(uint8_t  is_transparent) {u8g2_SetFontMode(&u8g2, is_transparent); }
	void setFontDirection(uint8_t dir) {u8g2_SetFontDirection(&u8g2, dir); }

	int8_t getAscent(void) { return u8g2_GetAscent(&u8g2); }
	int8_t getDescent(void) { return u8g2_GetDescent(&u8g2); }
  
	void setFontPosBaseline(void) { u8g2_SetFontPosBaseline(&u8g2); }
	void setFontPosBottom(void) { u8g2_SetFontPosBottom(&u8g2); }
	void setFontPosTop(void) { u8g2_SetFontPosTop(&u8g2); }
	void setFontPosCenter(void) { u8g2_SetFontPosCenter(&u8g2); }

	void setFontRefHeightText(void) { u8g2_SetFontRefHeightText(&u8g2); }
	void setFontRefHeightExtendedText(void) { u8g2_SetFontRefHeightExtendedText(&u8g2); }
	void setFontRefHeightAll(void) { u8g2_SetFontRefHeightAll(&u8g2); }
	
	u8g2_uint_t drawStr(u8g2_uint_t x, u8g2_uint_t y, const char *s) { return u8g2_DrawStr(&u8g2, x, y, s); }
	u8g2_uint_t drawUTF8(u8g2_uint_t x, u8g2_uint_t y, const char *s) { return u8g2_DrawUTF8(&u8g2, x, y, s); }
	
	u8g2_uint_t getStrWidth(const char *s) { return u8g2_GetStrWidth(&u8g2, s); }
	u8g2_uint_t getUTF8Width(const char *s) { return u8g2_GetUTF8Width(&u8g2, s); }
	
	/* u8glib compatible functions */
	void sleepOn(void) { u8g2_SetPowerSave(&u8g2, 1); }
	void sleepOff(void) { u8g2_SetPowerSave(&u8g2, 0); }    
	void setColorIndex(uint8_t color_index) { u8g2_SetDrawColor(&u8g2, color_index); }
	uint8_t getColorIndex(void) { return u8g2_GetDrawColor(&u8g2); }
	int8_t getFontAscent(void) { return u8g2_GetAscent(&u8g2); }
	int8_t getFontDescent(void) { return u8g2_GetDescent(&u8g2); }
	int8_t getMaxCharHeight(void) { return u8g2_GetMaxCharHeight(&u8g2); }
	int8_t getMaxCharWidth(void) { return u8g2_GetMaxCharWidth(&u8g2); }
	u8g2_uint_t getHeight() { return u8g2_GetDisplayHeight(&u8g2); }
	u8g2_uint_t getWidth() { return u8g2_GetDisplayWidth(&u8g2); }
};

template <typename CS, typename DC, typename RESET>
class SSD1322 : public U8G2 {
 private:
	typedef avrlib::SpiMasterPrimitive<> spi_;

	static uint8_t byte_hw_spi(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr) {	
		switch(msg) {
			case U8X8_MSG_BYTE_SEND:
				uint8_t *data;	
				data = (uint8_t *)arg_ptr;
				while( arg_int > 0 ) {
					spi_::Send((uint8_t)*data);
					data++;
					arg_int--;
				}
			break;
			
			case U8X8_MSG_BYTE_INIT:
				CS::High();
			break;
			
			case U8X8_MSG_BYTE_SET_DC:
				DC::set_value(arg_int);
			break;
			
			case U8X8_MSG_BYTE_START_TRANSFER:
				CS::Low();
			break;
			
			case U8X8_MSG_BYTE_END_TRANSFER:
				CS::High();
			break;
			
			default:
				return 0;
		}
		return 1;
	}
	
	static uint8_t gpio_and_delay(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr) {
		uint8_t i;
		switch(msg)
		{
			case U8X8_MSG_DELAY_MILLI:
			while (0 < arg_int) {
				_delay_ms(1);
				--arg_int;
			}			
			
			break;
			
			case U8X8_MSG_GPIO_RESET:
				RESET::set_value(arg_int);
			break;
			
			default:
				return 0;
		}
		return 1;
	}

 public:
	SSD1322() {
    	CS::set_mode(avrlib::DIGITAL_OUTPUT);
    	CS::High();
		spi_::Init();
		DC::set_mode(avrlib::DIGITAL_OUTPUT);
		RESET::set_mode(avrlib::DIGITAL_OUTPUT);
		u8g2_Setup_ssd1322_nhd_256x64_f(&u8g2, U8G2_R2, byte_hw_spi, gpio_and_delay);
	}
};

#endif // SSD1322_HH