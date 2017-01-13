#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>

#include "functions.h"
#include "lcdt.h"

//#include <Arduino.h>

const uint16_t timeON_min_set = 10;		// in minutes
const uint16_t timeOFF_min_set = 60*6;	// minutes plus hours

uint8_t timeOFF_sec = 0;
uint16_t timeOFF_min = timeOFF_min_set;
uint8_t timeON_sec = 0;
uint16_t timeON_min = timeON_min_set;
const uint8_t time_secMax = 59;

uint8_t state = 0;
volatile uint8_t flag_1s = 0;

#define DefOut_redLED()			DDRB |=  (1<<1);
#define DefOut_greenLED()		DDRB |=  (1<<2);
#define DefOut_triacStart()		DDRD |=  (1<<4);	// Signal to power kontactor
#define DefOut_triacHold()		DDRD |=  (1<<3);	// Signal to power kontactor
#define DefOut_buttonSignal()	DDRC |=  (1<<1);

#define DefIn__Signal()			DDRC &= ~(1<<0);
#define DefIn__Button()			DDRD &= ~(1<<2);

#define read_Signal				bit_is_set(PINC, 0)
#define read_driveTriac			bit_is_set(PIND, 3)
//#define read_button				bit_is_clear(PIND, 2)
#define read_button				~(PIND & (1<<2))

#define drive_butSignal_on()	PORTC |=  (1<<2);
#define drive_butSignal_off()	PORTC &= ~(1<<2);

#define drive_redLED_on()  		PORTB |=  (1<<1);
#define drive_redLED_off() 		PORTB &= ~(1<<1);
#define drive_redLED_toggle()  	PORTB ^=  (1<<1);

#define drive_greenLED_on()		PORTB |=  (1<<2);
#define drive_greenLED_off() 	PORTB &= ~(1<<2);
#define drive_greenLED_toogle() PORTB ^= (1<<2);

#define drive_triacStart_on()	PORTD |=  (1<<4);
#define drive_triacStart_off()	PORTD &= ~(1<<4);

#define drive_triacHold_on()	PORTD |=  (1<<3);
#define drive_triacHold_off()	PORTD &= ~(1<<3);

void init_IO()
{
	DefIn__Button();
	PORTD |= (1<<2);
//	DefIn__Signal();

	DefOut_greenLED();
	DefOut_redLED();
//	DefOut_triacHold();
	DefOut_triacStart();

//	DefOut_buttonSignal();
}

ISR(TIMER1_OVF_vect)
{
	flag_1s = 1;
}

void go_OFF()
{
	state = 0;
	drive_redLED_off();
	drive_greenLED_on();
	drive_triacStart_off();
	timeOFF_min = timeOFF_min_set;
	timeOFF_sec = 0;

	char buffer[16];
	sprintf(buffer,"Desligado");
	lcdt_print(0,0, buffer);
	memset(buffer,0,sizeof(buffer));
}
void go_ON()
{
	state = 1;
	drive_redLED_on();
	drive_greenLED_off();
	drive_triacStart_on();
	timeON_min = timeON_min_set;
	timeON_sec = 0;

	char buffer[16];
	sprintf(buffer,"Ligado   ");
	lcdt_print(0,0, buffer);
	memset(buffer,0,sizeof(buffer));
}
void display_refresh()
{
	char buffer[16];
	if(state)
	{
		sprintf(buffer,"%3.d:%.2d", timeON_min, timeON_sec);
		lcdt_print(0,1,buffer);
		memset(buffer,0,sizeof(buffer));
	}
	else
	{
		sprintf(buffer,"%3.d:%.2d", timeOFF_min, timeOFF_sec);
		lcdt_print(0,1,buffer);
		memset(buffer,0,sizeof(buffer));
	}


}
void PMS()
{
	if(state == 0)
	{
		if(!timeOFF_sec)
		{
			if(!timeOFF_min)
			{
				go_ON();
			}
			else
			{
				timeOFF_min--;
				timeOFF_sec = time_secMax;
			}
		}
		else
		{
			timeOFF_sec--;
		}
	}
	else
	{
		if(!timeON_sec)
		{
			if(!timeON_min)
			{
				go_OFF();
			}
			else
			{
				timeON_min--;
				timeON_sec = time_secMax;
			}
		}
		else
		{
			timeON_sec--;
		}
	}
	display_refresh();
}

int main(void)
{
//	Initialize arduino hardware requirements.
//	init();
	init_IO();

	cli();
	counter1s_Timer1();
	sei();

	lcdt_begin(16,2);
	lcdt_print(0,0, "Starting...");
	_delay_ms(500);
	lcdt_clear();
	go_OFF();

	while (1)
	{
		if(bit_is_clear(PIND, 2))
		{
			_delay_ms(250);
			while(bit_is_clear(PIND,2));
			_delay_ms(250);
			if(!state)
			{
				go_ON();
			}
			else
			{
				go_OFF();
			}
		}

		if(flag_1s)
		{
			flag_1s ^= flag_1s;

			PMS();
		}
	}
}


//#define Th_read (~PIND & 0b00001000)
//uint8_t motorStatus = 0;
//void motorStart()
//{
//	// LEDs
//	drive_redLED_on();
//	drive_greenLED_off();
//
//	// Triacs
//	drive_triacStart_on();
//	drive_triacHold_on();
//
//	_delay_ms(500);
//	drive_triacStart_off();
//
//	motorStatus = 1;
//	_delay_ms(3000);
//}
//void motorStop()
//{
//	// LEDs
//	drive_redLED_off();
//	drive_greenLED_on()
//
//	// Triacs
//	drive_triacHold_off();
//	drive_triacStart_off();
//
//	motorStatus = 0;
//	_delay_ms(3000);
//}
//void init_Timer1_1Hz()
//{
//	// Timer 1 with 16 bit time counter. On a Fast PWM
//	// TCCR1A <==	COM1A1	COM1A0	COM1B1	COM1B0	COM1C1	COM1C0	WGM11	WGM10
//	TCCR1A = 0b00000010;
//
//	// TCCR1B <==	ICNC1	ICES1	–		WGM13	WGM12	CS12	CS11	CS10
//	TCCR1B = 0b00011101; // Start timer at Fcpu/1024
//
//	// TIMSK1 <==	–		–		ICIE1	–		OCIE1C	OCIE1B	OCIE1A	TOIE1
////	TIMSK1 |= (1 << OCIE1A);
//	TIMSK1 = 0b00000010;
//
//	ICR1 = 15624; // To obtain 1Hz clock.
//}
