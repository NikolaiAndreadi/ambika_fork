#ifndef _OLED_DISPLAY_H
#define _OLED_DISPLAY_H

#include <stdlib.h>

#include "SSD1322.h"

namespace avrlib {

template <typename CS, typename DC, typename RESET>
class OledDisplay : private SSD1322<CS, DC, RESET> {
    static constexpr uint16_t max_x = 256;
    static constexpr uint16_t max_y = 64;
    static constexpr uint8_t row_count = 8;
    static constexpr uint8_t column_count = 32;
    static constexpr uint8_t dryrun_count = 8; // max 32
    static constexpr uint8_t char_height_up = 8;
    static constexpr uint8_t char_height_down = 2;
    
    uint8_t last_updated_row;
    uint8_t last_updated_column;
    // array idx - column idx
    // element bit - row idx
    uint8_t update_mask[32]; 
 public:
    OledDisplay() {
        last_updated_row = 0;
        last_updated_column = 0;
        memset(update_mask, 0, sizeof update_mask);
    }

    void begin() { 
        U8G2::begin(); 
        U8G2::setFont(u8g2_font_6x12_mr);
    }

    void immediateUpdate() {
        U8G2::updateDisplay();
        memset(update_mask, 0, sizeof update_mask);
    }

    // y first (down to up), then x (right to left)
    void update() {
        uint8_t counter = 0;
        while (1) {
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
                U8G2::updateDisplayArea(last_updated_column, last_updated_row, 1, 1);
                update_mask[last_updated_column] &= ~(0x01 << last_updated_row);
                return;
            }
        }
    }

    void fill() {
        U8G2::drawBox(0,0,max_x,max_y);
        memset(update_mask, 0xff, sizeof update_mask);
    }

    inline void setColor(uint8_t c) { U8G2::setColorIndex(c); }

    inline void updateMaskRegion(uint16_t x0, uint8_t y0, uint16_t x1, uint8_t y1) {
        x0/=8;
        x1/=8;
        x0 = column_count-x0; // is bigger
        x1 = column_count-x1;
        
        y0/=8;
        y1/=8;
        y0 = row_count-y0;
        y1 = row_count-y1;
        y0 = 0xff >> (8-y0);
        y1 = 0xff << y1-1;
        y0 &= y1;
        
        for (int x=x1; x<x0; x++)
            update_mask[x] |= y0;
    }

    inline void print(u8g2_uint_t x, u8g2_uint_t y, const char *s) {
        uint16_t count;
        count = U8G2::drawUTF8(x, y, s); // xy is down left point  
        //updateMaskRegion(x, y-char_height, x+count, y);
        setColor(2);
        U8G2::drawFrame(x, y-char_height_up, count, char_height_up+char_height_down);
    }
    
    inline void print(u8g2_uint_t x, u8g2_uint_t y, const uint8_t i) {
        char tmp[3];
        uint16_t count;
        itoa(i, tmp, 2);
        print(x, y, tmp);
    }


};

} // avrlib

#endif // _OLED_DISPLAY_H