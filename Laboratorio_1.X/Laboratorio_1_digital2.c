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
//******************************************************************************
//                             Palabras de configuracion 
//******************************************************************************
// CONFIG1
#pragma config FOSC = XT        // Oscillator Selection bits (XT oscillator: Crystal/resonator on RA6/OSC2/CLKOUT and RA7/OSC1/CLKIN)
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
//******************************************************************************
//                          Variables 
//******************************************************************************
# define _XTAL_ 800000
unsigned char check; 
unsigned char conteo1;
unsigned char conteo2;
//******************************************************************************
//                          Prototipos de funciones 
//******************************************************************************
void setup(void);
void luces (void);
void ganador (void);
//******************************************************************************
//                          Ciclo principal 
//******************************************************************************

void main(void) {
    setup();
    //**************************************************************************
    //                             mian loop
    //**************************************************************************
    while (1) {
        ganador ();
        
        if (check == 1) {
            
        }
    }
}


//******************************************************************************
//                          Setup
//******************************************************************************

void setup(void) {
    ANSEL = 0;
    ANSELH = 0;
    PORTC = 0;
    TRISC = 0;
    PORTD = 0;
    TRISD = 0;
    TRISBbits.TRISB0 = 1;
    TRISBbits.TRISB1 = 1;
    TRISBbits.TRISB2 = 1;
    TRISBbits.TRISB3 = 0;
    TRISBbits.TRISB4 = 0;
    TRISE = 0;
    PORTE = 0;
    WPUB = 0x10;
    OPTION_REG = 0b01010111;
}

//******************************************************************************
//                          SUBRUTIANS
//******************************************************************************
void luces (void) {
    PORTE = 1;
    __DELAY_MS 200;
    PORTE = 2;
    __DELAY_MS 200;
    PORTE = 2;
    __DELAY_MS 200;
    PORTE = 0;
}
void ganador (void) {
    if (conteo1 == 8) {
        PORTBbits.RB3 = 1;
    }
    if (conteo2 == ) {
        PORTBbits.RB3 = 1;
    }
}
void habilitar_semaforo (void){
    if (PORTBbits.RB0 == 0){
            luces();
            check = 1;
            PORTD = 0;
            PORTC = 0;
            conteo1 = 0;
            conteo2 = 0;
            PORTBbits.RB3 = 0;
            PORTBbits.RB4 = 0;
        }
}