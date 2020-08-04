/**************************************************************
 * File:   f-1_Fluctuation.c
 * Hard:   Projou(pic10f322) VDD 2.3V-5.5V
 **************************************************************
 * port assignment
 * 1 RA0 O RED    6 RA3 I NC
 * 2 VSS P GND    5 VDD P 3.7V
 * 3 RA1 O BLUE   4 RA2 O GREEN
 *
 * GP3 is MUST do PULLUP 10K OHM,if you use Pickit2/ICD2 is active.
 *
 * Author: denkijoshi
 * Created on 2020/08/04, 05:37
 */

#include <xc.h>

//LED port define
#define port_red RA0

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
unsigned char pwm_red;
unsigned char f1Fluctuation(void);
unsigned int rl = 234;
void yuragi(unsigned char);

char rnd_xorshift(){
	rl=rl^(rl<<3);
	rl=rl^(rl>>4);
	rl=rl^(rl<<1);
	rl=rl&0xff;
	return(rl);
}

void main(void) {
    TRISA = 0;//Output
    ANSELA = 0;//for Digital I/O
    
    yuragi(20);
    port_red = on;
    __delay_ms(180000);
    yuragi(10);
    port_red = on;
    __delay_ms(70000);
}

void yuragi(unsigned char outcycle){
    while(outcycle --){
        pwm_red = f1Fluctuation();
        unsigned char f1cycle = 50;
        while(f1cycle --){
            for (unsigned char count=1; count!=255; count++){ //pwm
                if (count < pwm_red) port_red = on; else port_red = off;
            }    
        }
    }
}

unsigned char f1Fluctuation(void){
    static unsigned int x = 256;
    if(x < 128){ x = x + 2 * x * x / 256;} 
    else { x = x - 2 * (256 - x) * (256 - x) / 256;}
    if(x < 5 || x > 255){ x = rnd_xorshift();}
    return((unsigned char)(x));
}
