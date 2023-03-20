#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "../config.h"

// Проверяет нажата ли данная кнопка 
#define CHECKBUTTON(PINX, BUTTONX) ((PINX & (1 << BUTTONX)) == 0)
#define SWITCHSIGNAL(PORTX, PORTXN) ((PORTX) ^= (1 << (PORTXN)))

// Кнопка переключения режима работы двигателя
#define RBUTTON PORTD3
// Кнопка включения/выключения (стоп/старт)
#define ONOFFBUTTON PORTD2
// Реле для движения вперед
#define FRELAY PORTC0
// Реле для движения назад
#define RRELAY PORTC1
// Реле для включения/выключения двигателя
#define ONOFFRELAY PORTC2

static bool is_in_reverse = false;
static bool is_off = true;

// // Функция переключающая сигнал на противоположный 
// // для определенной группы портов portx, определенного порта portxn
// static void switch_signal(int portx, int portxn)
// {
// 	(portx) ^= (1 << (portxn));
// }

// Обработчик прерывания PCINT0
ISR(PCINT0_vect)
{
	// Проверка и работа с кнопкой переключения режимов работы двигателя
	if (CHECKBUTTON(PIND, RBUTTON))
	{
		// Переключение сигнала для реле отвечающего за движение вперед
		SWITCHSIGNAL(PORTC, FRELAY);
		// Переключение сигнала для реле отвечающего за движение назад
		SWITCHSIGNAL(PORTC, RRELAY);
		// Смена чека
		is_in_reverse != is_in_reverse;
	}

	// Проверка и работа с кнопкой включения и выключения двигателя
	if (CHECKBUTTON(PIND, RBUTTON))
	{
		// Переключение сигнала для реле отвечающего за включение и выключение двигателя
		SWITCHSIGNAL(PORTC, ONOFFRELAY);
		// Смена чека
		is_off != is_off;
	}

	_delay_ms(50);					// антидребезг
}

// Основное тело программы
int main(void)
{
	// Подтягивание пинов для кнопок
	// Кнопка вкл/выкл двиг.
	DDRD &= ~(1 << ONOFFBUTTON);
	PORTD |= (1 << ONOFFBUTTON);
	// Кнопка переключения рев. двиг.
	DDRD &= ~(1 << RBUTTON);
	PORTD |= (1 << RBUTTON);
	
	// Пины для реле
	// Пин для движения вперед
	DDRC |= (1 << FRELAY);
	PORTC &= ~(1 << FRELAY);
	// Пин для движения назад
	DDRC |= (1 << RRELAY);
	PORTC &= ~(1 << RRELAY);
	// Пин для вкл/выкл двигателя
	DDRC |= (1 << ONOFFRELAY);
	PORTC &= ~(1 << ONOFFRELAY);

	// Настройка прерываний
	// GIMSK |= (1 << PCIE);		 	// Разрешаем прерывания PCINT0 
	// PCMSK |= (1 << ONOFFBUTTON); 	// Разрешаем по маске прерывания на ногах кнопки
	// PCMSK |= (1 << RBUTTON);     	// Разрешаем по маске прерывания на ногах кнопки

	SREG |= (1 << 7);
	GICR |= (1 << 6);
	MCUCR |= (1 << 1);

	sei();					 		// Разрешаем прерывания глобально
	while (1)
	{
	}
}