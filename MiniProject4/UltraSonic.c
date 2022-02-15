/*
 * UltraSonic.c
 *
 *  Created on: Oct 18, 2021
 *      Author: Mostafa Saftawy
 */


#include "UltraSonic.h"
#include "icu.h"
#include "gpio.h"
#include <util/delay.h>
#include <avr/io.h>

uint32  distance=0;
uint8 g_edgeCount = 0;
uint16 g_timeHigh = 0;


void Ultrasonic_init(void)
{
	/* Create configuration structure for ICU driver */
	Icu_ConfigType Icu_Config = {F_CPU_8,RISING};

	Icu_init(&Icu_Config);

	GPIO_setupPinDirection(PORTB_ID,PIN5_ID,PIN_OUTPUT);

	GPIO_setupPinDirection(PORTD_ID,PIN5_ID,PIN_INPUT);

	Icu_setCallBack(Ultrasonic_edgeProcessing);

	GPIO_writePin(PORTB_ID,PIN5_ID,LOGIC_LOW);

	SREG |= (1<<7);


}

void Ultrasonic_Trigger(void)
{
	GPIO_writePin(PORTB_ID,PIN5_ID,LOGIC_HIGH);
	_delay_us(10);
	GPIO_writePin(PORTB_ID,PIN5_ID,LOGIC_LOW);

}

uint16 Ultrasonic_readDistance(void)
{
	/* start to output logic high on the trigger pin */

	Ultrasonic_Trigger();

	distance= (g_timeHigh/57.5);

	return distance;
}

void Ultrasonic_edgeProcessing(void)
{

	if(g_edgeCount==0){
		/*
		 *  clear timer to start your measurements from failing edge
		 */
		 Icu_clearTimerValue();
		 /*
		  * detect the failing edge
		  */
		Icu_setEdgeDetectionType(FALLING);

		g_edgeCount=1;

	}
	else if (g_edgeCount==1){
		/*
		 * Store the High time value
		 */
		g_timeHigh = Icu_getInputCaptureValue();
	    /*
	     * Detect rising edge
	      */
		Icu_setEdgeDetectionType(RISING);
		/* to detect Falling for next edge*/
		g_edgeCount=0;
	}
}
