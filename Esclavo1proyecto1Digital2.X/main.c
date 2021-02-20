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
#include "configuracionADC.h"
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
uint8_t ADC = 0;
uint8_t ADC_GO = 0;
uint8_t toggle = 0;
uint8_t display1 = 0;
uint8_t display0 = 0;
uint8_t boton1 = 0;
uint8_t boton2 = 0;
//******************************************************************************
//                          Prototipos de funciones 
//******************************************************************************
void setup(void);
void GO_ADC (void );
//******************************************************************************
//                          interrupciones 
//******************************************************************************
void __interrupt() isr(void){
    if (INTCONbits.TMR0IF == 1){ //interrupcion del TMR0
        TMR0 = 236; //desborde cada 5ms
        ADC_GO++; //bandera de control del ADC GO
        INTCONbits.TMR0IF = 0; //se limpia la bandera de interrupcion 
        
    }
    if (PIR1bits.ADIF == 1){ //interrupcion del ADC
        ADC = ADRESH; //se guardan los valores del ADC en una variable 
        PIR1bits.ADIF = 0;//se limpia la bandera de interrupcion 
    }
    if (PIR1bits.SSPIF = 1){//interrupcion del SSP
        SSPBUF = ADC; //se mandan los valores del ADC al master
        PIR1bits.SSPIF = 0;//se limpia la bandera de interrupcion 
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
        GO_ADC();//rutina que da el GO del ADC al master 
        PORTB = ADC; //monitoreo de lectura del ADC
    }
}


//******************************************************************************
//                          Setup
//******************************************************************************

void setup(void) {
    initOsc(6);//configura el osculador interno a 4Mhz
    configADC(0,2); //canal 0 y velocidad FOSC/32
    OPTION_REG = 0b01010111; //configuracion para activar las PULL - UPS del puerto B y timer 0
    ANSEL = 0;
    ANSELbits.ANS0 = 1;//se activa la entrada analogica del RA0
    ANSELH = 0; //se borran las entradas analogicas 
    PORTC = 0; //se resetea el puerto C
    TRISC = 0; //se selecciona el puerto C como salida 
    PORTD = 0; //se resetea el puerto D
    TRISD = 0; //se selecciona el puerto D como salida 
    TRISB = 0;
    PORTB = 0; //se resetea el puerto B
    TRISE = 0; // se marca el puerto E como salida
    PORTE = 0;  //se resetea el puerto E
    PORTA = 0;
    TRISAbits.TRISA0 = 1; //se seleciona el RA0 como entrada 
    TRISCbits.TRISC5 = 0; //se selecciona el RC5 como salida 
    TRISAbits.TRISA5 = 1; //se selecciona el RA5 como entrada
    //configuracion del SSP en modo slave 
    spiInit(SPI_SLAVE_SS_EN, SPI_DATA_SAMPLE_MIDDLE, SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE);
//****************************interrupcinoes************************************
    INTCONbits.GIE = 1; //se activan las interrupciones globales 
    INTCONbits.PEIE = 1; // se activan las interrupciones perifericas 
    INTCONbits.TMR0IE = 1; //se activan las interrupciones del timer 0
    PIE1bits.ADIE = 1; //activar las interrupciones del ADC
    PIE1bits.SSPIE = 1; //se activa la interrupcion del SSP
    PIR1bits.SSPIF = 0; //se limpia la bandera de interrupcion del SSP
//******************************************************************************
}
void GO_ADC (void){
       if (ADC_GO > 10){ //cuando ADC_GO llega a 10, da el GO del ADC
           ADC_GO = 0;
           ADCON0bits.GO_nDONE = 1;
       } 
    }

