#include<Arduino.h>
#include"RGBW.h"


uint8_t head[8]={0x01,0x01,0x01,0x01,0xFE,0xFE,0xFE,0xFE};//must send this head data before sending RGBW data
                        
//shift_temp1[30][4] is used to save RGBW data of 30 LEDs with shift mode1
/*uint8_t shift_temp1_mid[64][4]={{0x00,0xFF,0x00,0x00},{0x00,0xFF,0x00,0x00},{0x00,0xFF,0x00,0x00},{0x00,0xFF,0x00,0x00},
                               {0x00,0xFF,0x00,0x00},{0x00,0xFF,0x00,0x00},{0x00,0xFF,0x00,0x00},{0x00,0xFF,0x00,0x00},
                               {0x00,0x00,0xFF,0x00},{0x00,0x00,0xFF,0x00},{0x00,0x00,0xFF,0x00},{0x00,0x00,0xFF,0x00},
                               {0x00,0x00,0xFF,0x00},{0x00,0x00,0xFF,0x00},{0x00,0x00,0xFF,0x00},{0x00,0x00,0xFF,0x00},
                               {0x00,0x00,0x00,0xFF},{0x00,0x00,0x00,0xFF},{0x00,0x00,0x00,0xFF},{0x00,0x00,0x00,0xFF},
                               {0x00,0x00,0x00,0xFF},{0x00,0x00,0x00,0xFF},{0x00,0x00,0x00,0xFF},{0x00,0x00,0x00,0xFF},
                               {0x00,0xFF,0xFF,0x00},{0x00,0xFF,0xFF,0x00},{0x00,0xFF,0xFF,0x00},{0x00,0xFF,0xFF,0x00},
                               {0x00,0xFF,0xFF,0x00},{0x00,0xFF,0xFF,0x00},{0x00,0xFF,0xFF,0x00},{0x00,0xFF,0xFF,0x00},
                               {0x00,0xFF,0x00,0xFF},{0x00,0xFF,0x00,0xFF},{0x00,0xFF,0x00,0xFF},{0x00,0xFF,0x00,0xFF},
                               {0x00,0xFF,0x00,0xFF},{0x00,0xFF,0x00,0xFF},{0x00,0xFF,0x00,0xFF},{0x00,0xFF,0x00,0xFF},
                               {0x00,0x00,0xFF,0xFF},{0x00,0x00,0xFF,0xFF},{0x00,0x00,0xFF,0xFF},{0x00,0x00,0xFF,0xFF},
                               {0x00,0x00,0xFF,0xFF},{0x00,0x00,0xFF,0xFF},{0x00,0x00,0xFF,0xFF},{0x00,0x00,0xFF,0xFF},
                               {0xFF,0x00,0x00,0x00},{0xFF,0x00,0x00,0x00},{0xFF,0x00,0x00,0x00},{0xFF,0x00,0x00,0x00},
                               {0xFF,0x00,0x00,0x00},{0xFF,0x00,0x00,0x00},{0xFF,0x00,0x00,0x00},{0xFF,0x00,0x00,0x00},
                               {0x00,0x00,0x00,0x00},{0x00,0x00,0x00,0x00},{0x00,0x00,0x00,0x00},{0x00,0x00,0x00,0x00},
                               {0x00,0x00,0x00,0x00},{0x00,0x00,0x00,0x00},{0x00,0x00,0x00,0x00},{0x00,0x00,0x00,0x00}};*/
                               
                               
           
//send head data function
//must use this function before sending RGBW data
void send_head()
{
      uint8_t mask;
      uint8_t *p = head;
      uint8_t *end = p + 8;
      while(p<end)
      {
        uint8_t data = *p++;
        for(mask=0x80; mask; mask>>=1)
        {
            if(mask&data)
            {
                PORTB &= ~0X80;
                PORTB &= ~0X80;
                PORTB &= ~0X80;
                PORTB &= ~0X80;
                PORTB &= ~0X80;
                PORTB &= ~0X80;
                //PORTC &= ~0X80;
                PORTB |= 0x80;
            }
            else
            {
                PORTB &= ~0X80;
                PORTB &= ~0X80;
                PORTB &= ~0X80;
                //_NOP();
                //PORTC &= ~0X80;
      
                PORTB |= 0x80;
                
            }
        }
      }
}


//LED with IC send RGBW data function
//*RGBW is a pointer point to an array with 4 uint8 data
void send_RGBW_IC(uint8_t *RGBW)
{
      uint8_t mask;
      uint8_t *p = RGBW;
      uint8_t *end = p + 4;
      while(p<end)
      {
        uint8_t data = *p++;
        for(mask=0x80; mask; mask>>=1)
        {
            if(mask&data)
            {
                PORTB &= ~0X80;
                PORTB &= ~0X80;
                PORTB &= ~0X80;
                PORTB &= ~0X80;
                PORTB &= ~0X80;
                PORTB &= ~0X80;
                //PORTC &= ~0X80;
                PORTB |= 0x80;
            }
            else
            {
                PORTB &= ~0X80;
                PORTB &= ~0X80;
                PORTB &= ~0X80;
                //PORTC &= ~0X80; 0xFF
                //_NOP();     
                PORTB |= 0x80;
                
            }
        }
      }
}

//LED with Ic show by panel mode function
//r,g,b,w is RGBW data of 30 LEDs   
void RGBW_Ic_Panel_Show(uint16_t r, uint16_t g, uint16_t b, uint16_t w,uint16_t num)
{
    uint8_t rgbw[4]={w,r,g,b};
    send_head();
    for(uint16_t i=0;i<num;i++)
    {
      send_RGBW_IC(rgbw);
    }
    delayMicroseconds(250);
}

void RGBW_Ic_Shift_Show(uint8_t p[256][4])
{
    
    send_head();
    for(uint16_t i=0;i<256;i++)
      {

        send_RGBW_IC(p[i]);
      } 
    delayMicroseconds(250);
}
