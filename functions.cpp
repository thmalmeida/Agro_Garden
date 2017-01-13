#include "functions.h"


void counter1s_Timer1()
{
//	[TCCR1B]: ICNC1 ICES1 – WGM13 WGM12 CS12 CS11 CS10
	TCCR1B = 0b00011100;

//	[TCCR1A]: COM1A1 COM1A0 COM1B1 COM1B0 FOC1A FOC1B WGM11 WGM10
	TCCR1A = 0b00000010;

//	[TIMSK]: OCIE2 TOIE2 TICIE1 OCIE1A OCIE1B TOIE1 – TOIE0
	TIMSK 	= 0b00000100;

	ICR1 = 31249;

	/*
	 * 	f_pwm = fclk/[N*(1+TOP)];
	 * 	f_pwm = 8000000/[256*(1+TOP)];
	 * 	f_pwm = 8000000/[256*(1+31249)];
	 */
	// mode [10] PWM, Phase correct
	// I want a 1Hz clock to create a 1 second.
	// Dividing the 8MHz clock by 256
	// And put the top ICR to 31250/2.

	//	TCCR1A = COM1A1 COM1A0 COM1B1 COM1B0 FOC1A FOC1B WGM11 WGM10
//	TCCR1A = 0b00000010;
//	//	TCCR1B = ICNC1 ICES1 – WGM13 WGM12 CS12 CS11 CS10
//	TCCR1B = 0b00010100;
//
//	//	TIMSK = OCIE2 TOIE2 TICIE1 OCIE1A OCIE1B TOIE1 – TOIE0
//	TIMSK = 0b00000100;
//
//	ICR1 = 15625;
//
//	OCR1A = 15625;

}
void counter1ms_Timer2()
{
	/*
	 * PWM, Fast PWM mode OCRA on TOP
	 * f = fosc/(N*256), N = (1, 8, 32, 64, 128, 256, or 1024);
	 * f = 1000000/(64*256) = 976.5625 Hz;  N = 64, CS2[2:0] = 100;
	 *
	 * f  = 976.5625 Hz;			frequency from bottom to top
	 * t  = T/256 = 4.0 us;		divided by sub intervals
	 * OCR2A = 250;
	 * T2 =	t*OCR2A;
	 * T2 = f/256*OCR2A
	 * 1 ms / t = 250 un;
	 * f = 1 kHz;
	*/

	/*
	 * PWM, Fast PWM mode OCRA on TOP
	 * f = fosc/(N*256), N = (1, 8, 32, 64, 128, 256, or 1024);
	 * f = 1000000/(64*256) = 976.5625 Hz;  N = 64, CS2[2:0] = 100;
	 *
	 * f  = 976.5625 Hz;			frequency from bottom to top
	 * t  = T/256 = 4.0 us;		divided by sub intervals
	 * OCR2A = 250;
	 * T2 =	t*OCR2A;
	 * T2 = f/256*OCR2A
	 * T2 = 1/(8000000/(32*256))/256*250 = 1 ms;
	 * 1 ms / t = 250 un;
	 * f = 1 kHz;
	*/





	// [TCCR2]: FOC2 WGM20 COM21 COM20 WGM21 CS22 CS21 CS20
	TCCR2 = 0b01001011;

	// [TIMSK]: OCIE2 TOIE2 TICIE1 OCIE1A OCIE1B TOIE1 – TOIE0
	TIMSK |= 0b01000000;

	OCR2 = 250;



	// [TCCR2A]: COM2A1 COM2A0 COM2B1 COM2B0 – – WGM21 WGM20
//	TCCR2A = 0b00000011;

	// [TCCR2B]: FOC2A FOC2B – – WGM22 CS22 CS21 CS20
//	TCCR2B = 0b00001100;

	// [TIMSK2]: – – – – – OCIE2B OCIE2A TOIE2
//	TIMSK2 = 0b00000010;

//	OCR2A = 250;
}
