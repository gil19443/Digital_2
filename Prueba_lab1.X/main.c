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
unsigned char check = 0 ; //variable para que se active el uso de los bottones 
unsigned char conteo1 = 0;//bandera del boton del jugador 1
unsigned char conteo2 = 0; //bandera del boton del jugador 2
unsigned char jugadores1 = 0; //selector de los cases que se incrementa con el boton1
unsigned char jugadores2 = 0; //selector de los cases que se incrementa con el boton2
//******************************************************************************
//                          Prototipos de funciones 
//******************************************************************************
void setup(void); //funcion del setup 
void luces (void); //funcion que enciende y apaga los leds del semadforo  
void habilitar_semaforo (void); //funcion que activa los contadores y enciende las luces 
void juego1(unsigned char selector1); // funcion que tiene los cases para incrementar el contador del jugador 1
void juego2(unsigned char selector2); // funcion que tiene los cases para incrementar el contador del jugador 2
void jugador1(void); //rutina de antirebotes del botton 1 que incrementa y llama la funcion de los cases 
void jugador2(void); //rutina de antirebotes del botton 2 que incrementa y llama la funcion de los cases 
//******************************************************************************
//                          Ciclo principal 
//******************************************************************************

void main(void) {
    setup(); //llama la rutina de configuracion 
    //**************************************************************************
    //                             mian loop
    //**************************************************************************
    while (1) {
        habilitar_semaforo(); //se  llama la funcion para habilitar jugadores 
        if (check == 1) { //condicion para poder empezar el juego 
            jugador1();
            jugador2();  //funciones con los antirebotes y cases de ambos jugadores 
        }
    }
}


//******************************************************************************
//                          Setup
//******************************************************************************

void setup(void) {
    ANSEL = 0;
    ANSELH = 0; //se borran las entradas analogicas 
    PORTC = 0; //se resetea el puerto C
    TRISC = 0; //se selecciona el puerto C como salida 
    PORTD = 0; //se resetea el puerto D
    TRISD = 0; //se selecciona el puerto D como salida 
    PORTB = 0; //se resetea el puerto B
    TRISBbits.TRISB0 = 1;
    TRISBbits.TRISB1 = 1;
    TRISBbits.TRISB2 = 1; //se seleccinoan los bits para los botones 
    TRISBbits.TRISB3 = 0;
    TRISBbits.TRISB4 = 0;// se marcan como salidas los pines indicadores de ganador 
    TRISE = 0; // se marca el puerto E como salida
    PORTE = 0;  //se resetea el puerto E
    WPUB = 0b00000111; //0000 0111 
    OPTION_REG = 0b01010111; //configuracion para activar las PULL - UPS del puerto B
}

//******************************************************************************
//                          SUBRUTIANS
//******************************************************************************
void luces (void) {  //se encuenden y apagan las luces con delays 
    PORTE = 1; 
    __delay_ms(500);
    PORTE = 2;
    __delay_ms(500);
    PORTE = 4;
    __delay_ms(500);
    PORTE = 0;
}
void habilitar_semaforo (void){ // se revisa el boton de inicio y se habilita para 
    if (PORTBbits.RB0 == 0){  //empezar el juego 
            luces();
            check = 1;
            PORTD = 0;
            PORTC = 0;
            conteo1 = 0;
            conteo2 = 0;
            jugadores1 = 0;
            jugadores2 = 0;
            PORTBbits.RB3 = 0;
            PORTBbits.RB4 = 0;
        }
}
void jugador1 (void){
    if (PORTBbits.RB1 == 0){ //se revisan si se presiono el boton 
        conteo1 = 1; //si el boton se presiono se activa la bander a
    }
    if (PORTBbits.RB1 == 1 && conteo1 == 1){ //si ya se solto el boton y la bandera estuva en 1
        conteo1 = 0; // se impia la bander a
        jugadores1++; // se incrementa el selector de la funcion de los cases 
        juego1(jugadores1); // se llama la funcion de los cases 
    }
}
//funciona exactamenre igual que la rutina de arriba 
void jugador2 (void ){
    if (PORTBbits.RB2 == 0){
        conteo2 = 1;
    }
    if (PORTBbits.RB2 == 1 && conteo2 == 1){
        conteo2 = 0;
        jugadores2++;
        juego2(jugadores2);
    }
}
void juego1(unsigned char selector1){  //rutina con cases 
    switch (selector1){
        case 1: //luego del primer click se incrementa el puerto 
            PORTC++;
            break;
        case 8: //en el case del ultimo bit se enciende el bit y se marca si gano 
            PORTC = 128; // se resetea las demas banderas para cuando vuelva a empezar 
            PORTBbits.RB3 = 1;
            check = 0;
            jugadores1 = 0;
            break;
        default: //se le da shift al bit del caso 1 hasta que sucede el caso 8
            PORTC = PORTC << 1;
            break;
    }
}
//funciona exacramente igual que la rutina de arriba 
void juego2(unsigned char selector2){
    switch (selector2){
        case 1:
            PORTD++;
            break;
        case 8:
            PORTD = 128;
            PORTBbits.RB4 = 1;
            check = 0;
            jugadores2 = 0;
            break;
        default:
            PORTD = PORTD << 1;
            break;  
    }
}