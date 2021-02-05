/*
 * File:   usart9600.c
 * Author: Carlos Gil
 *
 * Created on 4 de febrero de 2021, 04:48 PM
 */


#include <xc.h>
#include "usart9600.h"

void initUSART (void){ //configuracion para recibir y enviar con un baudrate de
    TXSTAbits.TX9 = 0; //9600
    TXSTAbits.TXEN = 1;
    TXSTAbits.SYNC = 0;
    TXSTAbits.BRGH = 1;
    BAUDCTLbits.BRG16 = 0;
    SPBRG = 25;
    SPBRGH = 0;
    RCSTAbits.SPEN = 1;
    RCSTAbits.RX9 = 0;
    RCSTAbits.CREN = 1;
}
