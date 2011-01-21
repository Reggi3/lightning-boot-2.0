/* LF1000 SPI Driver API
 *
 * spi.h -- SPI control
 *
 * Scott Esters
 * LeapFrog Enterprises
 *
 */

#ifndef LF1000_SPI_H
#define LF1000_SPI_H

int lf1000_spi_xfer(u16 data);		// write 16 bits out SPI port

#endif
