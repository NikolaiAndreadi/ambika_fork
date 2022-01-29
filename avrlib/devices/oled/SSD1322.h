#ifndef SSD1322_HH
#define SSD1322_HH

// Based on u8g2 cpp interface (trimmed down version)
// https://github.com/olikraus/u8g2
// partial display functionality by github.com/NikolaiAndreadi

#include <stdlib.h>
#include <string.h>

#include "avrlib/devices/oled/u8g2/u8g2.h"
#include "avrlib/time.h"
#include "avrlib/spi.h"

// U8G2 defines
// define U8G2_16BIT
// undef U8G2_WITH_INTERSECTION
// undef U8G2_USE_DYNAMIC_ALLOC
// undef U8G2_WITH_CLIP_WINDOW_SUPPORT
// undef U8G2_WITH_FONT_ROTATION
// undef U8G2_WITH_UNICODE

namespace avrlib {

// abstraction over c lib u8g2
// includes necessary drawing operations
// implements UpdateBuffer - helper for 'non-blocking' drawings
template <typename CS, typename DC, typename RESET, typename SPIPRIMITIVE>
class SSD1322  {
 private:
    // oled dimensions
    static constexpr uint16_t max_x = 256;
    static constexpr uint16_t max_y = 64;
    // screen is broke down to 8x8 chunks, total 32x8
    static constexpr uint8_t row_count = 8;
    static constexpr uint8_t column_count = 32;
    // max cycles in Update routine until 'break'
    static constexpr uint8_t dryrun_count = 8; 
    // font adjustments
    static constexpr uint8_t char_height_up = 1;
    static constexpr uint8_t char_height_down = 3;
    // Update() global vars
    uint8_t last_updated_row;
    uint8_t last_updated_column;
    // Mask for chunk updating
    // array idx - column idx
    // element bit - row idx
    uint8_t update_mask[32]; 

    // u8g2 hardware spi interface
    u8g2_t u8g2;
    static uint8_t byte_hw_spi(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr) {  
        switch(msg) {
            case U8X8_MSG_BYTE_SEND:
                uint8_t *data;  
                data = (uint8_t *)arg_ptr;
                while( arg_int > 0 ) {
                    SPIPRIMITIVE::Send((uint8_t)*data);
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
        switch(msg) {
            case U8X8_MSG_DELAY_MILLI:
                while (0 < arg_int) { _delay_ms(1); --arg_int; }      
            break;

            case U8X8_MSG_GPIO_RESET:
                RESET::set_value(arg_int);
            break;

            default:
                return 0;
        }
        return 1;
    }

    void updateMaskRegions(uint16_t x0, uint8_t y0, uint16_t x1, uint8_t y1) {
        x0>>=3;
        x1=(x1+7)>>3; // ceiling (div by 8)
        x0 = column_count-x0; // now is bigger than x1
        x1 = column_count-x1;
        
        y0>>=3;
        y1=(y1+7)>>3;
        y0 = row_count-y0;
        y1 = row_count-y1;
        
        y0 = 0xff >> (7-y0);
        y1 = 0xff << y1;
        y0 &= y1;
        
        for (int x=x1; x<x0; x++)
            update_mask[x] |= y0;
    }
 public:
    SSD1322() {
        CS::set_mode(avrlib::DIGITAL_OUTPUT);
        CS::High();
        DC::set_mode(avrlib::DIGITAL_OUTPUT);
        RESET::set_mode(avrlib::DIGITAL_OUTPUT);
        u8g2_Setup_ssd1322_nhd_256x64_f(&u8g2, U8G2_R2, byte_hw_spi, gpio_and_delay);
        last_updated_row = 0;
        last_updated_column = 0;
        memset(update_mask, 0, sizeof update_mask);
    }

    void Init() {
        u8g2_InitDisplay(&u8g2);
        u8g2_ClearDisplay(&u8g2);
        u8g2_SetFont(&u8g2, u8g2_font_6x12_mr);
        u8g2_SetPowerSave(&u8g2, 0);
    }

    void PowerOn() { u8g2_SetPowerSave(&u8g2, 0); }
    void PowerOff() { u8g2_SetPowerSave(&u8g2, 1); }
    void SetContrast(uint8_t value) { u8g2_SetContrast(&u8g2, value); }

    void Update() {
        uint8_t counter = 0;
        while (1) {
            // y first (down to up), then x (right to left)
            if (++last_updated_row == row_count) {
                last_updated_row = 0;
                if (++last_updated_column == column_count)
                    last_updated_column = 0;
            }
            if (update_mask[last_updated_column] == 0) {
                if (++counter == dryrun_count) 
                    return;
                continue;
            }

            if (update_mask[last_updated_column] & (0x01 << last_updated_row) ) {
                u8g2_UpdateDisplayArea(&u8g2, last_updated_column, last_updated_row, 1, 1);
                update_mask[last_updated_column] &= ~(0x01 << last_updated_row);
                return;
            }
        }
    }
    void UpdateAll() {
        u8g2_UpdateDisplay(&u8g2);
        memset(update_mask, 0, sizeof update_mask);
    }

    void SetColor(uint8_t color) { u8g2_SetDrawColor(&u8g2, color); }
    void SetFontMode(uint8_t mode) { u8g2_SetFontMode(&u8g2, mode); }

    void DrawUpdateMaskRegions() {
        for (uint8_t v=0; v<column_count; v++)
            DrawVLine(v*8, 0, max_y);
        for (uint8_t h=0; h<row_count; h++)
            DrawHLine(0, h*8, max_x);
        UpdateAll();
    }
    void DrawHLine(u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t w) {
        u8g2_DrawHLine(&u8g2, x, y, w);
        // mask update sequence:
        u8g2_uint_t x1 = x + w;
        x>>=3;
        x1=(x1+7)>>3;
        x = column_count-x;
        x1 = column_count-x1;
        
        y>>=3;
        y = row_count-y;     
        y = 0x01 << y;
        
        for (int i=x; x<x1; i++)
            update_mask[i] |= y;
    }
    void DrawVLine(u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t h) {
        u8g2_DrawVLine(&u8g2, x, y, h);
        // mask update sequence:
        x>>=3;
        x = column_count-x;

        u8g2_uint_t y1 = y + h;
        y>>=3;
        y1=(y1+7)>>3;
        y = row_count-y;
        y1 = row_count-y1;

        y = 0xff >> (7-y);
        y1 = 0xff << y1;
        y &= y1;

        update_mask[x] |= y;

    }
    void DrawRect(u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t w, u8g2_uint_t h) {
        u8g2_DrawFrame(&u8g2, x, y, w, h);
        updateMaskRegions(x, y, x+w, y+h);
    }
    void DrawFilledRect(u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t w, u8g2_uint_t h) {
        u8g2_DrawBox(&u8g2, x, y, w, h);
        updateMaskRegions(x, y, x+w, y+h);
    }
    uint16_t DrawText(u8g2_uint_t x, u8g2_uint_t y, const char *s) {
        uint16_t count;
        count = u8g2_DrawStr(&u8g2, x, y, s); // xy is down left point  
        updateMaskRegions(x, y-char_height_up, x+count, y+char_height_down);
        return count;
    }
    uint16_t DrawText(u8g2_uint_t x, u8g2_uint_t y, const uint8_t *i) {
        char tmp[3];
        uint16_t count;
        itoa(i, tmp, 2);
        return DrawText(x, y, tmp);
    }
    
    void FillAllDisplay() {
        u8g2_DrawBox(&u8g2, 0, 0, max_x, max_y);
        memset(update_mask, 0xff, sizeof update_mask);
    }
    void ClearBuffer() { u8g2_ClearBuffer(&u8g2); }
};
    
} // namespace avrlib

#endif // SSD1322_HH