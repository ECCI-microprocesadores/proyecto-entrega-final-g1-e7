#include "i2c.h"

void I2C_init(void)
{
    TRIS_SCL = 1;
    TRIS_SDA = 1;
    
    ANSEL_SCL = 0;  // Desactiva función analógica en SCL (RC3)
    ANSEL_SDA = 0; 
    
    SSPSTAT = 0x80;
    SSPCON1 = 0x28;
    SSPCON2 = 0x00;
    SSPCON1bits.SSPEN = 1;
}

void I2C_start(void)
{
    SSPCON2bits.SEN = 1;
    while(!PIR1bits.SSPIF);
    PIR1bits.SSPIF = 0;
}

void I2C_stop(void)
{
    SSPCON2bits.PEN = 1;
    while(!PIR1bits.SSPIF);
    PIR1bits.SSPIF = 0;
}

void I2C_write(unsigned char data)
{
    SSPBUF = data;
    while(!PIR1bits.SSPIF);
    PIR1bits.SSPIF = 0;
}
