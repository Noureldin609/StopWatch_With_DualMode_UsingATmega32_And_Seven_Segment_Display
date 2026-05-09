#include<avr/io.h>
#include<avr/interrupt.h>
#include<util/delay.h>
unsigned char hours_1=0, hours_2=0 , mins_1=5	 ,mins_2 =0 , seconds_1 =0 ,seconds_2 =0;
unsigned char flag=0 ,flag2=0,flag3=0 ,flag4=0,flag5=0 ,flag6=0;
ISR(TIMER1_COMPA_vect){
	if(PORTD & (1<<4)){
		PORTD &= ~(1<<0);
	 if(hours_1==9 && mins_1==9 && mins_2 ==5 && seconds_1 ==9 && seconds_2 ==5){
		hours_2++;
		hours_1=0;
	    mins_1=0;
		mins_2=0;
		seconds_1=0;
		seconds_2=0;
	}
	 else if(mins_1==9 && mins_2 ==5 && seconds_1 ==9 && seconds_2 ==5){
	    hours_1++;
	    mins_1=0;
		mins_2=0;
		seconds_1=0;
		seconds_2=0;
	}
	 else if (mins_1==9 && mins_2 !=5 && seconds_1 ==9 && seconds_2 ==5){
		mins_2 ++;
		mins_1=0;
		seconds_1=0;
		seconds_2=0;
	}
	else if(seconds_1 ==9 && seconds_2 ==5){
		mins_1 ++;
		seconds_1=0;
		seconds_2=0;
	}
	else if(seconds_1 ==9 && seconds_2 !=5){
		seconds_2 ++;
		seconds_1=0 ;
	}

	else {
		seconds_1 ++;
	}
	}
	else if(PORTD & (1<<5)){
		if(hours_1==0 && mins_1==0 && mins_2 ==0 && seconds_1 ==0 && seconds_2 ==0 && hours_2==0){
		hours_2=0;
		hours_1=0;
	    mins_1=0;
		mins_2=0;
		seconds_1=0;
		seconds_2=0;
		PORTD |= (1<<0);
	//	 PORTD ^=(1<<5);
	//	 PORTD ^=(1<<4);
	}
	else if(hours_1==0 && mins_1==0 && mins_2 ==0 && seconds_1 ==0 && seconds_2 ==0 && hours_2!=0){
			hours_2--;
			hours_1=9;
		    mins_1=9;
			mins_2=5;
			seconds_1=9;
			seconds_2=5;
		}
		 else if(mins_1==0 && mins_2 ==0 && seconds_1 ==0 && seconds_2 ==0 && hours_1 !=0){
		    hours_1--;
		    mins_1=9;
			mins_2=5;
			seconds_1=9;
			seconds_2=5;
		}
    else if (mins_1==0 && mins_2 !=0 && seconds_1 ==0 && seconds_2 ==0){
			mins_2 --;
			mins_1=9;
			seconds_1=9;
			seconds_2=5;
		}
		else if(seconds_1 ==0 && seconds_2 ==0 && mins_1 !=0){
			mins_1 --;
			seconds_1=9;
			seconds_2=5;
		}
		else if(seconds_1 ==0 && seconds_2 !=0){
			seconds_2 --;
			seconds_1=9 ;
		}

		else {
			seconds_1 --;
		}
	}
}
void display(){
	PORTA |= (1<<5);
	PORTC = (PORTC & 0xF0) | (seconds_1 & 0x0F);
	_delay_ms(2);
	PORTA &= ~(1<<5);
	PORTA |= (1<<4);
	PORTC = (PORTC & 0xF0) | (seconds_2 & 0x0F);
	_delay_ms(2);
	PORTA &= ~(1<<4);
	PORTA |= (1<<3);
	PORTC = (PORTC & 0xF0) | (mins_1 & 0x0F);
	_delay_ms(2);
	PORTA &= ~(1<<3);
	PORTA |= (1<<2);
	PORTC = (PORTC & 0xF0) | (mins_2 & 0x0F);
	_delay_ms(2);
	PORTA &= ~(1<<2);
	PORTA |= (1<<1);
	PORTC = (PORTC & 0xF0) | (hours_1 & 0x0F);
	_delay_ms(2);
	PORTA &= ~(1<<1);
	PORTA |= (1<<0);
	PORTC = (PORTC & 0xF0) | (hours_2 & 0x0F);
	_delay_ms(2);
	PORTA &= ~(1<<0);
}
void timer1_compare(){
	TCCR1A |= (1<<FOC1A);
	TCCR1B |= (1<<WGM12) | (1<<CS12); //| (1<<CS10);
	TCNT1=0;
	OCR1A=62500;
	SREG |= (1<<7);
	TIMSK |= (1<<OCIE1A);
}
void interrupt0(){
	DDRD &= ~ (1<<2);
	PORTD |= (1<<2);
	MCUCR |= (1<<ISC01);
	MCUCR &= ~(1<<ISC00);
	GICR |=(1<< INT0);
	 SREG |=(1<<7);
}
ISR(INT0_vect){
	hours_1=0, hours_2=0 , mins_1=0	 ,mins_2 =0 , seconds_1 =0 ,seconds_2 =0;
}
void interrupt1(){
	 DDRD  &= ~(1<<3);
	 MCUCR |= (1<<ISC11) | (1<<ISC10) ;
	 GICR  |= (1<< INT1);
	 SREG  |=(1<<7);
}
ISR(INT1_vect){
	TCCR1B &= ~(1<<CS10) & (1<<CS11)  & (1<<CS12);
	 PORTD &= ~(1<<0);
}
void interrupt2(){
	DDRB &=~(1<<PB2);
	PORTB |= (1<<2);
	MCUCSR &= ~ (1<<ISC2);
	GICR |= (1<<INT2);
	SREG |=(1<<7);
}
ISR(INT2_vect){
	TCCR1B |= (1<<CS12); //| (1<<CS10) ;
	TCCR1B &= ~(1<<CS11);
}
void increments(){
	if (!(PINB & (1<<1))){
		_delay_ms(30);
		if (!(PINB & (1<<1))){
			if(flag==0 && hours_1 !=9){
						hours_1 ++;
			flag=1;
			}
			else if (flag ==0 && hours_2 !=9  && hours_1 ==9){
				hours_2 ++;
				hours_1 = 0;
				flag=1;
			}
			else if(flag==0 && hours_2 == 9 && hours_1 == 9){
				hours_2 =0;
				hours_1=0;
				flag=1;
			}
		       }
		}
else {
	flag=0;
}
	if (!(PINB & (1<<4))){
		_delay_ms(30);
		if (!(PINB & (1<<4))){
			if(flag3==0 && mins_1 !=9){
				 mins_1 ++;
			flag3=1;
			}
			else if (flag3 ==0 &&  mins_2 !=5  &&  mins_1 ==9){
				 mins_2 ++;
				 mins_1 = 0;
				flag3=1;
			}
			else if(flag3==0 &&  mins_2 == 5 &&  mins_1 == 9){
				 mins_2 =0;
				 mins_1=0;
				flag3=1;
			}
		       }
		}
else {
	flag3=0;
}
	if (!(PINB & (1<<6))){
		_delay_ms(30);
		if (!(PINB & (1<<6))){
			if(flag5==0 && seconds_1 !=9){
				seconds_1 ++;
			flag5=1;
			}
			else if (flag5 ==0 && seconds_2 !=5  && seconds_1 ==9){
				seconds_2 ++;
				seconds_1 = 0;
				flag5=1;
			}
			else if(flag5==0 && seconds_2 == 5 && seconds_1 == 9){
				seconds_2 =0;
				seconds_1=0;
				flag5=1;
			}
		       }
		}
else {
	flag5=0;
}
}
void decrement(){
	if (!(PINB & (1<<0))){
	_delay_ms(30);
	if (!(PINB & (1<<0))){
		if(flag2==0 && hours_1 !=0){
					hours_1 --;
		flag2=1;
		}
		else if (flag2 ==0 && hours_2 !=0  && hours_1 ==0){
			hours_2 --;
			hours_1 = 9;
			flag2=1;
		}
		else if(flag2==0 && hours_2 == 0 && hours_1 == 0){
			hours_2 =0;
			hours_1=0;
			flag2=1;
		}
	       }
	}
else {
flag2=0;
}
if (!(PINB & (1<<3))){
	_delay_ms(30);
	if (!(PINB & (1<<3))){
		if(flag4==0 && mins_1 !=0){
			mins_1 --;
		flag4=1;
		}
		else if (flag4 ==0 && mins_2 !=0  && mins_1 ==0){
			mins_2 --;
			mins_1 = 9;
			flag4=1;
		}
		else if(flag4==0 && mins_2 == 0 && mins_1 == 0){
			mins_2 =0;
			mins_1=0;
			flag4=1;
		}
	       }
	}
else {
flag4=0;
}
if (!(PINB & (1<<5))){
	_delay_ms(30);
	if (!(PINB & (1<<5))){
		if(flag6==0 && seconds_1 !=0){
			seconds_1 --;
		flag6=1;
		}
		else if (flag6 ==0 && seconds_2 !=0  && seconds_1 ==0){
			seconds_2 --;
			seconds_1 = 9;
			flag6=1;
		}
		else if(flag6==0 && seconds_2 == 0 && seconds_1 == 0){
			seconds_2 =0;
			seconds_1=0;
			flag6=1;
		}
	       }
	}
else {
flag6=0;
}
}
int main(){
	DDRA |= 0X3f;
	DDRC |= 0X0F;//(1<<0) | (1<<1)| (1<<2)| (1<<3);
	DDRD &= 0xC2;//~ (1<<0) & (1<<2) & (1<<3) & (1<<4) & (1<<5);
	DDRB &= 0x84; //~(1<<0) & (1<<1)& (1<<3)& (1<<4)& (1<<5)& (1<<6);
	DDRB &= ~(1<<7);
	PORTB |= (1<<7);
	PORTB |= 0X7B;
    DDRD  |= (1<<4)|(1<<5)|(1<<0);
    PORTD |=(1<<4);
    PORTD &=~(1<<5);
	timer1_compare();
	interrupt0();
	interrupt1();
	interrupt2();
	while(1){ display();
			if((!(PINB & (1<<7)))&& (!(TCCR1B&(1<<2)))){
			_delay_ms(30);
			if((!(PINB & (1<<7)))&& (!(TCCR1B&(1<<2)))){
					   PORTD ^=(1<<5);
					    PORTD ^=(1<<4);
					    while((!(PINB & (1<<7)))&& (!(TCCR1B&(1<<2)))){}
					    display();
				}
			}
			increments();
			decrement();
	}
}



