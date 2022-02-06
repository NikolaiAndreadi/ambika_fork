#ifndef ROTARY_ENCODER_ARRAY_H_
#define ROTARY_ENCODER_ARRAY_H_

#include "pca9555.h"

namespace avrlib {

template <typename InterruptPin, uint8_t user_addr>
class EncoderArray {
    PCA9555_input<InterruptPin, user_addr> encodermux;
    static constexpr uint8_t encoder_count = 8;
    uint8_t encoderIncrements[encoder_count];
    /* no need for debouncing - 3 stages to make an encoderIncrement
    previous value - P_VAL
    increment++ if fresh value = 00,
    current value (last confirmed) = 01
    previous value  = 11
    if fresh = 00, current = 11, or similar bouncy behaviour - reset
    */
    enum {C_VAL, P_VAL}; // Current, Previous values
    // encoder states (from pins A and B)
    enum {ST_ZERO=0b00, ST_RIGHT=0b01, ST_LEFT=0b10, ST_CENTER=0b11};
    uint16_t encoderPositionBuffer[3];
    

    // 2nd encoder
    //    __    
    // 00 11 00 11 10 01 00 10
    // shift right 2*(7-n), numeration starts from zero
    inline uint8_t getEncoderData(uint16_t source, uint8_t encoder_num) {
        return (source >> (2*(encoder_count-1-encoder_num))) & 0b11;
    }

    inline void setEncoderData(uint16_t &destination, uint8_t encoder_num, uint8_t value) {
        uint8_t bit = value & 0b01;
        uint8_t offset = 2*(encoder_count-1-encoder_num);
        uint8_t data = getEncoderData(destination, encoder_num);
        if ( bit != data & 0b01 ) {
            if (bit == 0b00)
                destination &= ~(0b01 << offset);
            else   
                destination |= 0b01 << offset;
        }
        bit = value>>1;
        offset++;
        if ( bit != data>>1 ) {
            if (bit == 0b00)
                destination &= ~(0b01 << offset);
            else   
                destination |= 0b01 << offset;    
        }
    }

 public:
    void Init() {
        encodermux.Init();
        memset(encoderPositionBuffer, 0x00, sizeof(encoderPositionBuffer));
        memset(encoderIncrements, 0x00, sizeof(encoderIncrements));
    }

    bool Update() {
        if (!encodermux.Update())
            return false;
        
        uint16_t encmux_data = encodermux.GetData();
        if (encmux_data == encoderPositionBuffer[C_VAL])
            return false;
        
        for (uint8_t i=0; i<encoder_count; i++) {
            uint8_t fresh_data = getEncoderData(encmux_data, i);
            uint8_t cur_data = getEncoderData(encoderPositionBuffer[C_VAL], i);
            if ( fresh_data != cur_data ) {   
                uint8_t prev_data = getEncoderData(encoderPositionBuffer[P_VAL], i);

                if ((fresh_data == ST_ZERO) && (prev_data == ST_CENTER)) {
                    // use of ternary operator caused by specific hardware routing
                    if (cur_data == ST_RIGHT)
                        i>3 ? encoderIncrements[i]++ : encoderIncrements[i]-- ;
                    if (cur_data == ST_LEFT)
                        i>3 ? encoderIncrements[i]-- : encoderIncrements[i]++;   
                    setEncoderData(encoderPositionBuffer[P_VAL], i, ST_ZERO);
                    setEncoderData(encoderPositionBuffer[C_VAL], i, ST_ZERO);
                    continue;
                }

                setEncoderData(encoderPositionBuffer[P_VAL], i, cur_data);
                setEncoderData(encoderPositionBuffer[C_VAL], i, fresh_data);
            }
        }
        return true;
    }

    // hardware to software numeration
    // 4 5 6 7
    // 3 2 1 0
    // vvvvvvv
    // 0 1 2 3
    // 4 5 6 7
    uint8_t ReadEncoderValue(uint8_t encoder_num) {
        if (encoder_num < 4)
            encoder_num = 4 + encoder_num;
        else 
            encoder_num = encoder_count-1-encoder_num;
        uint8_t value = encoderIncrements[encoder_num];
        //encoderIncrements[encoder_num] = 0;
        return value;
    }

};

} // namespace avrlib

#endif //ROTARY_ENCODER_ARRAY_H_