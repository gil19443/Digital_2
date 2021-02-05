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
#include "Display8bits.h"
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
#define _XTAL_FREQ 400000
//******************************************************************************
//                          Macros 
//******************************************************************************
#define EN RE2
#define RW RE1
#define RS RE0
//******************************************************************************
//                          Variables 
//******************************************************************************
uint8_t ADC_selector =0;
uint8_t POT1 = 0;
uint8_t POT2 = 0;
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
//******************************************************************************
//                          Prototipos de funciones 
//******************************************************************************
void setup(void);
void mux_ADC (void);
void envio (uint8_t var_envio);
void sep_nibbles (uint8_t *var1, uint8_t *var2);
void verificacion (void);
void mapeo (uint8_t *val1, uint8_t *val2);
//******************************************************************************
//                          interrupciones 
//******************************************************************************
void __interrupt() isr(void){
    if (INTCONbits.TMR0IF == 1){
        TMR0 = 236; //valor del timer para 5ms
        sep_nibbles(&POT1, &POT2);
        envio(var_envio);
        INTCONbits.TMR0IF = 0; //limpiar la bandera 
    }
    if (PIR1bits.ADIF == 1){
        mux_ADC(); 
    }
    if (PIR1bits.RCIF == 1){
        if (RC_selector == 1){
            DATO_RECIBIDO = RCREG;
            RC_selector = 0;
        }else{
            RC_selector = 1;
            ENTER = RCREG;
        }
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
        verificacion();
        Lcd_Clear();
        Lcd_Set_Cursor(1,1);
        Lcd_Write_String("Prueba pantalla");
        Lcd_Set_Cursor(2,1);
        Lcd_Write_String("Prueba pantalla");
        __delay_ms(2000);
    }
}


//******************************************************************************
//                          Setup
//******************************************************************************

void setup(void) {
    initUSART();
    initOsc(7);//configura el osculador interno a 4Mhz
    configADC(10,2); //canal 0 y velocidad FOSC/32
    OPTION_REG = 0b11010111; //configuracion para activar las PULL - UPS del puerto B y timer 0
    ANSEL = 0;
    ANSELHbits.ANS10 = 1; //se borran las entradas analogicas 
    ANSELHbits.ANS12 = 1;
    PORTC = 0; //se resetea el puerto C
    TRISC = 0; //se selecciona el puerto C como salida 
    PORTD = 0; //se resetea el puerto D
    TRISD = 0; //se selecciona el puerto D como salida 
    PORTB = 0; //se resetea el puerto B
    TRISE = 0; // se marca el puerto E como salida
    PORTE = 0;  //se resetea el puerto E
    PORTA = 0;
    TRISBbits.TRISB0 = 1; //EL A0 como entrada 
    TRISBbits.TRISB1 = 1; //RB6 como entrada 
//****************************interrupcinoes************************************
    INTCONbits.GIE = 1; //se activan las interrupciones globales 
    INTCONbits.PEIE = 1; // se activan las interrupciones perifericas 
    INTCONbits.TMR0IE = 1; //se activan las interrupciones del timer 0
    PIE1bits.ADIE = 1; //activar las interrupciones del ADC
    PIR1bits.ADIF = 0; //limpiar la bandera del ADC
    INTCONbits.T0IF = 0; //limpiar bandera del timer0
    PIR1bits.RCIF = 0;//se limpia la bandera de recepcion para interrupcion 
    PIE1bits.RCIE = 0; //se activan las interrupciones del RCREG
}
void mux_ADC (void){
    if (ADC_selector == 1){
      ADC_selector = 1;
      POT1 = ADRESH;
      PIR1bits.ADIF = 0; //limpiar la bandera de interrupcion 
      configADC(12,2);
      ADCON0bits.GO_nDONE = 1;
    }else{
      ADC_selector = 1;
      POT2 = ADRESH;
      PIR1bits.ADIF = 0; //limpiar la bandera de interrupcion 
      configADC(10,2);
      ADCON0bits.GO_nDONE = 1;
    }
}
void envio (uint8_t var_envio){
    switch (var_envio){
        case 0:
           tabla_hex(NHPOT1,&TXREG);
           var_envio++;
           break;
        case 1:
           tabla_hex(NLPOT1,&TXREG);
           var_envio++;
           break;
        case 2:
           TXREG = 44; 
           var_envio++;
           break;
        case 3:
           tabla_hex(NHPOT2,&TXREG); 
           var_envio++;
           break;
        case 4:
           tabla_hex(NLPOT2,&TXREG); 
           var_envio++;
           break;
        case 5:
           TXREG = 10;
           var_envio = 0;
           break;
           
    }
}
void sep_nibbles (uint8_t *var1, uint8_t *var2){
    NLPOT1 = *var1 & 0b00001111;
    NHPOT1 = (*var1 & 0b11110000)>>4;
    NLPOT2 = *var2 & 0b00001111;
    NHPOT2 = (*var2 & 0b11110000)>>4;
    
}
void verificacion (void){
    if (ENTER == 10 & DATO_RECIBIDO == 43){
        CONT++;
    }
    else if (ENTER == 10 & DATO_RECIBIDO == 45){
        CONT--;
    }else{
        ENTER = 0;
        DATO_RECIBIDO = 0;
    }
}
void mapeo (uint8_t *val1, uint8_t *val2){
    TEMP1 = ((*val1 / 5)*100);
    TEMP2 = ((*val2 / 5)*100);
    centenasp1 = TEMP1/100;
    decenasp1 = (TEMP1-(centenasp1*100))/10;
    unidadesp1 = (TEMP1-((centenasp1*100)+(decenasp1*10)));
    
    centenasp2 = TEMP2/100;
    decenasp2 = (TEMP2-(centenasp2*100))/10;
    unidadesp2 = (TEMP2-((centenasp2*100)+(decenasp2*10)));
}