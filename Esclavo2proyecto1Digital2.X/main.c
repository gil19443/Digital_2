/*
 * File:   main.c
 * Author: Carlos Gil
 *
 * Created on 12 de febrero de 2021, 02:40 AM
 */
//******************************************************************************
//                      Importacion de librerias 
//******************************************************************************
#include <xc.h>
#include <stdint.h>
#include "Libreria2.h"
#include "SPI.h"
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
//******************************************************************************
//                          Variables 
//******************************************************************************
uint8_t boton1 = 0;
uint8_t boton2 = 0;
//******************************************************************************
//                          Prototipos de funciones 
//******************************************************************************
void setup(void);
//******************************************************************************
//                          interrupciones 
//******************************************************************************
void __interrupt() isr(void){
    if (INTCONbits.RBIF == 1){ //interrupciones del puerto B
        INTCONbits.RBIF = 0; //se limpia la bander ade interrupcion 
        if (PORTBbits.RB0 == 0){ //rutinas de antirebote para incrementar/decrementar 
            boton1 = 1; //el contados de 8bits
        }
        if (PORTBbits.RB0 && boton1 == 1){
            boton1  = 0;
            PORTD++;
        }
         if (PORTBbits.RB1 == 0){
            boton2 = 1;
        }
        if (PORTBbits.RB1 && boton2 == 1){
            boton2  = 0;
            PORTD--;
        }
    }
    if (PIR1bits.SSPIF == 1){ //interrupcion del SSP
        SSPBUF = PORTD; //se mandan los valores del contador al master 
        PIR1bits.SSPIF == 0;
    }
}
//******************************************************************************
//                          Ciclo principal 
//******************************************************************************

void main(void) {
    setup();
    //**************************************************************************
    //                             mian loop
    //**************************************************************************
    while (1) {

    }
}


//******************************************************************************
//                          Setup
//******************************************************************************

void setup(void) {
    initOsc(7);//configura el osculador interno a 4Mhz
    OPTION_REG = 0b01010111; //configuracion para activar las PULL - UPS del puerto B y timer 0
    ANSEL = 0;
    ANSELH = 0; //se borran las entradas analogicas 
    PORTC = 0; //se resetea el puerto C
    TRISC = 0; //se selecciona el puerto C como salida 
    PORTD = 0; //se resetea el puerto D
    TRISD = 0; //se selecciona el puerto D como salida 
    PORTB = 0; //se resetea el puerto B
    TRISE = 0; // se marca el puerto E como salida
    PORTE = 0;  //se resetea el puerto E
    PORTA = 0;
    TRISBbits.TRISB0 = 1; //se seleccionan como entradas los pines donde van los botones 
    TRISBbits.TRISB1 = 1;
    TRISCbits.TRISC5 = 0; //se selecciona el  RC5 como salida 
    TRISCbits.TRISC4 = 0;//se selecciona el  RC4 como salida 
    TRISCbits.TRISC3 = 1;//se selecciona el  RC3 como entrada 
    TRISAbits.TRISA5 = 1; //se selecciona el  RA5 como salida 
    WPUB = 0b00000011; //0000 0111 pines para pull up 
    IOCB = 0b00000011; //se configura para que los botones usen la interrupcion 
    //configuracion del SSP en modo esclavo 
    spiInit(SPI_SLAVE_SS_EN, SPI_DATA_SAMPLE_MIDDLE, SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE);
//****************************interrupcinoes************************************
    INTCONbits.GIE = 1; //se activan las interrupciones globales 
    INTCONbits.PEIE = 1; // se activan las interrupciones perifericas 
    INTCONbits.RBIE = 1; //se activan las interrupciones del puerto b
    PIE1bits.SSPIE = 1; //se activan las interrupciones del SSP
//******************************************************************************
}

