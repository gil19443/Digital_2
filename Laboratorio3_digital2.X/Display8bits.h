/*
 * File:   Display8bits.c
 * Author: electrosome
 * Programacion modificada para configuracion de 8 bits, obtenida en :https://electrosome.com/category/projects/embedded-projects/pic-microcontroller-projects/
 * Created on 1 de febrero de 2021, 06:51 PM
 */
#ifndef EN
#define EN
#ifndef RS
#define RS
#ifndef DISPLAY8BITS_H
#define	DISPLAY8BITS_H

#include <xc.h> // include processor files - each processor file is guarded.  

void Lcd_Port(char a);
void Lcd_Cmd(char a);
void Lcd_Clear(void);
void Lcd_Set_Cursor(char a, char b);
void Lcd_Init(void);
void Lcd_Write_Char(char a)
void Lcd_Write_String(char *a);
void Lcd_Shift_Right(void);
void Lcd_Shift_Left(void);


#endif	/* DISPLAY8BITS */

