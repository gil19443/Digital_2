/*
 * File:   initSPI.c
 * Author: Carlos Gil
 *
 * Created on 11 de febrero de 2021, 02:15 AM
 */


#include <xc.h>
#include <stdint.h>
#include <pic16f887.h>

void initSPIMASTER(uint8_t modo) {
        SSPSTATbits.SMP = 1;
        SSPSTATbits.CKE = 1;
        SSPCONbits.SSPEN = 1;
        SSPCONbits.CKP = 0;
        switch(modo){
            case 0:
                SSPCONbits.SSPM3 = 0;//FOSC/4
                SSPCONbits.SSPM2 = 0;
                SSPCONbits.SSPM1 = 0;
                SSPCONbits.SSPM0 = 0;
                break;
            case 1:
                SSPCONbits.SSPM3 = 0;//FOSC/16
                SSPCONbits.SSPM2 = 0;
                SSPCONbits.SSPM1 = 0;
                SSPCONbits.SSPM0 = 1;
                break;
            case 2:
                SSPCONbits.SSPM3 = 0;//FOSC/64
                SSPCONbits.SSPM2 = 0;
                SSPCONbits.SSPM1 = 1;
                SSPCONbits.SSPM0 = 0;
                break;
            case 3:
                SSPCONbits.SSPM3 = 0;//TMR2 output 
                SSPCONbits.SSPM2 = 0;
                SSPCONbits.SSPM1 = 1;
                SSPCONbits.SSPM0 = 1;
                break;

        }
}
void initSPISLAVE(uint8_t modos){
        SSPSTATbits.SMP = 0;
        SSPSTATbits.CKE = 0;
        SSPCONbits.SSPEN = 1;
        SSPCONbits.CKP = 1;
        switch(modos){
            case 0:
                SSPCONbits.SSPM3 = 0;//TMR2 output 
                SSPCONbits.SSPM2 = 1;
                SSPCONbits.SSPM1 = 0;
                SSPCONbits.SSPM0 = 0;
                break;
            case 1:
                SSPCONbits.SSPM3 = 0;//TMR2 output 
                SSPCONbits.SSPM2 = 1;
                SSPCONbits.SSPM1 = 0;
                SSPCONbits.SSPM0 = 1;
                break;
        }
}
                

