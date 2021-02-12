/* 
 * File:   initSPI
 * Author: Carlos Gil 
 * Comments: Liberria para configurar el SPI 
 * Revision history: 
 */

#ifndef INITSPI_H
#define	INITSPI_H
#include <stdint.h>
#include <xc.h> // include processor files - each processor file is guarded.  

void initSPIMASTER(uint8_t modo);
void initSPISLAVE(uint8_t modos);
#endif	/* INITSPI */

