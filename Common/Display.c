#include "Display.h"
#include <xc.h>
#include <stdlib.h>
#include "timers.h"
#include "Board.h"
#include <stdint.h>

#define MSG_TIME 50
#define NOP() asm("nop")

uint8_t LCD_Init() {

    REG_TRIS = 0;
    ENA_TRIS = 0;
    DB4_TRIS = 0;
    DB5_TRIS = 0;
    DB6_TRIS = 0;
    DB7_TRIS = 0;
    REG_LAT = 0;
    ENA_LAT = 0;
    DB4_LAT = 0;
    DB5_LAT = 0;
    DB6_LAT = 0;    
    DB7_LAT = 0;

    while (TIMERS_GetMilliSeconds() % 16 != 0) {
        NOP();
    }
    
    LCD_Sendcommand(0x30); 
    
    while(TIMERS_GetMilliSeconds() % 5 != 0){
        NOP();
    }
    LCD_Sendcommand(0x30);
    while(TIMERS_GetMicroSeconds() % 110 != 0){
        NOP();
    }
    LCD_Sendcommand(0b00110000);
    LCD_Sendcommand(0x20);
    LCD_Sendcommand(0x20);
    LCD_Sendcommand(0xC0);
    LCD_Sendcommand(0x00);
    LCD_Sendcommand(0x80);
    LCD_Sendcommand(0x00);
    LCD_Sendcommand(0x10);
    LCD_Sendcommand(0x00);
    LCD_Sendcommand(0x20);
    
    return 1;
}

uint8_t LCD_Cleardisplay() {

    return 1;
}

//separates 8bit message into two halves (upper/lower nibbles) and prints character

char LCD_Sendchar(char Char_Out) {

    char Upper_Half, Lower_Half;
  
    Upper_Half = Char_Out & 0xF0;
    Lower_Half = Char_Out & 0x0F;

    //logic based on 4 bit timing diagram 
    REG_LAT = 1;
     
    LCD_Setpins(Upper_Half >> 4);

    ENA_LAT = 1;

    //allow sufficient time for message to send (at least 40us)
    while (TIMERS_GetMicroSeconds() % MSG_TIME != 0) {
        asm("nop");
    }

    ENA_LAT = 0;

    LCD_Setpins(Lower_Half);

    ENA_LAT = 1;

    while (TIMERS_GetMicroSeconds() % MSG_TIME != 0) {
        asm("nop");
    }
    ENA_LAT = 0;
    int i = 1;
    return (Upper_Half | Lower_Half);
}

//goes through string, sending one character at a time until null terminator found

uint8_t LCD_Sendstring(char* String_Out) {

    return 1;
}

//sets the 4 data pins HI or LO depending on received command from send string

char LCD_Setpins(char Pins) {

    if (Pins & 1) {
        DB4_LAT = 1;
    } else {
        DB4_LAT = 0;
    }
    if (Pins & 2) {
        DB5_LAT = 1;
    } else {
        DB5_LAT = 0;
    }
    if (Pins & 4) {
        DB6_LAT = 1;
    } else {
        DB6_LAT = 0;
    }
    if (Pins & 8) {
        DB7_LAT = 1;
    } else {
        DB7_LAT = 0;
    }
    return (DB7_LAT << 8 | DB6_LAT << 7 | DB5_LAT << 6 | DB4_LAT << 5);

}

//sends command from uC32 to the LCD display

uint8_t LCD_Sendcommand(char command) {

    REG_LAT = 0;
    LCD_Setpins(command);
    ENA_LAT = 1;
    int cur_time = 0;
    cur_time = TIMERS_GetMilliSeconds();
    while (TIMERS_GetMilliSeconds() % 5 != 0) {
        NOP();
    }
    ENA_LAT = 0;
    return 1;
}
int main() {
    BOARD_Init();
    TIMERS_Init();
    LCD_Init();
    char A = 0b01000001;
    char test = LCD_Sendchar(A);
    int i = 1;
    while(1);
    return (EXIT_SUCCESS);
}