/**************************************************************
 * File:   sdgs.c
 * Hard:   Projou(pic10f322) VDD 2.3V-5.5V
 **************************************************************
 * port assignment
 * 1 RA0 O RED    6 RA3 I NC
 * 2 VSS P GND    5 VDD P 3.7V
 * 3 RA1 O BLUE   4 RA2 O GREEN
 *
 * GP3 is MUST do PULLUP 10K OHM,if you use Pickit2/ICD2 is active.
 *
 * Author: shibuki
 * Created on 2020/05/19
 */

#include <xc.h>

//LED port define
#define port_red RA0
#define port_green RA2
#define port_blue RA1

//LED polarity setting. Anode common
#define off 1
#define on 0

//Chip configuration
//file:///C:/Program%20Files/Microchip/xc8/v2.20/docs/chips/12f322.html
// Oscillator Selection bits : INTOSC: CLKIN function disabled
// Brown-out Reset Enable : Brown-out Reset disabled
// Watchdog Timer Enable : WDT disabled
// Power-up Timer Enable bit : PWRT disabled
// MCLR Pin Function Select bit : MCLR pin function is ?
// Code Protection bit : Program memory code protection is disabled
// Low-Voltage Programming Enable : Low-voltage programming disabled
// Brown-out Reset Selection bits : BOR enabled
// Brown-out Reset Voltage Selection : Brown-out Reset Voltage (Vbor), low trip point selected.
// Flash Memory Self-Write Protection : Write protection off
#pragma config FOSC = INTOSC, BOREN = OFF, WDTE = OFF, PWRTE = OFF, MCLRE = OFF, CP = OFF, LVP = OFF, LPBOR = ON, BORV = LO, WRT = OFF

#define _XTAL_FREQ 250000

//Parameter setting
unsigned char state;
int staytimer;
unsigned char pwm_red, pwm_green ,pwm_blue;
void change(unsigned char, unsigned char, unsigned char, unsigned char);
void stay(unsigned char, unsigned char, unsigned char, unsigned char);

void main(void) {    
    TRISA = 0;//Output
    ANSELA = 0;//for Digital I/O

    pwm_red = pwm_green = pwm_blue = 0;

    while(1){
        switch(state){
     
            //Programming change start.(Red,Green,Blue,Time[sec])
            //case 0: stay(240, 20, 30, 4); break;

            case 0: stay(240, 0, 0, 4); break;
            case 1: stay(102, 73, 0, 4); break;
            case 2: stay(40, 240, 0, 4); break;
            case 3: stay(160, 0, 40, 4); break;
            case 4: stay(180, 20, 0, 4); break;
            case 5: stay(130, 200, 255, 4); break;
            case 6: stay(179, 128, 0, 4); break;
            case 7: stay(240, 20, 30, 4); break;
            case 8: stay(200, 150, 0, 4); break;
            case 9: stay(240, 100, 180, 4); break;
            case 10: stay(180, 100, 0, 4); break;
            case 11: stay(200, 150, 0, 4); break;
            case 12: stay(0, 240, 0, 4); break;
            case 13: stay(0, 120, 255, 4); break;
            case 14: stay(80, 240, 0, 4); break;
            case 15: stay(0, 30, 210, 4); break;
            case 16: stay(0, 0, 240, 4); break;

    
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
