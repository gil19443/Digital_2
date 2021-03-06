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
#pragma config FOSC =  EXTRC_NOCLKOUT       // Oscillator Selection bits (XT oscillator: Crystal/resonator on RA6/OSC2/CLKOUT and RA7/OSC1/CLKIN)
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
#define _XTAL_FREQ 800000
unsigned char check = 0 ; 
unsigned char conteo1 = 0;
unsigned char conteo2 = 0;
unsigned char jugadores1 = 0;
unsigned char jugadores2 = 0;
//******************************************************************************
//                          Prototipos de funciones 
//******************************************************************************
void setup(void);
void luces (void);
void habilitar_semaforo (void);
void juego1(unsigned char jugadores1);
void juego2(unsigned char jugadores2);
void jugador1(void);
void jugador2(void);
//******************************************************************************
//                          Ciclo principal 
//******************************************************************************

void _main(void) {
    setup();
    //**************************************************************************
    //                             mian loop
    //**************************************************************************
    while (1) {
        habilitar_semaforo();
        if (check == 1) {
            jugador1();
            jugador2();
            juego1(jugadores1);
            juego2(jugadores2);
        }
    }
}


//******************************************************************************
//                          Setup
//******************************************************************************

void setup(void) {
    ANSEL = 0;
    ANSELH = 0;
    PORTA = 0;
    TRISA = 0;
    PORTC = 0;
    TRISC = 0;
    PORTD = 0;
    TRISD = 0;
    PORTB = 0;
    TRISBbits.TRISB0 = 1;
    TRISBbits.TRISB1 = 1;
    TRISBbits.TRISB2 = 1;
    TRISBbits.TRISB3 = 0;
    TRISBbits.TRISB4 = 0;
    TRISBbits.TRISB5 = 0;
    TRISE = 0;
    PORTE = 0;
    WPUB = 0b00000111; //0000 0111
    OPTION_REG = 0b01010111;
}

//******************************************************************************
//                          SUBRUTIANS
//******************************************************************************
void luces (void) {
    PORTE = 1;
    __delay_ms(200);
    PORTE = 2;
    __delay_ms(200);
    PORTE = 2;
    __delay_ms(200);
    PORTE = 0;
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
void jugador1 (void ){
    if (PORTBbits.RB1 == 0){
        conteo1 = 1;
    }
    if (PORTBbits.RB1 == 1 && conteo1 == 1){
        conteo1 = 0;
        jugadores1++;
    }
}
void jugador2 (void ){
    if (PORTBbits.RB2 == 0){
        conteo2 = 1;
    }
    if (PORTBbits.RB2 == 1 && conteo2 == 1){
        conteo2 = 0;
        jugadores2++;
    }
}
void juego1(unsigned char jugadores1){
    switch (jugadores1){
        case 0:
            PORTC = 1;
            break;
        case 8:
            PORTBbits.RB3 = 1;
            check = 0;
            PORTC = 0;
            jugadores1 = 0;
            break;
        default:
            PORTC = PORTC << 1;
                    
        
    }
}
void juego2(unsigned char jugadores2){
    switch (jugadores2){
        case 0:
            PORTD = 1;
            break;
        case 8:
            PORTBbits.RB4 = 1;
            check = 0;
            PORTD = 0;
            jugadores2 = 0;
            break;
        default:
            PORTD = PORTD << 1;
            break;        
        
    }
}