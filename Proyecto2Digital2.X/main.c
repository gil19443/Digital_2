/*
 * File:   main.c
 * Author: Carlos Gil
 *
 * Created on 18 de enero de 2021, 02:53 PM
 */
//******************************************************************************
//                      Importacion de librerias 
//******************************************************************************
#include <pic16f887.h>
#include <xc.h>
#include <stdint.h>
#include "I2C.h"
#include "Libreria2.h"
#include "usart9600.h"
#include "TablaACSII_HEX.h"
//******************************************************************************
//                             Palabras de configuracion 
//******************************************************************************
// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT  // Oscillator Selection bits (XT oscillator: Crystal/resonator on RA6/OSC2/CLKOUT and RA7/OSC1/CLKIN)
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
#define _XTAL_FREQ 4000000
//******************************************************************************
//                          Variables 
//******************************************************************************
uint8_t controles = 0;
unsigned short segundos = 0;
unsigned short minutos = 0;
unsigned short horas = 0;
unsigned short dia_S = 0;
unsigned short dia = 0;
unsigned short mes = 0;
unsigned short year = 0;
unsigned short var_envio = 0;
//******************************************************************************
//                          Prototipos de funciones 
//******************************************************************************
void setup(void);
void TX_GO (void);
void envio (void);
//******************************************************************************
//                          interrupciones 
//******************************************************************************
void __interrupt() isr(void){
    if (INTCONbits.T0IF == 1){
        TMR0 = 236; //valor del timer para 5ms
        controles++; //variable de control para el TXIE y el SSP
        INTCONbits.TMR0IF = 0; //limpiar la bandera 
    }
    if (PIR1bits.TXIF == 1){
        envio(); //rutina que envia los datos de los esclavos por el puerto serial 
        PIE1bits.TXIE = 0;  //se limpia el enable de la interrupcion del TXREG
    }
}
//******************************************************************************
//                          Ciclo principal 
//******************************************************************************

void main(void) {
    setup();
    I2C_Master_Start(); //iniciar comunicacion 
    I2C_Master_Write(0xD0);//adress del RTC para que reciba
    I2C_Master_Write(0); //colocar pointer en 0
    I2C_Master_Write(0); //segundos en 0
    I2C_Master_Write(0); //minutos en 0
    I2C_Master_Write(0b00011000); //horas
    I2C_Master_Write(0b00000100); //dia de la semana
    I2C_Master_Write(0b00100101); // numero de dia 
    I2C_Master_Write(0b00000010); //mes
    I2C_Master_Write(0x21); //year
    I2C_Master_Stop(); //finalizar comunicacion 
    //**************************************************************************
    //                             mian loop
    //**************************************************************************
    while (1) {
        TX_GO();
        I2C_Master_Start();
        I2C_Master_Write(0xD0);
        I2C_Master_Write(0);
        I2C_Master_RepeatedStart();
        I2C_Master_Write(0xD1);
        segundos = I2C_Master_Read(0);
        minutos = I2C_Master_Read(0);
        horas = I2C_Master_Read(0);
        dia_S = I2C_Master_Read(0);
        dia = I2C_Master_Read(0);
        mes = I2C_Master_Read(0);
        year = I2C_Master_Read(1);
        I2C_Master_Stop();
    }
}


//******************************************************************************
//                          Setup
//******************************************************************************

void setup(void) {
    initUSART();//funcion de configuracion del UART
    initOsc(6);//configura el osculador interno a 4Mhz
    OPTION_REG = 0b11010111; //configuracion para activar las PULL - UPS del puerto B y timer 0
    ANSEL = 0;
    ANSELH = 0; //se limpian las entradas analogicas
    PORTC = 0; //se resetea el puerto C
    PORTD = 0; //se resetea el puerto D
    TRISD = 0; //se selecciona el puerto D como salida 
    PORTB = 0; //se resetea el puerto B
    TRISE = 0; // se marca el puerto E como salida
    PORTE = 0;  //se resetea el puerto E
    PORTA = 0; //se resetea el puerto A
    TRISA = 0; //se selecciona el puerto A como salida 
    TRISB = 0; // se selecciona el puerto B como salida 
//****************************** MSSP I2C **************************************
    I2C_Master_Init(100000);
//****************************interrupcinoes************************************
    INTCONbits.GIE = 1; //se activan las interrupciones globales 
    INTCONbits.PEIE = 1; // se activan las interrupciones perifericas 
    INTCONbits.TMR0IE = 1; //se activan las interrupciones del timer 0
    INTCONbits.T0IF = 0; //limpiar bandera del timer0
    PIE1bits.TXIE = 1; //se activa la interrupcion del TX
    PIR1bits.TXIF = 0; //se limpia la babndera del TX
}
void TX_GO (void){ //por cada 10 de control activa el TXIE
    if (controles>10){ //revisa si el contador es mayor a 10 para dar el TXEN 
        controles = 0;
        PIE1bits.TXIE = 1;
    }
}
void envio (void){
    switch (var_envio){
        case 0:
            tabla_hex(((horas & 0xF0)>>4), &TXREG);
            var_envio++;
            break;
        case 1:
            tabla_hex((horas & 0x0F), &TXREG);
            var_envio++;
            break;
        case 2:
            TXREG = 58;
            var_envio++;
            break;
        case 3:
            tabla_hex(((minutos & 0xF0)>>4), &TXREG);
            var_envio++;
            break;
        case 4:
            tabla_hex((minutos & 0x0F), &TXREG);
            var_envio++;
            break;
        case 5:
            TXREG = 58;
            var_envio++;
            break;
        case 6:
            tabla_hex(((segundos & 0xF0)>>4), &TXREG);
            var_envio++;
            break;
        case 7:
            tabla_hex((segundos & 0x0F), &TXREG);
            var_envio++;
            break;
        case 8:
            TXREG = 44;
            var_envio++;
            break;
        case 9:
            tabla_hex(((dia & 0xF0)>>4), &TXREG);
            var_envio++;
            break;
        case 10:
            tabla_hex((dia & 0x0F), &TXREG);
            var_envio++;
            break;
        case 11:
            TXREG = 47;
            var_envio++;
            break;
        case 12:
            tabla_hex(((mes & 0xF0)>>4), &TXREG);
            var_envio++;
            break;
        case 13:
            tabla_hex((mes & 0x0F), &TXREG);
            var_envio++;
            break;
        case 14:
            TXREG = 47;
            var_envio++;
            break;
        case 15:
            tabla_hex(((year & 0xF0)>>4), &TXREG);
            var_envio++;
            break;
        case 16:
            tabla_hex((year & 0x0F), &TXREG);
            var_envio++;
            break;
        case 17:
            TXREG = 10;
            var_envio = 0;
            break;
    }
}
