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
//*****************************************************************************************
//                                               variables
//*****************************************************************************************

//*****************************************************************************************
//                                  Prototipos de funciones
//*****************************************************************************************
void setup (void);
//*****************************************************************************************
//                                  ciclo principal
//******************************************************************************************
int main(void){
    setup();
    while(1)
    {

    }
}
//******************************************************************************************
//                                          Funciones
//******************************************************************************************
void setup (void){

    SysCtlClockSet(SYSCTL_OSC_MAIN|SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ); //configurar el oscilador a 40MHz

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);//habilita el puerto F

    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);//selecciona como salidas los pines 1,2 y 3 del puerto F

    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0); // Se habilita el reloj para el temporizador

    TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC); // Configuración del Timer 0 como temporizador períodico

    TimerLoadSet(TIMER0_BASE, TIMER_A, (2*(SysCtlClockGet())) - 1); // Establecer el periodo del temporizador con periodo de 1seg

    IntEnable(INT_TIMER0A); // Se habilita la interrupción por el TIMER0A

    TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT); // Se establece que exista la interrupción por Timeout

    IntMasterEnable(); // Se habilitan las interrupciones Globales

    TimerEnable(TIMER0_BASE, TIMER_A); // Se habilita el Timer
}
void Timer0IntHandler(void){
    // Clear the timer interrupt
    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    // Read the current state of the GPIO pin and
    // write back the opposite state
    if (GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_2)){
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, 0);
    }else{
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 4);
    }
}
