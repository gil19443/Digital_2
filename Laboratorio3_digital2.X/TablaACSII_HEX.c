/*
 * File:   TablaACSII_HEX.c
 * Author: Carlos Gil
 *
 * Created on 4 de febrero de 2021, 05:19 PM
 */

#include <stdint.h>
#include <xc.h>
#include "TablaACSII_HEX.h"
void tabla_hex (uint8_t selector, uint8_t *puerto){
switch (selector){
    case 0:
        *puerto = 48;
        break;
    case 1:
        *puerto = 49;
        break;
    case 2:
        *puerto = 50;
        break;
    case 3:
        *puerto = 51;
        break;
    case 4:
        *puerto = 52;
        break;
    case 5:
        *puerto = 53;
        break;
    case 6:
        *puerto = 54;
        break;
    case 7:
        *puerto = 55;
        break;
    case 8:
        *puerto = 56;
        break;
    case 9:
        *puerto = 57;
        break;
    case 10:
        *puerto = 65;
        break;
    case 11:
        *puerto = 66;
        break;    
    case 12:
        *puerto = 67;
        break;
    case 13:
        *puerto = 68;
        break;
    case 14:
        *puerto = 69;
        break;
    case 15:
        *puerto = 70;
        break;
    default:
        *puerto = 48;
        break;
        
}      
    
}