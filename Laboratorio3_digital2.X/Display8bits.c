/*
 * File:   Display8bits.c
 * Author: electrosome
 * Programacion modificada para configuracion de 8 bits, obtenida en :https://electrosome.com/category/projects/embedded-projects/pic-microcontroller-projects/
 * Created on 1 de febrero de 2021, 06:51 PM
 */
//LCD Functions Developed by electroSome
#ifndef EN
#define EN
#endif
#ifndef RS
#define RS
#endif
#define _XTAL_FREQ 400000
#include "Display8bits.h"
void Lcd_Port(char a){
    PORTA = a; 
}
void Lcd_Cmd(char a){
	RS = 0;             // => RS = 0
	Lcd_Port(a);
	EN  = 1;             // => E = 1
    __delay_ms(4);
    EN  = 0;             // => E = 0
}
void Lcd_Clear(void){
	Lcd_Cmd(0);
	Lcd_Cmd(1);
}
void Lcd_Set_Cursor(char a, char b){
	char temp;
	if(a == 1)
	{
	  temp = 0x80 + b - 1;
      Lcd_Cmd(temp);
	}
	else if(a == 2)
	{
		temp = 0xC0 + b - 1;
        Lcd_Cmd(temp);
	}
}
void Lcd_Init(){
  Lcd_Port(0x00);
   __delay_ms(20);
  Lcd_Cmd(0x30);
	__delay_ms(5);
  Lcd_Cmd(0x30);
	__delay_us(80);
  Lcd_Cmd(0x30);
  /////////////////////////////////////////////////////
  Lcd_Cmd(0x38);
  Lcd_Cmd(0x08);
  Lcd_Cmd(0x01);
  Lcd_Cmd(0x06);
}
void Lcd_Write_Char(char a){
   RS = 1;             // => RS = 1
   Lcd_Port(a);             //Data transfer
   EN = 1;
   __delay_us(40);
   EN = 0;
}
void Lcd_Write_String(char *a){
	int i;
	for(i=0;a[i]!='\0';i++)
	   Lcd_Write_Char(a[i]);
}
void Lcd_Shift_Right(){
	Lcd_Cmd(0x01);
	Lcd_Cmd(0x0C);
}
void Lcd_Shift_Left(){
	Lcd_Cmd(0x01);
	Lcd_Cmd(0x08);
}
