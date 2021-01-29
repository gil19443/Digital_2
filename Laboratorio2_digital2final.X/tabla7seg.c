/*
 * File:   tabla7seg.c
 * Author: Carlos Gil
 *
 * Created on 28 de enero de 2021, 06:21 PM
 */


#include "tabla7seg.h"
void tabla(uint8_t numero){
    switch (numero){
        case 0:
            PORTC = 0b11000000;
            break;
        case 1:
            PORTC = 0b11111001;
            break;
        case 2:
            PORTC = 0b10100100;
            break;
        case 3:
            PORTC = 0b10110000;
            break;
        case 4:
            PORTC = 0b10011001;
            break;
        case 5:
            PORTC = 0b10010010;
            break;
        case 6:
            PORTC = 0b10000010;
            break;
        case 7:
            PORTC = 0b11111000;
            break;
        case 8:
            PORTC = 0b10000000;
            break;
        case 9:
            PORTC = 0b10010000;
            break;
        case 10:
            PORTC = 0b10001000;
            break;
        case 11:
            PORTC = 0b10000011;
            break;
        case 12:
            PORTC = 0b11000110;
            break;
        case 13:
            PORTC = 0b10100001;
            break;
        case 14:
            PORTC = 0b10000110;
            break;
        case 15:
            PORTC = 0b10001110;
            break;         
}
}
