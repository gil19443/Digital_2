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
#include "tabla7seg.h"
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
uint8_t toggle = 0;
uint8_t display1 = 0;
uint8_t display0 = 0;
uint8_t boton1 = 0;
uint8_t boton2 = 0;
uint8_t controlADC = 0;
//******************************************************************************
//                          Prototipos de funciones 
//******************************************************************************
void setup(void);
void chequeo (void);
void display (void);
void ADC_GO (void);
void toggle_s (void);
//******************************************************************************
//                          interrupciones 
//******************************************************************************
void __interrupt() isr(void){
    if (INTCONbits.TMR0IF == 1){
        TMR0 = 236; //valor del timer para 5ms
        toggle_s(); //funcion que alterna el valor de la variable que multiplexa
        controlADC++; //contador para dar el GO del ADC en el main loop 
        INTCONbits.TMR0IF = 0; //limpiar la bandera 
    }
    if (PIR1bits.ADIF == 1){
        ADC = ADRESH; //mover el valor de la lectura a una variable local 
        PIR1bits.ADIF = 0; //limpiar la bandera de interrupcion 
    }
    if (INTCONbits.RBIF == 1){ //rutina de interrupcion con antirrebote de ambos botones 
        if (PORTBbits.RB6 == 0){ //un boton incrementa el puertoD y otra lo decrementa 
            boton1 = 1;
        }
        if (PORTBbits.RB6 == 1 && boton1 == 1){
            boton1  = 0;
            PORTD++; 
        }
        if (PORTBbits.RB7 == 0){
            boton2 = 1;
        }
        if (PORTBbits.RB7 == 1 && boton2 == 1){
            boton2  = 0;
            PORTD--;
        }
        INTCONbits.RBIF = 0;
    }
}
//******************************************************************************
//                          Ciclo principal 
//******************************************************************************

void main(void) {
    setup(); //setup 
    //**************************************************************************
    //                             mian loop
    //**************************************************************************
    while (1) {
        display (); //rutina que multiplexa los displays 
        chequeo (); //rutina que hace la verificacion del puerto para encender el indicador 
        ADC_GO (); //rutina que revisa el contador para dar el GO cada 5 interrupcinoes del TMR0

    }
}


//******************************************************************************
//                          Setup
//******************************************************************************

void setup(void) {
    initOsc(7);//configura el osculador interno a 4Mhz
    configADC(0,2); //canal 0 y velocidad FOSC/32
    OPTION_REG = 0b01010111; //configuracion para activar las PULL - UPS del puerto B y timer 0
    ANSEL = 0;
    ANSELbits.ANS0 = 1;
    ANSELH = 0; //se borran las entradas analogicas 
    PORTC = 0; //se resetea el puerto C
    TRISC = 0; //se selecciona el puerto C como salida 
    PORTD = 0; //se resetea el puerto D
    TRISD = 0; //se selecciona el puerto D como salida 
    PORTB = 0; //se resetea el puerto B
    TRISE = 0; // se marca el puerto E como salida
    PORTE = 0;  //se resetea el puerto E
    PORTA = 0;
    TRISAbits.TRISA0 = 1; //EL A0 como entrada 
    TRISBbits.TRISB6 = 1; //RB6 como entrada 
    TRISBbits.TRISB7 = 1; //RB7 como entrada 
    WPUB = 0b11000000; //pull ups en RB7 y RB6
//****************************interrupcinoes************************************
    INTCONbits.GIE = 1; //se activan las interrupciones globales 
    INTCONbits.PEIE = 1; // se activan las interrupciones perifericas 
    INTCONbits.TMR0IE = 1; //se activan las interrupciones del timer 0
    INTCONbits.RBIE = 1; //se activan las interrupciones del puerto b
    PIE1bits.ADIE = 1; //activar las interrupciones del ADC
    IOCB = 0b11000000; //configuracion de los pines de las interrupciones PORTB
    PIR1bits.ADIF = 0; //limpiar la bandera del ADC
    INTCONbits.RBIF = 0; //limpiar la bandera del puerto B
    INTCONbits.T0IF = 0; //limpiar bandera del timer0
//******************************************************************************
}
void chequeo (void){
    if (ADC > PORTD){ //revisar so el valor del ADC es mayor al contador 
        PORTEbits.RE2 = 1; //si es mayor encender el led
    }else{
        PORTEbits.RE2 = 0; //si es menor apagar el led 
    }    
}
void display (void){
    PORTEbits.RE0 = 0;//apagar los bits para los transistores 
    PORTEbits.RE1 = 0;
    if (toggle == 1){ //muxeo de los valores del display
        PORTEbits.RE1 = 1; //encender el valor del transistor 
        display0 = ADC & 0b00001111; //colocar la variable del display el primer nibble de la variable ADC
        tabla(display0); //llamar a la tabla para mostrar el valor correspondiente
    }else{
        PORTEbits.RE0 = 1;
        display1 = (ADC & 0b11110000)>>4; //mover el segundo nibble a los primers 4 bits para buscar en la tabla
        tabla(display1);//llamar a la tabla para mostrar el valor correspondiente
    }
}
void ADC_GO (void){
    if (controlADC > 10){ //revisa si el contador es mayor a 10 para dar el GO 
        controlADC = 0;
        ADCON0bits.GO_nDONE = 1;
    }
}
void toggle_s (void) { //alterna la variable toggle entre 1 y 0
    if (toggle == 1){
        toggle = 0;
    }else{
        toggle = 1;
    }
}
