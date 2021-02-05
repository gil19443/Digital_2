/*
 * File:   main.c
 * Author: Carlos Gil
 *
 * Created on 18 de enero de 2021, 02:53 PM
 */
//******************************************************************************
//                      Importacion de librerias 
//******************************************************************************
#include <xc.h>
#include <stdint.h>
#include "Libreria2.h"
#include "usart9600.h"
//******************************************************************************
//                             Palabras de configuracion 
//******************************************************************************
// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT        // Oscillator Selection bits (XT oscillator: Crystal/resonator on RA6/OSC2/CLKOUT and RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)
#define _XTAL_FREQ 400000
void setup(void);
void ADC_GO(void);
uint8_t controlADC = 0;
void __interrupt() isr(void){
    if (INTCONbits.TMR0IF == 1){
        TMR0 = 236; //valor del timer para 5ms
        controlADC++;
        INTCONbits.TMR0IF = 0; //limpiar la bandera 
    }

//    if (PIR1bits.TXIF == 1){
//        TXREG = 0x30;
//        PIE1bits.TXIE = 0;
//        
//    }
}
void main(void) {
    setup();
    //**************************************************************************
    //                             mian loop
    //**************************************************************************
    while (1) {
          ADC_GO();
          TXREG = 48;
    }
}
void setup(void) {
    initUSART();
    initOsc(6);
    OPTION_REG = 0b11010111;//configura el osculador interno a 4Mhz
     //canal 0 y velocidad FOSC/32
    WDTCON = 0;
    ANSEL = 0;
    ANSELH = 0;
    PORTC = 0; //se resetea el puerto C
    TRISC = 0x80; //se selecciona el puerto C como salida 
    PORTD = 0; //se resetea el puerto D
    TRISD = 0; //se selecciona el puerto D como salida 
    PORTB = 0; //se resetea el puerto B
    TRISE = 0; // se marca el puerto E como salida
    PORTE = 0;  //se resetea el puerto E
    PORTA = 0;
    TRISA = 0;
    TRISB = 0;
    TRISAbits.TRISA0 = 1; //EL A0 como entrada 
    TRISAbits.TRISA1 = 1; //RB6 como entrada 
//****************************interrupcinoes************************************
    INTCONbits.GIE = 1; //se activan las interrupciones globales 
    INTCONbits.PEIE = 1; // se activan las interrupciones perifericas 
//    PIE1bits.TXIE = 1;
    INTCONbits.TMR0IE = 1;
}
void ADC_GO (void){
    if (controlADC > 10){ //revisa si el contador es mayor a 10 para dar el GO 
        controlADC = 0;
       // ADCON0bits.GO_nDONE = 1;
       // PIE1bits.TXIE = 1;
    }
}