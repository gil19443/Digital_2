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


#define RS RE0
#define EN RE2
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
#define EN RE2
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
uint8_t controlADC = 0;
uint8_t bandera1 = 0;
uint8_t bandera2 = 0;
//******************************************************************************
//                          Prototipos de funciones 
//******************************************************************************
void setup(void);
void envio (void);
void sep_nibbles (void);
void verificacion (void);
void mapeo (void);
void setup(void);
void ADC_GO (void);
void write_data (void);
//******************************************************************************
//                          interrupciones 
//******************************************************************************
void __interrupt() isr(void){
    if (INTCONbits.TMR0IF == 1){
        TMR0 = 236; //valor del timer para 5ms
        sep_nibbles();
        controlADC++;
        INTCONbits.TMR0IF = 0; //limpiar la bandera 
    }
    
    if (PIR1bits.ADIF == 1){
        if (ADC_selector == 1){
          ADC_selector = 0;
          POT1 = ADRESH;
           //limpiar la bandera de interrupcion 
          configADC(1,2);
        }else{
          ADC_selector = 1;
          POT2 = ADRESH;
          configADC(0,2);
        }
        PIR1bits.ADIF = 0;
    } 
    if (PIR1bits.RCIF == 1){
           DATO_RECIBIDO = RCREG;
        }
    
    if (PIR1bits.TXIF == 1){
        envio();
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
          ADC_GO();
          //verificacion();
            Lcd_Clear();
            Lcd_Set_Cursor(1,1);
            Lcd_Write_String("LCD Library for");
            Lcd_Set_Cursor(2,1);
            Lcd_Write_String("MPLAB XC8");
            __delay_ms(2000);
            Lcd_Clear();
            Lcd_Set_Cursor(1,1);
            Lcd_Write_String("Developed By");
            Lcd_Set_Cursor(2,1);
            Lcd_Write_String("electroSome");
            __delay_ms(2000);
            Lcd_Clear();
            Lcd_Set_Cursor(1,1);
            Lcd_Write_String("www.electroSome.com");
            Lcd_Clear();
            Lcd_Set_Cursor(2,1);
            Lcd_Write_Char('e');
            Lcd_Write_Char('S');
            __delay_ms(2000);
    }
}


//******************************************************************************
//                          Setup
//******************************************************************************

void setup(void) {
    Lcd_Init();
    initUSART();
    initOsc(6);//configura el osculador interno a 4Mhz
    configADC(0,2); //canal 0 y velocidad FOSC/32
    OPTION_REG = 0b11010111; //configuracion para activar las PULL - UPS del puerto B y timer 0
    ANSEL = 0;
    ANSELH = 0;
    ANSELbits.ANS0 = 1; //se borran las entradas analogicas 
    ANSELbits.ANS1 = 1;
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
    INTCONbits.TMR0IE = 1; //se activan las interrupciones del timer 0
    PIE1bits.ADIE = 1; //activar las interrupciones del ADC
    PIR1bits.ADIF = 0; //limpiar la bandera del ADC
    INTCONbits.T0IF = 0; //limpiar bandera del timer0
    PIR1bits.RCIF = 0;//se limpia la bandera de recepcion para interrupcion 
    PIE1bits.TXIE = 1;
    PIR1bits.TXIF = 0;
    PIE1bits.RCIE = 1; //se activan las interrupciones del RCREG
}
void sep_nibbles (void){
    NLPOT1 = POT1 & 0b00001111;
    NHPOT1 = (POT1 & 0b11110000)>>4;
    NLPOT2 = POT2 & 0b00001111;
    NHPOT2 = (POT2 & 0b11110000)>>4;
    
}
void verificacion (void){
    if (DATO_RECIBIDO == 43){
        bandera1 = 1;      
    }
    if (DATO_RECIBIDO != 43 && bandera1 ==1){
        bandera1 = 0;
        PORTB++;
    }
    if (DATO_RECIBIDO == 45){
        bandera2 = 1;      
    }
    if (DATO_RECIBIDO != 45 && bandera2 ==1){
        bandera2 = 0;
        PORTB--;
    }

}
void mapeo (void){
    TEMP1 = ((POT1 / 5)*100);
    TEMP2 = ((POT2 / 5)*100);
    centenasp1 = TEMP1/100;
    decenasp1 = (TEMP1-(centenasp1*100))/10;
    unidadesp1 = (TEMP1-((centenasp1*100)+(decenasp1*10)));
    
    centenasp2 = TEMP2/100;
    decenasp2 = (TEMP2-(centenasp2*100))/10;
    unidadesp2 = (TEMP2-((centenasp2*100)+(decenasp2*10)));
}
void ADC_GO (void){
    if (controlADC > 10){ //revisa si el contador es mayor a 10 para dar el GO 
        controlADC = 0;
        ADCON0bits.GO_nDONE = 1;
        PIE1bits.TXIE = 1;
    }
}
void envio (void){
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
