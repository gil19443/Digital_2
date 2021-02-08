/*
 * File:   TablaACSII_HEX.c
 * Author: Carlos Gil
 *
 * Created on 4 de febrero de 2021, 05:19 PM
 */

#include <stdint.h>
#include <xc.h>
#include "TablaACSII_HEX.h"
void tabla_hex (uint8_t selector, volatile uint8_t *port){ //recibe un selector 
switch (selector){ //modifica el valor del puntero para pasarlo a HEX en ACSII
        case 0:
            *port = 48;
            break;
        case 1:
            *port = 49;
            break;
        case 2:
            *port = 50;
            break;
        case 3:
            *port = 51;
            break;
        case 4:
           *port = 52;
            break;
        case 5:
            *port = 53;
            break;
        case 6:
            *port = 54;
            break;
        case 7:
            *port = 55;
            break;
        case 8:
            *port = 56;
            break;
        case 9:
            *port = 57;
            break;
        case 10:
            *port = 65;
            break;
        case 11:
            *port = 66;
            break;    
        case 12:
            *port = 67;
            break;
        case 13:
            *port = 68;
            break;
        case 14:
            *port = 69;
            break;
        case 15:
            *port = 70;
            break;
        default:
            *port = 48;
            break;
        
}      
    
}