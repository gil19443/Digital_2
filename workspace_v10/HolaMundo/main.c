//******************************************* Librerias ***********************************
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
//*****************************************************************************************
//                                               variables
//*****************************************************************************************
    uint8_t LED = 0;
    uint8_t var = 0;
    uint8_t boton = 0;
    uint8_t enable = 0;
//*****************************************************************************************
//                                  Prototipos de funciones
//*****************************************************************************************
    void setup (void);
    void semaforo (void);
    void debounce (void);
//*****************************************************************************************
//                                  ciclo principal
//******************************************************************************************
int main(void){
    setup();
    while(1)
    {
        if (enable == 1){
            semaforo();
        }
        debounce();
    }
}
//******************************************************************************************
//                                          Funciones
//******************************************************************************************
void setup (void){

    SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);

    GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_4, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
}
void semaforo (void){
    switch (var){
        case 0:
            LED = 8;
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, LED);
            SysCtlDelay(5000000);
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0);
            SysCtlDelay(5000000);
            var++;
            break;
        case 4:
            LED = 10;
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, LED);
            SysCtlDelay(5000000);
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0);
            SysCtlDelay(5000000);
            var++;
            break;
        case 5:
            LED = 2;
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, LED);
            SysCtlDelay(5000000);
            enable = 0;
            var = 0;
            break;
        default:
            LED = 8;
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, LED);
            SysCtlDelay(2000000);
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0);
            SysCtlDelay(5000000);
            var++;
            break;

    }
}
void debounce (void){
    if (GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4) == 0){
        boton = 1;
    }else{
        if (boton == 1){
            boton = 0;
            enable = 1;
            var = 0;
        }
    }
}
