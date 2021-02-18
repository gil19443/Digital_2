/*
 * File:   main.c
 * Author: Carlos Gil
 *
 * Created on 17 de febrero de 2021, 02:53 PM
 */
//******************************************************************************
//                      Importacion de librerias 
//******************************************************************************
#include <xc.h>
#include <stdint.h>
#include "Libreria2.h"
#include "TablaACSII_HEX.h"
#include "configuracionADC.h"
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

//******************************************************************************
//                          Macros 
//******************************************************************************
#define _XTAL_FREQ 400000
#define RS RE0 //RS del LCD asignado al RE0
#define EN RE2 //EN del LCD asignado al RE2
//******************************************************************************
//                          Variables 
//******************************************************************************
uint8_t ADC_selector =0;
uint8_t POT1 = 0;
uint8_t POT2 = 0;
uint8_t POT3 = 0;
uint8_t POT4 = 0;
uint8_t NHPOT1 = 0;
uint8_t NLPOT1 = 0;
uint8_t NHPOT2 = 0;
uint8_t NLPOT2 = 0;
uint8_t var_envio = 0;
uint8_t RC_selector = 0;
uint8_t DATO_RECIBIDO = 0;
uint8_t ENTER = 0;
uint8_t CONT = 0;
uint8_t unidadesp1 = 0;
uint8_t decenasp1 = 0;
uint8_t centenasp1 = 0;
uint8_t unidadesp2 = 0;
uint8_t decenasp2 = 0;
uint8_t centenasp2 = 0;
uint16_t TEMP1 = 0; 
uint16_t TEMP2 = 0;
uint8_t controlADC = 0;
uint8_t bandera1 = 0;
uint8_t bandera2 = 0;
uint8_t LCD = 0;
//******************************************************************************
//                          Prototipos de funciones 
//******************************************************************************
void setup(void);
void envio (void);
void ADC_GO (void);
//******************************************************************************
//                          interrupciones 
//******************************************************************************
void __interrupt() isr(void){
    if (INTCONbits.TMR0IF == 1){
        TMR0 = 236; //valor del timer para 5ms
        controlADC++; //rutina que controla el Go del ADC y el TXIE
        INTCONbits.TMR0IF = 0; //limpiar la bandera 
    }
    
    if (PIR1bits.ADIF == 1){ //mux del ADC para leer dos puertos analogicos 
        switch (ADC_selector){
            case 0:
                ADC_selector++;
                POT1 = ADRESH;
                configADC(1,2);
                break;
            case 1:
                ADC_selector++;
                POT2 = ADRESH;
                configADC(2,2);
                break;
            case 2:
                ADC_selector++;
                POT3 = ADRESH;
                configADC(3,2);
                break;
            case 3:
                ADC_selector = 0;
                POT4 = ADRESH;
                configADC(0,2);
                break;   
        }
        PIR1bits.ADIF = 0;
    } 
    
    if (PIR1bits.TXIF == 1){
        envio(); //rutina que envia los datos del pot en ACSSII
        PIE1bits.TXIE = 0;
        
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
          ADC_GO(); //rutina que activa el GO y el TXIE
    }
}


//******************************************************************************
//                          Setup
//******************************************************************************

void setup(void) {
    initUSART();//funcion de configuracion del UART
    initOsc(6);//configura el osculador interno a 4Mhz
    configADC(0,2); //canal 0 y velocidad FOSC/32
    OPTION_REG = 0b11010111; //configuracion para activar las PULL - UPS del puerto B y timer 0
    ANSEL = 0;
    ANSELH = 0; //se limpian las entradas analogicas
    ANSELbits.ANS0 = 1; 
    ANSELbits.ANS1 = 1;//se activan las entradas del RA0 y RA1
    ANSELbits.ANS2 = 1; 
    ANSELbits.ANS3 = 1;//se activan las entradas del RA0 y RA1
    PORTC = 0; //se resetea el puerto C
    TRISC = 0x80; //se selecciona como entrada el RX
    PORTD = 0; //se resetea el puerto D
    TRISD = 0; //se selecciona el puerto D como salida 
    PORTB = 0; //se resetea el puerto B
    TRISE = 0; // se marca el puerto E como salida
    PORTE = 0;  //se resetea el puerto E
    PORTA = 0; //se resetea el puerto A
    TRISA = 0; //se selecciona el puerto A como salida 
    TRISB = 0; // se selecciona el puerto B como salida 
    TRISAbits.TRISA0 = 1; //EL A0 como entrada 
    TRISAbits.TRISA1 = 1; //El A1 como entrada 
    TRISAbits.TRISA2 = 1; //EL A0 como entrada 
    TRISAbits.TRISA3 = 1; //El A1 como entrada 
//****************************interrupcinoes************************************
    INTCONbits.GIE = 1; //se activan las interrupciones globales 
    INTCONbits.PEIE = 1; // se activan las interrupciones perifericas 
    INTCONbits.TMR0IE = 1; //se activan las interrupciones del timer 0
    PIE1bits.ADIE = 1; //activar las interrupciones del ADC
    PIR1bits.ADIF = 0; //limpiar la bandera del ADC
    INTCONbits.T0IF = 0; //limpiar bandera del timer0
    PIE1bits.TXIE = 1; //se activa la interrupcion del TX
    PIR1bits.TXIF = 0; //se limpia la babndera del TX
}

void ADC_GO (void){ //por cada 10 de control activa el GO y el TXIE
    if (controlADC > 10){ //revisa si el contador es mayor a 10 para dar el GO 
        controlADC = 0;
        ADCON0bits.GO_nDONE = 1;
        PIE1bits.TXIE = 1;
    }
}
void envio (void){ //rutina que envia datos como "POT1 , POT2 /n"
    switch (var_envio){
        case 0:
           tabla_hex(((POT1 & 0b11110000)>>4),&TXREG);
           var_envio++;
           break;
        case 1:
           tabla_hex((POT1 & 0b00001111),&TXREG);
           var_envio++;
           break;
        case 2:
           TXREG = 44; 
           var_envio++;
           break;
        case 3:
           tabla_hex(((POT2 & 0b11110000)>>4),&TXREG);
           var_envio++;
           break;
        case 4:
           tabla_hex((POT2 & 0b00001111),&TXREG);
           var_envio++;
           break;
        case 5:
           TXREG = 44; 
           var_envio++;
           break;
        case 6:
           tabla_hex(((POT3 & 0b11110000)>>4),&TXREG);
           var_envio++;
           break;
        case 7:
           tabla_hex((POT3 & 0b00001111),&TXREG);
           var_envio++;
           break; 
        case 8:
           TXREG = 44; 
           var_envio++;
           break;
        case 9:
           tabla_hex(((POT4 & 0b11110000)>>4),&TXREG);
           var_envio++;
           break;
        case 10:
           tabla_hex((POT4 & 0b00001111),&TXREG);
           var_envio++;
           break; 
        case 11:
           TXREG = 10;
           var_envio = 0;
           break;
           
    }
}
