#ifndef LCD_DISPLAY_H
#define LCD_DISPLAY_H

#include "main.h"

#define LCD_DISPLAY_I2C_ADDR	0x27
// #define LCD_DISPLAY_I2C_ADDR	0x3F

void lcd_init(void);
void lcd_setCursor(int a, int b);
void lcd_send_string(char *str);
void lcd_clear(void);

#endif