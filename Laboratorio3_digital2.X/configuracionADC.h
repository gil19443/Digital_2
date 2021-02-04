
/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef CONFIGURACIONADC_H
#define	CONFIGURACIONADC_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdint.h>
void configADC(uint8_t canal, uint8_t vel);
#endif	/* XC_HEADER_TEMPLATE_H */

