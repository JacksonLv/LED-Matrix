#include <RBL_nRF8001.h>
#include <RBL_services.h>

#include "Adafruit_NeoPixel.h"
#include <SPI.h>
#include <EEPROM.h>
#include <boards.h>
#include <RBL_nRF8001.h>
#include "RGBW.h"
#include "TimerOne.h"



uint8_t RGBW_rec[256][4]={0};
uint8_t RGBW_temp[256][4]={0x14,0,0};
uint8_t p[256][4]={0x14,0};
uint8_t pq[256][4]={{0,0,0,0},0x14};
uint8_t pa[256][4]={{0,0,0,0},{0,0,0,0},0x14,0};
uint8_t ps[256][4]={0};
uint8_t pw[256][4]={0};
uint8_t pc[256][4]={0};
uint8_t pv[256][4]={0};
uint8_t pb[256][4]={0};
uint8_t pf[256][4]={0};
uint8_t pg[256][4]={0};
uint8_t ph[256][4]={0};
uint8_t pj[256][4]={0};
uint8_t pk[256][4]={0};


static uint8_t pic_count =0;
static uint8_t time_250ms_count = 0;

uint8_t error_temp = 0;
uint8_t XYRGBW_rec[6] = {0};
static uint8_t receive_count = 0;
static uint8_t rec_start_flag = 0 ;
static uint8_t data_start_end = 0;
static uint8_t rec_end_flag = 0 ;
static uint8_t rec_RGBW_fin = 0;

// Parameter 1 = number of pixels in strip
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_RGB     Pixels are wired for RGB bitstream
//   NEO_GRB     Pixels are wired for GRB bitstream
//   NEO_KHZ400  400 KHz bitstream (e.g. FLORA pixels)
//   NEO_KHZ800  800 KHz bitstream (e.g. High Density LED strip)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(1, 7, NEO_GRB + NEO_KHZ800);



void setup() 
{
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  ble_set_name("BLE MATRIX");
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
  //i2c_init();
  Serial.begin(9600);
  ble_begin();
  Timer1.initialize(25000);         // initialize timer1, and set a 1/4 second period
  Timer1.attachInterrupt(colorWipe);  // attaches callback() as a timer overflow interrupt
  /*while(1)
  {
      
      //RGBW_Ic_Shift_Show(p);
      RGBW_Ic_Panel_Show(0x04,0,0,0,256);
      delay(100);
      //RGBW_Ic_Shift_Show(p);
      RGBW_Ic_Panel_Show(0,0x04,0,0,256);
      delay(100);
  }*/
}

void loop() 
{
  uint8_t i,h;
  if(1 == ble_available())
  {
      data_start_end = ble_read();
      Serial.println(data_start_end,HEX);
      if(data_start_end == 0xaa)
      {
          rec_start_flag = 1;
      }
      else if(data_start_end == 0xbb)
      {
         if(rec_RGBW_fin==1)
         {
            rec_RGBW_fin = 0;
            rec_start_flag = 0;
            rec_end_flag = 1;
            memcpy(RGBW_temp,RGBW_rec,sizeof(RGBW_temp));
         }
      }
  }
  else  if(16 == ble_available())
        {
            if(rec_start_flag == 1)
            {
                RGBW_rec[receive_count*4][1] = ble_read();
                RGBW_rec[receive_count*4][2] = ble_read();
                RGBW_rec[receive_count*4][3] = ble_read();
                RGBW_rec[receive_count*4][0] = ble_read();
                for(h=0;h<4;h++)
                    {
                        Serial.println(RGBW_rec[receive_count*4][h],HEX);
                    }    
                    
                
                RGBW_rec[receive_count*4+1][1] = ble_read();
                RGBW_rec[receive_count*4+1][2] = ble_read();
                RGBW_rec[receive_count*4+1][3] = ble_read();
                RGBW_rec[receive_count*4+1][0] = ble_read();
                for(h=0;h<4;h++)
                    {
                        Serial.println(RGBW_rec[receive_count*4+1][h],HEX);
                    }
                    
                
                RGBW_rec[receive_count*4+2][1] = ble_read();
                RGBW_rec[receive_count*4+2][2] = ble_read();
                RGBW_rec[receive_count*4+2][3] = ble_read();
                RGBW_rec[receive_count*4+2][0] = ble_read();
                for(h=0;h<4;h++)
                    {
                        Serial.println(RGBW_rec[receive_count*4+2][h],HEX);
                    }
                    
                
                RGBW_rec[receive_count*4+3][1] = ble_read();
                RGBW_rec[receive_count*4+3][2] = ble_read();
                RGBW_rec[receive_count*4+3][3] = ble_read();
                RGBW_rec[receive_count*4+3][0] = ble_read();
                 for(h=0;h<4;h++)
                    {
                        Serial.println(RGBW_rec[receive_count*4+3][h],HEX);
                    }
               
                receive_count = receive_count+1;
                if(receive_count == 64)
                {
                    receive_count=0;
                    rec_start_flag = 0;
                    rec_RGBW_fin = 1;

                }
            }
            else 
            {
                for(i=0;i<4;i++)
               {
                 for(h=0;h<4;h++)
                 {
                   error_temp=ble_read();
                   Serial.println(error_temp,HEX);
                 }
               }
            }
        }
  else if(6 == ble_available())
  {
      for(h=0;h<6;h++)
     {
       XYRGBW_rec[h]=ble_read();
       Serial.println(XYRGBW_rec[h],HEX);
     }
      if((XYRGBW_rec[0]<16)&&(XYRGBW_rec[1]<16))
      {
          i = XYRGBW_rec[1]*16+XYRGBW_rec[0];
          RGBW_temp[i][1] = XYRGBW_rec[2];
          RGBW_temp[i][2] = XYRGBW_rec[3];
          RGBW_temp[i][3] = XYRGBW_rec[4];
          RGBW_temp[i][0] = XYRGBW_rec[5];
      }
  }
  //}
  ble_do_events();
}

// Fill the dots one after the other with a color
void colorWipe() 
{
    /*time_250ms_count++;
    
    if(time_250ms_count == 10)
    {
        switch(pic_count)
      {
        case 0: 
                memcpy(RGBW_temp,p,sizeof(RGBW_temp));
                break;
        case 1:
                memcpy(RGBW_temp,pq,sizeof(RGBW_temp));
                break;
        case 2:
                memcpy(RGBW_temp,pa,sizeof(RGBW_temp));
                break;
                
        default:break;
      }
      pic_count++;
      time_250ms_count = 0;
      
    }
    if(pic_count == 3)
    {
        pic_count = 0;
    }*/
    RGBW_Ic_Shift_Show(RGBW_temp);
}
