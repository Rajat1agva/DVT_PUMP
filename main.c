/*
 * PWM007.c
 *
 * Created: 8/26/2021 11:02:49 AM
 * Author : RAJAT 
 */ 

#define  F_CPU 4000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include <avr/xmega.h>
#include "I2C_0_AVR128DA64.h"
#include "MPRSS_sensor.h"
#include "UART_1_AVR128DA64 (1).h"



void pwm_init(void);
void pwm1(void);
void TCB_TIMER_init(void);

bool v4_H = true;
uint8_t VALVE1_TIMER = 0;
uint8_t VALVE2_TIMER = 0;
uint8_t VALVE4_TIMER = 0;
bool V4_30sec_flag = true;
int main(void)
{
   	
	PORTE.DIR |= PIN0_bm; // VALVE 1 
	PORTC.DIR |= PIN4_bm; //VALVE 3 
	PORTC.DIR |= PIN5_bm; // VALVE 2
	PORTE.DIR |= PIN1_bm; //VALVE 4
	//PORTE.OUTSET |= PIN0_bm;
	//PORTC.PIN5CTRL |= (1<<7);
  //  PORTMUX.TCAROUTEA |= 0x4;
	//PORTMUX.TCAROUTEA |= (1<<3); //SET PC4 AND PC5 OUTPUT FOR WO AND W1
	//PORTA.OUTSET |= (PIN3_bm);
 //CLKCTRL.MCLKCTRLB |= 0x04;
//CLKCTRL.MCLKCTRLA |= 0x2;
   USART1_init(9600);
	pwm_init();
	pwm1();
	TCB_TIMER_init();
	Pressure_init();
	//TCB_TIMER_init();
    /* Replace with your application code */
    while (1) 
    {
	USART1_sendFloat(Pressure_read(),1);
    _delay_ms(100);
    }
}
void pwm_init(void)
{
	TCA0.SINGLE.PER = 3906;
  //  TCA0.SINGLE.CNT = 0;
	//TCA0.SINGLE.INTCTRL |= (1<<0) ;

	TCA0.SINGLE.CTRLA  |= 0x0f;// set clock divided by 1024
    
	//TCA0.SINGLE.CTRLB  |= 0x13; //PWM generation mode Enable cmp0
	
	//TCA0.SINGLE.CTRLB  |= 0x71; //Enable CMP0,CMP1,CMP2
	
	//TCA0.SINGLE.CMP0 = 4881; //5 seconds  
	//TCA0.SINGLE.CMP1 = 29295; //30 seconds
	//TCA0.SINGLE.CMP2 = 14648; //15 seconds
	TCA0.SINGLE.INTCTRL |= 0x01; //Enable interrupt for ovf
	//TCA0.SINGLE.INTCTRL |= 0x70; //Set Interrupt Enable for CMP0 CMP1 CMP2
	
	sei();
   
}
void pwm1(void)
{
	TCA1.SINGLE.PER = 3906; // set time period 60 seconds
	TCA1.SINGLE.CTRLA |= 0x0f; //Enable time with clock div 1024
	TCA1.SINGLE.INTCTRL |= (1<<0);
	//TCA1.SINGLE.CTRLB |= 0x33; //selects operating mode pwm and enable 
	//TCA1.SINGLE.CMP0 = 14647;  // 15 seconds
    //	TCA1.SINGLE.CMP1 = 29295;  // 30 seconds
	 
	
}
void TCB_TIMER_init(void)
{
	TCB0.CCMP = 3906;
	TCB0.CTRLA |= 0x05;
	TCB0.INTCTRL |= (1<<0);
}
ISR(TCA0_OVF_vect)
{   VALVE1_TIMER=VALVE1_TIMER+1;
	if(VALVE1_TIMER <= 29)
	{
		
	
	if(VALVE1_TIMER <= 5)
	{  PORTE.OUT |= (1<<0);
		                   //SET HIGH VALVE 1 FOR 5 SECONDS
	}
	else 
	{
		PORTE.OUT &=~ (1<<0); //LOw FOR 25 seconds
		
	}
	}
	else
	{
		VALVE1_TIMER = 0;
		
	}
	TCA0.SINGLE.INTFLAGS |= (1<<0);
	
}
ISR(TCA1_OVF_vect)
{  VALVE2_TIMER=VALVE2_TIMER+1;
	if(VALVE2_TIMER <= 59)
	{   if(VALVE2_TIMER==30)
		{
			PORTC.OUT ^= (1<<5); 
		}
		if(VALVE2_TIMER <= 15)
		{  PORTC.OUT |= (1<<4);
			//SET HIGH VALVE 3 FOR 5 SECONDS
		}
		else
		{
			PORTC.OUT &=~ (1<<4); //LOw FOR 25 seconds
			
		}
	}
	else
	{   PORTC.OUT ^= (1<<5); 
		VALVE2_TIMER = 0;
		
	}
	
	
	
TCA1.SINGLE.INTFLAGS |= (1<<0);	
}
ISR(TCB0_INT_vect)
{   VALVE4_TIMER = VALVE4_TIMER+1;
	if((VALVE4_TIMER == 29)&&(V4_30sec_flag))
	{
		V4_30sec_flag = false;
	    VALVE4_TIMER = 0;
	}
	if(V4_30sec_flag)
	{ 
	}
	else
	{
		if(VALVE4_TIMER <= 59)
		{   
			if(VALVE4_TIMER <= 15)
			{  PORTE.OUT |= (1<<1);
				//SET HIGH VALVE 3 FOR 5 SECONDS
			}
			else
			{
				PORTE.OUT &=~ (1<<1); //LOw FOR 25 seconds
				
			}
		}
		else
		{ 
			VALVE4_TIMER = 0;
			
		}
	}
    TCB0.INTFLAGS |= (1<<0);
}
