#ifndef _OLED_DISPLAY_H
#define _OLED_DISPLAY_H

#include <stdlib.h>

#include "SSD1322.h"

namespace avrlib {

template <typename CS, typename DC, typename RESET>
class OledDisplay : private SSD1322<CS, DC, RESET> {
    static const uint16_t max_x = 256;
    static const uint16_t max_y = 64;

    static constexpr uint8_t row_count = 8;
    static constexpr uint8_t column_count = 32;

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
                if (++counter == 32) 
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

    void updateAll() {
        U8G2::updateDisplayArea(0, 0, 32, 8);
        memset(update_mask, 0, sizeof update_mask);
    }

    void drawPixel() {
        
    }

    void print(u8g2_uint_t x, u8g2_uint_t y, const char *s) {
        uint16_t count;
        count = U8G2::drawUTF8(x, y, s);
        // TODO: update updatemask
    }
    
    void print(u8g2_uint_t x, u8g2_uint_t y, const uint8_t i) {
        char tmp[3];
        uint16_t count;
        itoa(i, tmp, 10);
        count = U8G2::drawUTF8(x, y, tmp);
        // TODO: update updatemask
    }


};

} // avrlib

#endif // _OLED_DISPLAY_H