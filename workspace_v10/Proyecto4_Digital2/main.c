//******************************************* Librerias ***********************************
#include <stdint.h>
#include <stdbool.h>
#include "inc/tm4c123gh6pm.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "driverlib/gpio.h"
#include "driverlib/timer.h"
#include "driverlib/uart.h"
#include "driverlib/pin_map.h"
//*****************************************************************************************
//                                               variables
//*****************************************************************************************
uint8_t parqueo1 = 0;
uint8_t parqueo2 = 0;
uint8_t parqueo3 = 0;
uint8_t parqueo4 = 0;
//*****************************************************************************************
//                                  Prototipos de funciones
//*****************************************************************************************
void leds (uint32_t puertoLG,uint32_t puertoLR, uint32_t puerto, uint8_t boton, uint8_t ledrojo, uint8_t ledverde);
void setup (void);
//*****************************************************************************************
//                                  ciclo principal
//******************************************************************************************
int main(void){
    setup();
    while(1)
    {
     leds(GPIO_PORTF_BASE,GPIO_PORTF_BASE,GPIO_PORTB_BASE,GPIO_PIN_4,GPIO_PIN_3,GPIO_PIN_2);
     leds(GPIO_PORTB_BASE,GPIO_PORTC_BASE,GPIO_PORTA_BASE,GPIO_PIN_5,GPIO_PIN_4,GPIO_PIN_3);
     leds(GPIO_PORTC_BASE,GPIO_PORTC_BASE,GPIO_PORTA_BASE,GPIO_PIN_6,GPIO_PIN_6,GPIO_PIN_5);
     leds(GPIO_PORTC_BASE,GPIO_PORTD_BASE,GPIO_PORTA_BASE,GPIO_PIN_7,GPIO_PIN_6,GPIO_PIN_7);
    }
}
//******************************************************************************************
//                                          Funciones
//******************************************************************************************
void setup (void){

    SysCtlClockSet(SYSCTL_OSC_MAIN|SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ); //configurar el oscilador a 40MHz

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);//habilita el puerto A

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);//habilita el puerto B

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);//habilita el puerto C

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);//habilita el puerto F

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);//habilita el puerto D

    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_2|GPIO_PIN_3);

    GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, GPIO_PIN_6);

    GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE,GPIO_PIN_3);

    GPIOPinTypeGPIOOutput(GPIO_PORTC_BASE,GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7);

    GPIOPadConfigSet(GPIO_PORTB_BASE, GPIO_PIN_4, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

    GPIOPadConfigSet(GPIO_PORTA_BASE, GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
    //************************************** configuracion timer 0*****************************************

    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0); // Se habilita el reloj para el temporizador

    TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC); // Configuración del Timer 0 como temporizador períodico

    TimerLoadSet(TIMER0_BASE, TIMER_A, ((SysCtlClockGet())/200) - 1); // Establecer el periodo del temporizador con periodo de 1seg

    IntEnable(INT_TIMER0A); // Se habilita la interrupción por el TIMER0A

    TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT); // Se establece que exista la interrupción por Timeout

    TimerEnable(TIMER0_BASE, TIMER_A); // Se habilita el Timer

    //************************************** configuracion UART*****************************************

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);//habilita el puerto A

    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART1); //habilita el periferico del UART

    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_UART1)){} //esperar a que el UART este habilitado

    UARTConfigSetExpClk(UART1_BASE, SysCtlClockGet(), 115200, UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE); //configuracion UART

    GPIOPinConfigure(GPIO_PB0_U1RX);

    GPIOPinConfigure(GPIO_PB1_U1TX);

    GPIOPinTypeUART(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1);//pines 0 y 1 del PORTA para el UART

    UARTEnable(UART1_BASE);

    IntMasterEnable(); // Se habilitan las interrupciones Globales

}

void Timer0IntHandler(void){
    // Clear the timer interrupt
    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    // Read the current state of the GPIO pin and
    // write back the opposite state
    UARTCharPutNonBlocking(UART1_BASE, parqueo1+48);
    UARTCharPutNonBlocking(UART1_BASE, parqueo2+48);
    UARTCharPutNonBlocking(UART1_BASE, parqueo3+48);
    UARTCharPutNonBlocking(UART1_BASE, parqueo4+48);
    UARTCharPutNonBlocking(UART1_BASE, 10);

}
void leds (uint32_t puertoLG,uint32_t puertoLR, uint32_t puerto, uint8_t boton, uint8_t ledrojo, uint8_t ledverde){
    if(GPIOPinRead(puerto, boton) == 0){
        GPIOPinWrite(puertoLR, ledrojo, ledrojo);
        GPIOPinWrite(puertoLG, ledverde,0);
        if (boton == GPIO_PIN_4){
            parqueo1 = 1;
        }else if (boton == GPIO_PIN_5){
            parqueo2 = 1;
        }else if (boton ==GPIO_PIN_6){
            parqueo3 = 1;
        }else if (boton == GPIO_PIN_7){
            parqueo4 = 1;
        }
    }else{
        GPIOPinWrite(puertoLG, ledverde, ledverde);
        GPIOPinWrite(puertoLR, ledrojo, 0);
        if (boton == GPIO_PIN_4){
            parqueo1 = 0;
        }else if (boton == GPIO_PIN_5){
            parqueo2 = 0;
        }else if (boton ==GPIO_PIN_6){
            parqueo3 = 0;
        }else if (boton == GPIO_PIN_7){
            parqueo4 = 0;
        }
    }
}

