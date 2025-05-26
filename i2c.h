#ifndef I2C_H
#define I2C_H

#include <xc.h>

#define ANSEL_SCL ANSELCbits.ANSC3
#define ANSEL_SDA ANSELCbits.ANSC4

#define TRIS_SCL TRISCbits.TRISC3
#define TRIS_SDA TRISCbits.TRISC4

void I2C_init(void);
void I2C_start(void);
void I2C_stop(void);
void I2C_write(unsigned char data);

#endif