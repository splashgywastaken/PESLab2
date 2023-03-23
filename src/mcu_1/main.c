#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

// Переключает сигнал для порта 
#define SWITCHSIGNAL(PORTX, PORTXN) ((PORTX) ^= (1 << (PORTXN)))
// Проверяет нажата ли данная кнопка (пуст ли бит)
#define CHECKBUTTON(PINX, BUTTON) ((PINX & (1 << BUTTON)) == 0)
// Кнопка переключения режима работы двигателя
#define RBUTTON PORTB0
// Кнопка включения/выключения (стоп/старт)
#define ONOFFBUTTON PORTB1
// Реле для движения вперед
#define FRELAY PORTA0
// Реле для движения назад
#define RRELAY PORTA1
// Реле для включения/выключения двигателя
#define ONOFFRELAY PORTA2

// static uint8_t is_moving_backwards = 0;
// static uint8_t is_on = 1;

// Обработчик прерывания PCINT0
ISR(PCINT0_vect)
{
	// Проверка и работа с кнопкой переключения режимов работы двигателя
	if (CHECKBUTTON(PINB, RBUTTON))
	{
		SWITCHSIGNAL(PORTA, FRELAY);
		SWITCHSIGNAL(PORTA, RRELAY);
		_delay_ms(50);

		// if (is_moving_backwards == 0)
		// {
		// 	// SWITCHSIGNAL(PORTA, FRELAY);
		// 	// is_moving_backwards = 1;

		// 	PORTA &= ~(1 << FRELAY);
		// 	PORTA |= (1 << RRELAY);
		// 	is_moving_backwards = 1;
		// } else 
		// {
		// 	// SWITCHSIGNAL(PORTA, FRELAY);
		// 	// is_moving_backwards = 0;

		// 	PORTA |= (1 << FRELAY);
		// 	PORTA &= ~(1 << RRELAY);
		// 	is_moving_backwards = 0;
		// }
		// _delay_ms(50);	// антидребезг
	}

	// Проверка и работа с кнопкой включения и выключения двигателя
	if (CHECKBUTTON(PINB, ONOFFBUTTON))
	{
		SWITCHSIGNAL(PORTA, ONOFFRELAY);
		_delay_ms(50);

		// if (is_on == 0)
		// {
		// 	PORTA |= (1 << ONOFFRELAY);
		// 	is_on = 1;
		// } else 
		// {
		// 	PORTA &= ~(1 << ONOFFRELAY);
		// 	is_on = 0;	
		// }
		// _delay_ms(50);
	}
}

// Основное тело программы
int main(void)
{
	// Подтягивание пинов для кнопок
	// Кнопка вкл/выкл двиг.
	// Кнопка переключения рев. двиг.
	// DDRB &= ~((1 << ONOFFBUTTON) | (1 << RBUTTON));
	// PORTB |= ((1 << ONOFFBUTTON) | (1 << RBUTTON));
	
	DDRB &= ~((1 << PB0) | (1 << PB1));  // установка входного режима на портах B0 и B1
	PORTB |= (1 << PB0) | (1 << PB1);   // включение подтягивающих резисторов на портах B0 и B1


	// Пины для реле
	// Пин для движения вперед
	// Пин для движения назад
	// Пин для вкл/выкл двигателя
	DDRA |= ((1 << FRELAY) | (1 << RRELAY) | (1 << ONOFFRELAY));
	PORTA |= ((1 << FRELAY) | (1 << ONOFFRELAY));

	// Настройка прерываний
	PCMSK0 |= (1 << PCINT0) | (1 << PCINT1);  // разрешение прерываний на портах B0 и B1
	GIMSK |= (1 << PCIE0);                   // разрешение прерываний по изменению состояния на портах B0-B3
	// Разрешаем прерывания глобально
	sei();

	while (1)
	{
		// Проверка и работа с кнопкой переключения режимов работы двигателя
		if (CHECKBUTTON(PINB, RBUTTON))
		{
			SWITCHSIGNAL(PORTA, FRELAY);
			_delay_ms(100);
			SWITCHSIGNAL(PORTA, RRELAY);
			_delay_ms(500);

			// if (is_moving_backwards == 0)
			// {
			// 	// SWITCHSIGNAL(PORTA, FRELAY);
			// 	// is_moving_backwards = 1;

			// 	PORTA &= ~(1 << FRELAY);
			// 	PORTA |= (1 << RRELAY);
			// 	is_moving_backwards = 1;
			// } else 
			// {
			// 	// SWITCHSIGNAL(PORTA, FRELAY);
			// 	// is_moving_backwards = 0;

			// 	PORTA |= (1 << FRELAY);
			// 	PORTA &= ~(1 << RRELAY);
			// 	is_moving_backwards = 0;
			// }
			// _delay_ms(50);	// антидребезг
		}

		// Проверка и работа с кнопкой включения и выключения двигателя
		if (CHECKBUTTON(PINB, ONOFFBUTTON))
		{
			SWITCHSIGNAL(PORTA, ONOFFRELAY);
			_delay_ms(500);

			// if (is_on == 0)
			// {
			// 	PORTA |= (1 << ONOFFRELAY);
			// 	is_on = 1;
			// } else 
			// {
			// 	PORTA &= ~(1 << ONOFFRELAY);
			// 	is_on = 0;	
			// }
			// _delay_ms(50);
		}
	}
}