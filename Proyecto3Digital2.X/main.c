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
#include "Display8bits.h"
#include "initSPI.h"
#include "usart9600.h"
#include "Libreria2.h"
#include "TablaACSII_HEX.h"
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
#define RS RE0 //RS del LCD asignado al RE0
#define EN RE2 //EN del LCD asignado al RE2
//******************************************************************************
//                          Variables 
//******************************************************************************
uint8_t controles = 0;
uint8_t esclavo1 = 0;
uint8_t esclavo2 = 0;
uint8_t esclavo3 = 0;
uint8_t slave = 0;
uint8_t var_envio = 0;
uint8_t unidadesp1 = 0;
uint8_t decenasp1 = 0;
uint8_t centenasp1 = 0;
uint8_t unidadesp2 = 0;
uint8_t decenasp2 = 0;
uint8_t centenasp2 = 0;
//******************************************************************************
//                          Prototipos de funciones 
//******************************************************************************
void setup(void);
void TX_GO (void);
void envio (void);
void mapeo (void);
void envio_esclavos(void);
//******************************************************************************
//                          interrupciones 
//******************************************************************************
void __interrupt() isr(void){
    if (INTCONbits.T0IF = 0){
        TMR0 = 236; //valor del timer para 5ms
        controles++; //rutina que controla el Go del ADC y el TXIE
        INTCONbits.TMR0IF = 0; //limpiar la bandera 
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
    Lcd_Clear();
    Lcd_Set_Cursor(1,1);
    Lcd_Write_String("POT1 CONT LM35"); //labels del LCD  
    //**************************************************************************
    //                             mian loop
    //**************************************************************************
    while (1) {
        TX_GO();
        envio_esclavos();
        mapeo();
        Lcd_Set_Cursor(2,1);
        Lcd_Write_Char(centenasp1+48);
        Lcd_Set_Cursor(2,2);
        Lcd_Write_Char(46);
        Lcd_Set_Cursor(2,3);
        Lcd_Write_Char(decenasp1+48);
        Lcd_Set_Cursor(2,4);
        Lcd_Write_Char(unidadesp1+48);
        Lcd_Set_Cursor(2,5);
        Lcd_Write_Char(32);
        Lcd_Set_Cursor(2,6);
        Lcd_Write_Char(centenasp2+48);
        Lcd_Set_Cursor(2,7);
        Lcd_Write_Char(46);
        Lcd_Set_Cursor(2,8);
        Lcd_Write_Char(decenasp2+48);
        Lcd_Set_Cursor(2,9);
        Lcd_Write_Char(unidadesp2+48);
        Lcd_Set_Cursor(2,10);
        Lcd_Write_Char(32);
        Lcd_Set_Cursor(2,11);
        Lcd_Write_Char((esclavo2/100)+48);
        Lcd_Set_Cursor(2,12);
        Lcd_Write_Char(((esclavo2-((esclavo2/100)*100))/10)+48);
        Lcd_Set_Cursor(2,13);
        Lcd_Write_Char((esclavo2-(((esclavo2-((esclavo2/100)*100))/10)))+48);
    }
}
//******************************************************************************
//                          Setup
//******************************************************************************

void setup(void) {
    initSPIMASTER(2);
    initUSART();//funcion de configuracion del UART
    initOsc(6);//configura el osculador interno a 4Mhz
    OPTION_REG = 0b11010111; //configuracion para activar las PULL - UPS del puerto B y timer 0
    ANSEL = 0;
    ANSELH = 0; //se limpian las entradas analogicas
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
    PORTCbits.RC0 = 1;
    PORTCbits.RC1 = 1;
    PORTCbits.RC2 = 1;
    Lcd_Init(); //rutina que inicializa el LCD
//****************************** SSP *******************************************
    TRISCbits.TRISC4 = 1;
    TRISCbits.TRISC3 = 0;
    TRISCbits.TRISC0 = 0;
    TRISCbits.TRISC1 = 0;
    TRISCbits.TRISC2 = 0;
//****************************interrupcinoes************************************
    INTCONbits.GIE = 1; //se activan las interrupciones globales 
    INTCONbits.PEIE = 1; // se activan las interrupciones perifericas 
    INTCONbits.TMR0IE = 1; //se activan las interrupciones del timer 0
    INTCONbits.T0IF = 0; //limpiar bandera del timer0
    PIE1bits.TXIE = 1; //se activa la interrupcion del TX
    PIR1bits.TXIF = 0; //se limpia la babndera del TX
    PIE1bits.SSPIE = 1;
    PIR1bits.SSPIF = 0;
}
void TX_GO (void){ //por cada 10 de control activa el TXIE
    if (controles == 10){ //revisa si el contador es mayor a 10 para dar el TXEN 
        PIE1bits.TXIE = 1;
    }
}
void envio_esclavos(void){
    if (controles > 50){
        controles = 0;
        PORTCbits.RC0 = 1;
        PORTCbits.RC1 = 1;
        PORTCbits.RC2 = 1;
            switch(slave){
                case 0:
                    PORTCbits.RC0 = 0;
                    while ( !SSPSTATbits.BF );
                    esclavo1 = SSPBUF;
                    slave++;
                    break;
                case 1:
                    PORTCbits.RC1 = 0;
                    while ( !SSPSTATbits.BF );
                    esclavo2 = SSPBUF;
                    slave++;
                    break;
                case 2:
                    PORTCbits.RC2 = 1;
                    while ( !SSPSTATbits.BF );
                    esclavo3 = SSPBUF;
                    slave = 0;
                    break;     
            }
        }
    }
void envio (void){ //rutina que envia datos como "POT1 , POT2 /n"
    switch (var_envio){
        case 0:
           tabla_hex(((esclavo1 & 0b11110000)>>4),&TXREG);
           var_envio++;
           break;
        case 1:
           tabla_hex((esclavo1 & 0b00001111),&TXREG);
           var_envio++;
           break;
        case 2:
           TXREG = 44; 
           var_envio++;
           break;
        case 3:
           tabla_hex(((esclavo2 & 0b11110000)>>4),&TXREG); 
           var_envio++;
           break;
        case 4:
           tabla_hex((esclavo2 & 0b00001111),&TXREG); 
           var_envio++;
           break;
        case 5:
           TXREG = 44; 
           var_envio++;
           break;
        case 6:
           tabla_hex(((esclavo3 & 0b11110000)>>4),&TXREG); 
           var_envio++;
           break;
        case 7:
           tabla_hex((esclavo3 & 0b00001111),&TXREG); 
           var_envio++;
           break;
        case 8:
           TXREG = 11;
           var_envio = 0;
           break;    
    }
}
void mapeo (void){ //mapea los valores para que pasen de ir de 0 a 255
    uint16_t TEMP1 = 0;
    uint16_t TEMP2 = 0;
    TEMP1 = ((esclavo1 *100)/51); // para estar de 0 a 5 con 2 decimales 
    TEMP2 = ((esclavo3 *10)/17);
    
    centenasp1 = TEMP1/100;
    decenasp1 = (TEMP1-(centenasp1*100))/10;
    unidadesp1 = (TEMP1-((centenasp1*100)+(decenasp1*10)));  
    
    centenasp2 = TEMP2/100;
    decenasp2 = (TEMP2-(centenasp2*100))/10;
    unidadesp2 = (TEMP2-((centenasp2*100)+(decenasp2*10)));
}

