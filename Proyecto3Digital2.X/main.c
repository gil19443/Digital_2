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
#include "SPI.h"
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
uint16_t gradospos = 0;
uint8_t nueva = 0;
//******************************************************************************
//                          Prototipos de funciones 
//******************************************************************************
void setup(void);
void TX_GO (void);
void envio (void);
void mapeo (void);
void envio_esclavos(void);
void temp_pos (void);
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
    Lcd_Clear();
    __delay_ms(10);
    Lcd_Set_Cursor(1,1);
    Lcd_Write_String("POT1 LM35 CONT"); //labels del LCD  
    //**************************************************************************
    //                             mian loop
    //**************************************************************************
    while (1) {
        temp_pos(); //rutina que mapea los valores recibidos del esclavo 2 (LM35)
        TX_GO();//rutina que activa el enable dle TXREG cada 50 ms
        envio_esclavos(); //rutina que establece la comunicacion entre el master y los esclavos 
        mapeo();//rutina que mapea los valores del esclavo 1 (potencometro )
        Lcd_Set_Cursor(2,1); //caracteres del esclavo 1 para la LCD en decimal 
        Lcd_Write_Char(centenasp1+48);
        Lcd_Set_Cursor(2,2);
        Lcd_Write_Char(46);
        Lcd_Set_Cursor(2,3);
        Lcd_Write_Char(decenasp1+48);
        Lcd_Set_Cursor(2,4);
        Lcd_Write_Char(unidadesp1+48);

        if (esclavo3< 69){ //chequeo del valor del esclavo 3 para mostrar grados 
            PORTD = 3; //positivos o negativos 
            Lcd_Set_Cursor(2,5); //valores negativos  
            Lcd_Write_Char(32);//del esclavo 3 de tempratura en decimal 
            Lcd_Set_Cursor(2,6);
            Lcd_Write_Char(45);
            Lcd_Set_Cursor(2,7);
            Lcd_Write_Char(32);
            Lcd_Set_Cursor(2,8);
            Lcd_Write_Char(decenasp2+48);
            Lcd_Set_Cursor(2,9);
            Lcd_Write_Char(unidadesp2+48);
        }else{
            PORTD = 5;
            Lcd_Set_Cursor(2,5);//valores positivos  
            Lcd_Write_Char(32);//del esclavo 3 de tempratura en decimal 
            Lcd_Set_Cursor(2,6);
            Lcd_Write_Char(32);
            Lcd_Set_Cursor(2,7);
            Lcd_Write_Char(centenasp2+48);
            Lcd_Set_Cursor(2,8);
            Lcd_Write_Char(decenasp2+48);
            Lcd_Set_Cursor(2,9);
            Lcd_Write_Char(unidadesp2+48);
        }
        Lcd_Set_Cursor(2,10); //valores del esclavo 2 en decimal (contador)
        Lcd_Write_Char(32);
        Lcd_Set_Cursor(2,11);
        Lcd_Write_Char((esclavo2/100)+48);
        Lcd_Set_Cursor(2,12);
        Lcd_Write_Char(((esclavo2-((esclavo2/100)*100))/10)+48);
        Lcd_Set_Cursor(2,13);
        Lcd_Write_Char((esclavo2-((((esclavo2-((esclavo2/100)*100))/10)*10)+((esclavo2/100)*100)))+48);
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
    PORTCbits.RC0 = 1; //se inicializan en 1 los pines de comunicacion con los 
    PORTCbits.RC1 = 1; //esclavos 
    PORTCbits.RC2 = 1;
    Lcd_Init(); //rutina que inicializa el LCD
//****************************** SSP *******************************************
    TRISCbits.TRISC4 = 1; //se coloca como entrada el SDI para recibir informacion de los esclavos 
    TRISCbits.TRISC3 = 0; //se seleccinoan como salidas el SD0, los slave select y el CLK
    TRISCbits.TRISC0 = 0;
    TRISCbits.TRISC1 = 0;
    TRISCbits.TRISC2 = 0;
    //rutina de configuracion del SSP en modo master 
    spiInit(SPI_MASTER_OSC_DIV4, SPI_DATA_SAMPLE_MIDDLE, SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE);
//****************************interrupcinoes************************************
    INTCONbits.GIE = 1; //se activan las interrupciones globales 
    INTCONbits.PEIE = 1; // se activan las interrupciones perifericas 
    INTCONbits.TMR0IE = 1; //se activan las interrupciones del timer 0
    INTCONbits.T0IF = 0; //limpiar bandera del timer0
    PIE1bits.TXIE = 1; //se activa la interrupcion del TX
    PIR1bits.TXIF = 0; //se limpia la babndera del TX
}
void TX_GO (void){ //por cada 10 de control activa el TXIE
    if (controles == 10){ //revisa si el contador es mayor a 10 para dar el TXEN 
        PIE1bits.TXIE = 1;
    }
}
void envio_esclavos(void){
    if (controles > 50){ //condicional que hace que la comunicacion se ejecute 
        controles = 0; //cada 250ms para evitar saturacion en el SSP
        PORTCbits.RC2 = 1; //se colocan en 1 los slaves select para que en los cases 
        PORTCbits.RC1 = 1; //se establezca la comunicacino al colcoarlos en 0
        PORTCbits.RC0 = 1;
        switch(slave){//switch case para muxear la comunicacion con los esclavos 
            case 0:
                PORTCbits.RC0 = 0; //se establece comunicacion con un eslavo 
                SSPBUF = 0; //se le envia un dato al esclavo 
                esclavo1 = spiRead();//se revisa que no haya overflow y se lee lo que el esclavo envia 
                slave++; //se incrementa para que al reingresar entre el siguiente case
                break; //sucede lo mismo para los otros 2
            case 1:
                PORTCbits.RC1 = 0;
                SSPBUF = 0;
                esclavo2 = spiRead();
                slave++;
                break;
            case 2:
                PORTCbits.RC2 = 0;
                SSPBUF = 0;
                esclavo3 = spiRead();
                slave = 0;
                break;     
        }
      }
   }
void envio (void){ //rutina que envia datos mapeados, recibidos de los esclavos 
    switch (var_envio){
        case 0:
           TXREG = centenasp1+48;//se envian las centenas del esclavo1
           var_envio++;
           break;
        case 1:            
           TXREG = 46; //se envia un punto 
           var_envio++;
           break;
        case 2:
           TXREG = decenasp1+48; //se envian las decenas del esclavo1
           var_envio++;
           break;
        case 3:
           TXREG = unidadesp1+48; //se envian las unidades del esclavo1
           var_envio++;
           break;
        case 4:
           TXREG = 44; //se envia una coma 
           var_envio++;
           break;
        case 5: //se revisa si el valor de el esclavo 3 son grados neativos o positivos 
            var_envio++;
            if (esclavo3< 69){ //si son negativos se envia un signo menos 
                TXREG = 45;    
            }else{
                TXREG = 32; //si son positivos se envia un espacio     
            }
           break;
        case 6://se envian las centenas del esclavo 3 (termometro )
           TXREG = centenasp2+48; 
           var_envio++;
           break;
        case 7://se envian las decenas del esclavo 3 (termometro )
            TXREG = decenasp2+48; 
            var_envio++;
           break;
        case 8://se envian las unidades  del esclavo 3 (termometro )
           TXREG = unidadesp2+48; 
           var_envio++;
           break;
        case 9:
           TXREG = 44; //se envia una coma 
           var_envio++;
           break;
        case 10: //se envian las centenas del valor del esclavo 2 (contador)
           TXREG = ((esclavo2/100)+48); 
           var_envio++;
           break;
        case 11://se envian las decenas del valor del esclavo 2 (contador)
           TXREG = ((esclavo2-((esclavo2/100)*100))/10)+48; 
           var_envio++;
           break;
        case 12://se envian las decenas del valor del esclavo 2 (contador)
           TXREG = ((esclavo2-((esclavo2/100)+((esclavo2-((esclavo2/100)*100))/10)))+48); 
           var_envio++;
           break;
        case 13: //se envia un enter 
           TXREG = 13;
           var_envio = 0;
           break;    
    }
}
void mapeo (void){ //mapea los valores para que pasen de ir de 0 a 255
    uint16_t TEMP1 = 0; //para que vayan de 0 a 5 con dos decimales 
    TEMP1 = ((esclavo1 *100)/51); 
    centenasp1 = TEMP1/100; //se separan las centenas, decenas y unidades
    decenasp1 = (TEMP1-(centenasp1*100))/10;
    unidadesp1 = (TEMP1-((centenasp1*100)+(decenasp1*10)));  
}
void temp_pos (void){ //se mapean los valores para que pasen de ir de 0 a 255
    gradospos = esclavo3; //para que vayan de -55 a 150
    if (esclavo3 <69){ //se revisa si son grados negativos, para volverlos positivos 
        nueva = (-1)*((gradospos*0.807)-55.75); //y no tener inconvenientes 
        centenasp2 = nueva/100; //se separan las centenas, decenas y unidades
        decenasp2 = ((nueva-(centenasp2*100)))/10;
        unidadesp2 = nueva-((centenasp2*100)+(decenasp2*10));
    }else{
        nueva = ((gradospos*0.807)-55.75);
        PORTB = nueva;//se separan las centenas, decenas y unidades
        centenasp2 = (nueva/100);
        decenasp2 = (((nueva-(centenasp2*100)))/10);
        unidadesp2 = (nueva-((centenasp2*100)+(decenasp2*10)));
        
    }
}

            