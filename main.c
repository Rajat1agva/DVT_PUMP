/*
 * PWM007.c
 *
 * Created: 8/26/2021 11:02:49 AM
 * Author : agva
 */ 

#define  F_CPU 1000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include <avr/xmega.h>

#define VALVE1 PE0
#define VALVE2 PA6
#define VALVE3 PA2
#define VALVE4 PB5


void pwm_init(void);
void pwm1(void);


bool v4_H = true;

int main(void)
{
	_PROTECTED_WRITE (CLKCTRL.OSCHFCTRLA, ((CLKCTRL_FREQSEL_1M_gc)  //Oscillator frequency is 1MHz
									  |(CLKCTRL_AUTOTUNE_bm)));  // Auto tuning
	PORTE.DIR |= PIN0_bm; // VALVE 1 
	PORTA.DIR |= PIN3_bm; //VALVE 4
	PORTC.DIR |= PIN4_bm; //VALVE 3 
	PORTC.DIR |= PIN5_bm; // VALVE 2
	PORTE.OUTSET |= PIN0_bm;
	PORTA.OUTCLR |= PIN3_bm;
	PORTC.PIN5CTRL |= (1<<7);
    PORTMUX.TCAROUTEA |= 0x4;
	PORTMUX.TCAROUTEA |= (1<<3); //SET PC4 AND PC5 OUTPUT FOR WO AND W1
	//PORTA.OUTSET |= (PIN3_bm);
 //CLKCTRL.MCLKCTRLB |= 0x04;
//CLKCTRL.MCLKCTRLA |= 0x2;
    
	pwm_init();
	pwm1();
    /* Replace with your application code */
    while (1) 
    {if(v4_H)
		{
			_delay_ms(30000);
			v4_H=false;
		}
		PORTA.OUTSET |= (PIN3_bm);
	 _delay_ms(15000);
	 PORTA.OUTCLR |=(PIN3_bm);
	 _delay_ms(45000);
    }
}
void pwm_init(void)
{
	TCA0.SINGLE.PER = 29295;
  //  TCA0.SINGLE.CNT = 0;
	//TCA0.SINGLE.INTCTRL |= (1<<0) ;

	TCA0.SINGLE.CTRLA  |= 0x0f;// set clock divided by 1024
    
	TCA0.SINGLE.CTRLB  |= 0x13; //PWM generation mode Enable cmp0
	
	//TCA0.SINGLE.CTRLB  |= 0x71; //Enable CMP0,CMP1,CMP2
	
	TCA0.SINGLE.CMP0 = 4881; //5 seconds  
	//TCA0.SINGLE.CMP1 = 29295; //30 seconds
	//TCA0.SINGLE.CMP2 = 14648; //15 seconds
	//TCA0.SINGLE.INTCTRL |= 0x01; //Enable interrupt for ovf
	//TCA0.SINGLE.INTCTRL |= 0x70; //Set Interrupt Enable for CMP0 CMP1 CMP2
	
	sei();

}
void pwm1(void)
{
	TCA1.SINGLE.PER = 58593; // set time period 60 seconds
	TCA1.SINGLE.CTRLA |= 0x0f; //Enable time with clock div 1024
	TCA1.SINGLE.CTRLB |= 0x33; //selects operating mode pwm and enable 
	TCA1.SINGLE.CMP0 = 14647;  // 15 seconds
	TCA1.SINGLE.CMP1 = 29295;  // 30 seconds
	 
	
}
