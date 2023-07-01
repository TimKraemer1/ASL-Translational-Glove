    /* 
 * File:   Display.h
 * Author: matt kaltman
 * code to communicate parallel with arduino LCD display
 * https://www.arduino.cc/documents/datasheets/LCDscreen.PDF
 * Created on June 6, 2023, 1:08 PM
 */



#include <stdint.h>

#ifndef DISPLAY_H
#define	DISPLAY_H
#endif

//defining pin outs 
#define REG_TRIS TRISFbits.TRISF2 //PIN 0
#define ENA_TRIS TRISFbits.TRISF3 //PIN 1
#define DB4_TRIS TRISDbits.TRISD8 //PIN 2
#define DB5_TRIS TRISDbits.TRISD0 //PIN 3
#define DB6_TRIS TRISFbits.TRISF1 //PIN 4
#define DB7_TRIS TRISDbits.TRISD1 //PIN 5

#define REG_LAT LATFbits.LATF2
#define ENA_LAT LATFbits.LATF3
#define DB4_LAT LATDbits.LATD8
#define DB5_LAT LATDbits.LATD0
#define DB6_LAT LATFbits.LATF1
#define DB7_LAT LATDbits.LATD1


uint8_t LCD_Init();

char LCD_Setpins(char Pins);

char LCD_Sendchar(char Char_Out);

uint8_t LCD_Sendstring(char *String_Out);

uint8_t LCD_Sendcommand(char Command);

uint8_t LCD_Cleardisplay(); 
