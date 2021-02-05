
/* 
 * File:   TABLAACSII_HEX
 * Author: Carlos Gil 
 * Comments: funcion para transformar un numero a 
 * hex en ACSII
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef TABLAACSII_HEX_H
#define	TABLAACSII_HEX_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdint.h>

void tabla_hex (uint8_t selector, uint8_t *puerto);
#endif	/* XC_HEADER_TEMPLATE_H */

