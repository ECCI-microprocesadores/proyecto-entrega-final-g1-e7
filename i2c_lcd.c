#include <xc.h>
#include "i2c.h"
#include "i2c_lcd.h"


void lcd_init(void)
{
    __delay_ms(20);
    lcd_cmd(0x33);
    lcd_cmd(0x32);
    lcd_cmd(0x28);
    lcd_cmd(0x0C);
    lcd_cmd(0x06);
    lcd_cmd(0x01);
    __delay_ms(3);
}

void lcd_cmd(unsigned char cmd)
{
    char data_u, data_l;
    data_u = (cmd & 0xF0);
    data_l = ((cmd << 4) & 0xF0);

    I2C_start();
    I2C_write(ADDRESS_LCD);
    I2C_write(data_u | 0x0C);
    I2C_write(data_u | 0x08);
    I2C_write(data_l | 0x0C);
    I2C_write(data_l | 0x08);
    I2C_stop();
}

void lcd_write_char(char c)
{
    char data_u, data_l;
    data_u = (c & 0xF0);
    data_l = ((c << 4) & 0xF0);

    I2C_start();
    I2C_write(ADDRESS_LCD);
    I2C_write(data_u | 0x0D);
    I2C_write(data_u | 0x09);
    I2C_write(data_l | 0x0D);
    I2C_write(data_l | 0x09);
    I2C_stop();
}

void lcd_set_cursor(unsigned char row, unsigned char col)
{
    if (row == 0) lcd_cmd(0x80 + col);
    else lcd_cmd(0xC0 + col);
}


void lcd_write_string(const char *str)
{
    while(*str != '\0')
    {
        lcd_write_char(*str++);
    }
}

void lcd_clear(void)
{
    lcd_cmd(0x01);
    __delay_ms(2);
}
