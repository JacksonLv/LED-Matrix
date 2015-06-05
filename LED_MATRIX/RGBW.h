#ifndef _RGBW_H
#define _RGBW_H

#include<Arduino.h>

void send_RGBW_IC(uint8_t *RGBW);
void RGBW_Ic_Panel_Show(uint16_t r, uint16_t g, uint16_t b, uint16_t w,uint16_t num);
void RGBW_Ic_Shift_Show(uint8_t p[256][4]);





#endif
