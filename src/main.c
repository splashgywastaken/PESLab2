// #define F_CPU 8000000

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

// Обработчик прерывания PCINT0
ISR(PCINT0_vect)
{
	_delay_ms(50);					// антидребезг
}

int main(void)
{
	sei();					 		// Разрешаем прерывания глобально
	while (1)
	{
	}
}