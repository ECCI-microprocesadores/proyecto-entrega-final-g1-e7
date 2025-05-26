#ifndef LCD_I2C_H
#define LCD_I2C_H

#define _XTAL_FREQ 4000000
#define ADDRESS_LCD 0x4E

void lcd_init(void);
void lcd_cmd(unsigned char cmd);
void lcd_set_cursor(unsigned char row, unsigned char col);
void lcd_write_char(char c);
void lcd_write_string(const char *str);
void lcd_clear(void);

#endif