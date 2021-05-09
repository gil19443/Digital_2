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
//*****************************************************************************************
//                                               variables
//*****************************************************************************************
uint8_t var = 5;
uint8_t var1 = 0;
char rec = 0;
char rec1 = 0;
//*****************************************************************************************
//                                  Prototipos de funciones
//*****************************************************************************************
void setup (void);
void toggle (uint32_t port, uint8_t pins);
//*****************************************************************************************
//                                  ciclo principal
//******************************************************************************************
int main(void){
    setup();
    while(1)
    {
       if (rec == rec1){
           var = 5;
           GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, 8);
           rec1  = rec;
        }else{
            if ((rec == 'r') || (rec1 == 'r')) {
               var = 0;
               rec1  = rec;
               //GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, 0);
           }else if((rec == 'g') || (rec1 == 'g')){
               var = 2;
               rec1  = rec;
               //GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, 0);
           }else if ((rec == 'b') || (rec1 == 'b')){
               var = 1;
               rec1  = rec;
               //GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, 0);
          }
        }
    }
}
//******************************************************************************************
//                                          Funciones
//******************************************************************************************
void setup (void){

    SysCtlClockSet(SYSCTL_OSC_MAIN|SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ); //configurar el oscilador a 40MHz

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);//habilita el puerto F

    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);//selecciona como salidas los pines 1,2 y 3 del puerto F

    //************************************** configuracion timer 0*****************************************

    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0); // Se habilita el reloj para el temporizador

    TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC); // Configuración del Timer 0 como temporizador períodico

    TimerLoadSet(TIMER0_BASE, TIMER_A, (2*(SysCtlClockGet())) - 1); // Establecer el periodo del temporizador con periodo de 1seg

    IntEnable(INT_TIMER0A); // Se habilita la interrupción por el TIMER0A

    TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT); // Se establece que exista la interrupción por Timeout

    TimerEnable(TIMER0_BASE, TIMER_A); // Se habilita el Timer

    //*********************************** configuracion UART ************************************************

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);//habilita el puerto A

    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0); //habilita el periferico del UART

    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_UART0)){} //esperar a que el UART este habilitado

    UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 115200, (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE)); //configuracion UART

    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);//pines 0 y 1 del PORTA para el UART

    IntEnable(INT_UART0); //se habilita la interrupcion para el UART0

    UARTIntEnable(UART0_BASE, UART_INT_RX | UART_INT_RT); //habilitar interrupcion del RX y TX

    UARTEnable(UART0_BASE);

    //*******************************************************************************************************

    IntMasterEnable(); // Se habilitan las interrupciones Globales

}
void UARTIntHandler (void){
    UARTIntClear(UART0_BASE, UART_INT_RX | UART_INT_RT);
    if (UARTCharsAvail(UART0_BASE)){
        rec =  UARTCharGetNonBlocking(UART0_BASE);
      /*  if (rec == UARTCharGetNonBlocking(UART0_BASE)){
            rec1 = 0;
        }else{
            rec =  UARTCharGetNonBlocking(UART0_BASE);
            rec1 =1;
        }*/
    }
}
void Timer0IntHandler(void){
    // Clear the timer interrupt
    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    // Read the current state of the GPIO pin and
    // write back the opposite state
    switch(var){
        case 0: //LED rojo
            toggle(GPIO_PORTF_BASE, GPIO_PIN_1);
            break;
        case 1://LED azul
            toggle(GPIO_PORTF_BASE, GPIO_PIN_2);
            break;
        case 2://LED verde
            toggle(GPIO_PORTF_BASE, GPIO_PIN_3);
            break;
    }
}
void toggle (uint32_t port, uint8_t pins){
    switch(GPIOPinRead(port, pins)){
        case 0:
            GPIOPinWrite(port, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, pins);
            break;
        default:
            GPIOPinWrite(port, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, 0);
            break;
    }
}
