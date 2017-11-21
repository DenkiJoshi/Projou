/**************************************************************
 * TechnoAccessory FullcolorLED parts
 * Fullcolor Illumination
 **************************************************************
    author denkijoshi
    MPLAB X IDE v3.00
    microchip XC8 compiler 1.35
---------------------------------------------------------------
Date    :Time    Version    Build    Note
20160824 ----    1.00    3.26    Release version.
20161214 ----    1.01    3.35    buttontime short

---------------------------------------------------------------
port assignment
1 N/C           8 GP3 I SW 
2 VDD P 3.0V    7 VSS P
3 GP2 O BLUE    6 N/C
4 GP1 O RED     5 GP0 O GREEN

VDD 2.2V to 5V
GP3 is MUST do PULLUP 10K OHM,if you use Pickit2/ICD2 is active.

*/
#include <xc.h>

#define MHz 000000 // for delay_ms(x) definition
#define _XTAL_FREQ 4MHz // for delay_ms(x) definition

//LED port define
#define port_red GP1
#define port_green GP0
#define port_blue GP2

//LED polarity setting. Anode common
#define off 1
#define on 0

//Chip configuration
//file:///C:/Program%20Files/Microchip/xc8/v1.36/docs/chips/12f222.html
// Watchdog Timer Enable bit: WDT disabled
// Code protection bit: Code protection off
// Master Clear Pull-up Enable bit: Pull-up enabled
// GP3/MCLR Pin Function Select bit: GP3/MCLR pin function is GP3
// Internal Oscillator Frequency Select bit: 4 MHz
#pragma config WDTE = OFF, CP = OFF, MCPU = ON, MCLRE = OFF, IOSCFS = 4MHZ

//Parameter setting
unsigned char state,temp;
unsigned int outcnt;
bit sleepEN;
unsigned char pwm_red, pwm_green ,pwm_blue;
void stay(unsigned char, unsigned char, unsigned char, unsigned int);
void change(unsigned char, unsigned char, unsigned char, unsigned int);

void delay_10ms(unsigned char temp){
	while(temp--){
		__delay_ms(10);
	}	
}
void all_off(void)
{
    port_red = off;
    port_green = off;
    port_blue = off;
}

main(void){ 
    OPTION   = 0b00010111;
    TRISGPIO = 0b11111000;
    GPIO = 0b11111111;
    ANS1 = ANS0 = 0;//digital port set
        
    sleepEN=!GP3;

    pwm_red = pwm_green = pwm_blue = 0;
    state = 0;

    while(1){
       
        switch (state){
            //-------Setting erea start--------
            case 0:
                change(0,120,0,8);
                state = 1;
                break;
            case 1:
                change(0,120,120,8);
                state = 2;
                break;
            case 2:
                change(0,0,120,8);
                state = 3;
                break;
            case 3:
                change(120,0,120,8);
                state = 4;
                break;
            case 4:
                change(120,0,0,8);
                state = 5;
                break;
            case 5:
                change(120,120,0,8);
                state = 6;
                break;
            case 6:
                change(0,0,0,8);
                state = 7;
                break;
            case 7:
                stay(0,0,0,1000);
                state = 8;
                break;
            //-------Setting erea end--------
            default:
                state=0;
        }
    }
}

void stay(unsigned char red_value, unsigned char green_value, unsigned char blue_value, unsigned int time)
{
    unsigned char count = 0;
    while(time--){
        for (count=1; count!=255; count++){ //pwm
            if(count < red_value) port_red = on; else port_red = off;
            if(count < green_value) port_green = on; else port_green = off;
            if(count < blue_value) port_blue = on; else port_blue = off;
            if(GP3==0 && sleepEN==on){
                delay_10ms(5);
                if(GP3==0){
                    all_off();
                    delay_10ms(50);
                    temp=GPIO;
                    sleepEN=off;
                    asm("sleep");
                }
            }
            if(GP3==0 && sleepEN==off){
                delay_10ms(5);
                if(GP3==0){
                    delay_10ms(50);
                    sleepEN=on;
                }
            }
        }
    }
}

void change(unsigned char red_value, unsigned char green_value, unsigned char blue_value, unsigned int cycle)
{
    unsigned char count = 0;

    while (1){
        if (pwm_red < red_value) pwm_red++; else if (pwm_red > red_value) pwm_red--;
        if (pwm_green < green_value) pwm_green++; else if (pwm_green > green_value) pwm_green--;
        if (pwm_blue < blue_value) pwm_blue++; else if (pwm_blue > blue_value) pwm_blue--;

        if(pwm_red == red_value && pwm_green == green_value && pwm_blue == blue_value) break;

        outcnt = cycle;
        while(outcnt--){
            for (count=1; count!=255; count++){ //pwm
                if (count < pwm_red) port_red = on; else port_red = off;
                if (count < pwm_green) port_green = on; else port_green = off;
                if (count < pwm_blue) port_blue = on; else port_blue = off;
                if(GP3==0 && sleepEN==on){
                    delay_10ms(5);
                    if(GP3==0){
                        all_off();
                        delay_10ms(50);
                        temp=GPIO;
                        sleepEN=off;
                        asm("sleep");
                    }
                }
                if(GP3==0 && sleepEN==off){
                    delay_10ms(5);
                    if(GP3==0){
                        delay_10ms(50);
                        sleepEN=on;
                    }
                }
            }
        }
    }
}
