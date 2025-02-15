#include "lcd_display.h"

void lcd_write_i2c(char saddr,uint8_t *buffer, uint8_t length) {
	while (I2C1->SR2 & I2C_SR2_BUSY);
	I2C1->CR1 |= I2C_CR1_START;
	while (!(I2C1->SR1 & I2C_SR1_SB));
	volatile int Temp;
	I2C1->DR = saddr<< 1;
	while (!(I2C1->SR1 & I2C_SR1_ADDR));
	Temp = I2C1->SR2;

	for (uint8_t i=0;i<length;i++) {
		I2C1->DR=buffer[i];
		while (!(I2C1->SR1 & I2C_SR1_BTF));
	}

	I2C1->CR1 |= I2C_CR1_STOP;
}

void lcd_send_cmd (char cmd)
{
  char data_u, data_l;
	uint8_t data_t[4];
	data_u = (cmd&0xf0);
	data_l = ((cmd<<4)&0xf0);
	data_t[0] = data_u|0x0C;  //en=1, rs=0
	data_t[1] = data_u|0x08;  //en=0, rs=0
	data_t[2] = data_l|0x0C;  //en=1, rs=0
	data_t[3] = data_l|0x08;  //en=0, rs=0
	lcd_write_i2c(LCD_DISPLAY_I2C_ADDR, (uint8_t *) data_t, 4);
}

void lcd_send_data (char data)
{
	char data_u, data_l;
	uint8_t data_t[4];
	data_u = (data&0xf0);
	data_l = ((data<<4)&0xf0);
	data_t[0] = data_u|0x0D;  //en=1, rs=1
	data_t[1] = data_u|0x09;  //en=0, rs=1
	data_t[2] = data_l|0x0D;  //en=1, rs=1
	data_t[3] = data_l|0x09;  //en=0, rs=1
	lcd_write_i2c(LCD_DISPLAY_I2C_ADDR,(uint8_t *)data_t,4);
}

void lcd_init (void)
{
	// 4 bit initialisation
	HAL_Delay(50);  // wait for >40ms
	lcd_send_cmd (0x3);
	HAL_Delay(5);  // wait for >4.1ms
	lcd_send_cmd (0x3);
	HAL_Delay(1);  // wait for >100us
	lcd_send_cmd (0x3);
	HAL_Delay(10);
	lcd_send_cmd (0x2);  // 4bit mode
	HAL_Delay(10);

  // dislay initialisation
	lcd_send_cmd (0x28); // Function set --> DL=0 (4 bit mode), N = 1 (2 line display) F = 0 (5x8 characters)
	HAL_Delay(1);
	lcd_send_cmd (0x08); //Display on/off control --> D=0,C=0, B=0  ---> display off
	HAL_Delay(1);
	lcd_send_cmd (0x01);  // clear display
	HAL_Delay(1);
	HAL_Delay(1);
	lcd_send_cmd (0x06); //Entry mode set --> I/D = 1 (increment cursor) & S = 0 (no shift)
	HAL_Delay(1);
	lcd_send_cmd (0x0C); //Display on/off control --> D = 1, C and B = 0. (Cursor and blink, last two bits)
}

void lcd_setCursor(int a, int b)
{
	int i=0;
	switch(b){
	case 0:lcd_send_cmd(0x80);break;
	case 1:lcd_send_cmd(0xC0);break;
	case 2:lcd_send_cmd(0x94);break;
	case 3:lcd_send_cmd(0xd4);break;}
	for(i=0;i<a;i++)
	lcd_send_cmd(0x14);
}

void lcd_send_string (char *str)
{
	while (*str) lcd_send_data (*str++);
}

void lcd_clear (void)
{

	#define LCD_CLEARDISPLAY 0x01
	lcd_send_cmd(LCD_CLEARDISPLAY);
	HAL_Delay(100);

}