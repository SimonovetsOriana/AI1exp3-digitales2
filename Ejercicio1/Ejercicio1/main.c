
/*
 * Problema1.c
 *
 * Created: 15/9/2023 11:02:25
 * Author : Oriana
 */
//Juego de luces Programable
//- PB1: encender/apagar el juego de luces //
//- PB2: seleccionar la secuencia de encendido //
//- PB3:  modificar la velocidad de ejecución //
//------------------------------------------//

#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

volatile int FLAG=0;
int contador=0;
int velocidad=500;
int enable=0;


ISR (INT0_vect)
{
	_delay_ms(20);
	if (!(PIND & (1 << PORTD0))){
	FLAG=1;
	}
}

void sec1();
void sec2();
void sec3();
void sec4();
int pollPB2();
int pollPB3();
void rotar_sec();

int main(void)
{
	DDRA = 0x3F;							//puerto A como salida
	PORTA = 0x00;							//inicializo apagado
	DDRD &= ~((1 << DDD0) | (1 << DDD1) |(1 << DDD2));   //puertos D como entrada
	PORTD |= ((1 << PORTD0) | (1 << PORTD1) |  (1 << PORTD2)) ;
	EICRA |= (1<<ISC01);                    // config. interrup. INT0 sensible a flanco asc.
	EIMSK |= (1<<INT0);						//habilita interrp. interna INT0
	EIFR = 0x00;						    // borra flag INTF0 para evitar interrup. espúrea
	sei();									// activa interrupciones globalmente

	while (1){
		if (FLAG){
			if (enable==1) enable=0;
			else enable=1;
			_delay_ms(100);
			FLAG=0;
		}
		if (enable){
			PORTA = 0x00;					//apago después de cada sec.
			switch (contador){				// selección de secuencias
				case 0 :
				sec1();
				break;
				case 1 :
				sec2();
				break;
				case 2 :
				sec3();
				break;
				case 3:
				sec4();
				break;
			}
		}
		else PORTA = 0x00;		
	}
}

// interrupciones por encuestas 
int pollPB2(){							//cambiar secuencia
	if (!(PIND & (1 << PORTD1))){
		_delay_ms(10);
		if (PIND & (1 << PORTD1)){
			if (contador < 3) {
				contador++;
				} else {
				contador = 0;
			}
			return 0;
		}
	}
	return 1;
}
int pollPB3(){							//cambiar velocidad
	if (!(PIND & (1 << PORTD2))){
		_delay_ms(10);
		if (PIND & (1 << PORTD2)){
			if (velocidad==600) {
				velocidad=300;
				} else {
				velocidad=600;
			}
			return 0;
		}
	}
	return 1;
}
void sec1() {
	int stop=1; int t;
	while(stop){
		t=0;
		PORTA = 0x2A;
		while (t<velocidad){
			if (FLAG) return;
			_delay_ms(0.5);
			if (!(pollPB2())){
				stop=0;
				return;
			}
			if (!(pollPB3())){
				stop=0;
				return;
			}
			t++;
		}
		t=0;
		if (stop==1){
			PORTA = 0x15;
			while (t<velocidad){
				if (FLAG) return;
				_delay_ms(0.5);
				if (!(pollPB2())){
					stop=0;
					return;
				}
				if (!(pollPB3())){
					stop=0;
					return;
				}
				t++;
			}
		}
	}
}

void sec2(){
	int t; int stop=1;
	while (stop){
		for (int i = 0; i < 6; i++){
			PORTA |= (1 << i);
			t=0;
			while (t<velocidad){
				if (FLAG) return;
				_delay_ms(0.5);
				if (!(pollPB2())){
					stop=0;
					return;
				}
				if (!(pollPB3())){
					stop=0;
					return;
				}
				t++;
			}
			if (stop==1){
			PORTA &= ~(1 << i);
			t=0;
			while (t<velocidad){
				if (FLAG) return;
				_delay_ms(0.5);
				if (!(pollPB2())){
					stop=0;
					return;
				}
				if (!(pollPB3())){
					stop=0;
					return;
				}
				t++;
			}	
			}
		}
	}
}
void sec3() {
	int stop=1; int t;
	while(stop){
		t=0;
		PORTA = 0x21;
		while (t<velocidad){
			if (FLAG) return;
			_delay_ms(0.5);
			if (!(pollPB2())){
				stop=0;
				return;
			}
			if (!(pollPB3())){
				stop=0;
				return;
			}
			t++;
		}
		t=0;
		if (stop==1){
			PORTA = 0x12;
			while (t<velocidad){
				if (FLAG) return;
				_delay_ms(0.5);
				if (!(pollPB2())){
					stop=0;
					return;
				}
				if (!(pollPB3())){
					stop=0;
					return;
				}
				t++;
			}
		}
			t=0;
			if (stop==1){
				PORTA = 0xC;
				while (t<velocidad){
					if (FLAG) return;
					_delay_ms(0.5);
					if (!(pollPB2())){
						stop=0;
						return;
					}
					if (!(pollPB3())){
						stop=0;
						return;
					}
					t++;
				}
			}
				t=0;
				if (stop==1){
					PORTA = 0x12;
					while (t<velocidad){
						if (FLAG) return;
						_delay_ms(0.5);
						if (!(pollPB2())){
							stop=0;
							return;
						}
						if (!(pollPB3())){
							stop=0;
							return;
						}
						t++;
					}
				}
	}
}
void sec4(){
	int t; int stop=1;
	while (stop){
		for (int i = 0; i < 6; i++){
			PORTA |= (1 << i);
			t=0;
			while (t<velocidad){
				if (FLAG) return;
				_delay_ms(0.5);
				if (!(pollPB2())){
					stop=0;
					return;
				}
				if (!(pollPB3())){
					stop=0;
					return;
				}
				t++;
			}
		}
		if (stop==1) PORTA=0x00;
		_delay_ms(100);
	}
}