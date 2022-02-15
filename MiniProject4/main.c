/*
 * main.c
 *
 *  Created on: Oct 19, 2021
 *      Author: Mostafa Saftawy
 */
#include "UltraSonic.h"
#include "lcd.h"


void main(void)
{
	LCD_init();
	Ultrasonic_init();
	uint16 Distance=0;
	while(1){
		LCD_displayStringRowColumn(0,1,"Distance = ");
		Distance=Ultrasonic_readDistance();
		LCD_intgerToString(Distance);
		LCD_displayString(" cm");
	}
}
