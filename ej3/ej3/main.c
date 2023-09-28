/*
 * ej3.c
 *
 * Created: 27/9/2023 19:41:20
 * Author : Oriana
 */ 

#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

int umbral=50;
int marcha=0;
int cuenta=0;

void pollP1();
int pollP2();
void pollP3();
int select_umbral();

int main(void)
{
    	DDRA = 0xFF;							//puerto A como salida
    	PORTA = 0x00;							//inicializo apagado
    	DDRD &= ~((1 << DDD0) | (1 << DDD1) |(1 << DDD2));   //puertos D como entrada
    	//PORTD |= ((1 << PORTD0) | (1 << PORTD1));
		TCCR1B = (1 << WGM12); // Modo CTC1
		OCR1A = 625;
		TIMSK1 = (1 << OCIE1A); // Interrupción por igualación TCNT1 == OCR1A
		TCCR1B |= (1 << CS12);  // Prescaler 256. Inicia el conteo
		sei();                  // Setea bit global de interrupciones


    while (1) 
    {
		pollP1();
		pollP2();
	}
}

void pollP1(){
	if (!(PIND & (1 << PORTD1))){
		_delay_ms(10);
	if (PIND & (1 << PORTD1)){
		if (!marcha) umbral+=select_umbral();
		else cuenta=0;
		}
	}
}

int pollP2(){							//cambiar secuencia
	if (!(PIND & (1 << PORTD2))){
		_delay_ms(10);
		if (PIND & (1 << PORTD2)){
			if (marcha==0) marcha=1;
			else marcha=0;
		return 0;
		}
	}
	return 1;
}
void pollP3(){							//cambiar secuencia
	if (!(PIND & (1 << PORTD2))){
		_delay_ms(10);
		if (PIND & (1 << PORTD2)) cuenta++;
	}
			
}
int select_umbral (){
	int inc=0; int stop=1;
	while (stop){
		while (inc<350){
			if (!(PIND & (1 << PORTD1))){ //aumenta el umbral
				_delay_ms(10);
				if (PIND & (1 << PORTD1)) inc++;}
				if (!(pollP2())){ //si se presiona P2
					stop=0;
					return inc;
				}
			}
	}
	return inc;
}
