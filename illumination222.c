/**************************************************************
 * File:   illumination222.c
 * Hard:   Projou(pic10f222) VDD 2.0V-5.5V
 **************************************************************
 * port assignment
 * 1 GP0 O RED    6 GP3 I NC
 * 2 VSS P GND    5 VDD P 3.7V
 * 3 GP1 O BLUE   4 GP2 O GREEN
 *
 * GP3 is MUST do PULLUP 10K OHM,if you use Pickit2/ICD2 is active.
 *
 * Author: denkijoshi
 * Created on 2020/05/29, 9:26
 */

#include <xc.h>

//LED port define
#define port_red GP0
#define port_green GP2
#define port_blue GP1

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
unsigned char state;
int staytimer;
unsigned char pwm_red, pwm_green ,pwm_blue;
void change(unsigned char, unsigned char, unsigned char, unsigned char);
void stay(unsigned char, unsigned char, unsigned char, unsigned char);

void main(void) {
    OPTION   = 0b00010111;
    TRISGPIO = 0b11111000;
    GPIO = 0b11111111;
    ANS1 = ANS0 = 0;//digital port set

    pwm_red = pwm_green = pwm_blue = 0;

    while(1){
        switch(state){
     
        //Programming change start.(Red,Green,Blue,Time[sec])

    case 0: change(0, 240, 0, 8); break;
    case 1: change(0, 240, 240, 8); break;
    case 2: change(0, 0, 240, 8); break;
    case 3: change(240, 0, 240, 8); break;
    case 4: change(240, 0, 0, 8); break;
    case 5: change(240, 240, 0, 8); break;
    
        //Programming change end.
        
        default: state=0;
        }
    }
}

void change(unsigned char red_value, unsigned char green_value, unsigned char blue_value, unsigned char speed)
{
    if (pwm_red < red_value) pwm_red++; else if (pwm_red > red_value) pwm_red--;
    if (pwm_green < green_value) pwm_green++; else if (pwm_green > green_value) pwm_green--;
    if (pwm_blue < blue_value) pwm_blue++; else if (pwm_blue > blue_value) pwm_blue--;

    if(pwm_red == red_value && pwm_green == green_value && pwm_blue == blue_value) state++;

    while(speed--){
        for (unsigned char count=1; count!=255; count++){ //pwm
            if (count < pwm_red) port_red = on; else port_red = off;
            if (count < pwm_green) port_green = on; else port_green = off;
            if (count < pwm_blue) port_blue = on; else port_blue = off;
        }
    }
}

void stay(unsigned char red_value, unsigned char green_value, unsigned char blue_value, unsigned char cycle)
{
    staytimer++;
    if(staytimer == cycle*60) { state++; staytimer = 0;}
    
    for (unsigned char count=1; count!=255; count++){ //pwm
        if (count < red_value) port_red = on; else port_red = off;
        if (count < green_value) port_green = on; else port_green = off;
        if (count < blue_value) port_blue = on; else port_blue = off;
    }
}
