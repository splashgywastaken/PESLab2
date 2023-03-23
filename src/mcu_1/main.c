#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "../config.h"

// Проверяет нажата ли данная кнопка 
#define CHECKBUTTON(PINX, BUTTONX) ((PINX & (1 << BUTTONX)) == 0)
#define SWITCHSIGNAL(PORTX, PORTXN) ((PORTX) ^= (1 << (PORTXN)))

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

// Обработчик прерывания PCINT0
ISR(PCINT0_vect)
{
	// Проверка и работа с кнопкой переключения режимов работы двигателя
	if (CHECKBUTTON(PINB, RBUTTON))
	{
		// Переключение сигнала для реле отвечающего за движение вперед
		SWITCHSIGNAL(PORTA, FRELAY);
		// Переключение сигнала для реле отвечающего за движение назад
		SWITCHSIGNAL(PORTA, RRELAY);
	}

	// Проверка и работа с кнопкой включения и выключения двигателя
	if (CHECKBUTTON(PINB, RBUTTON))
	{
		// Переключение сигнала для реле отвечающего за включение и выключение двигателя
		SWITCHSIGNAL(PORTA, ONOFFRELAY);
	}

	_delay_ms(50);					// антидребезг
}

// Основное тело программы
int main(void)
{
	// Подтягивание пинов для кнопок
	// Кнопка вкл/выкл двиг.
	DDRB &= ~(1 << ONOFFBUTTON);
	// Кнопка переключения рев. двиг.
	DDRB &= ~(1 << RBUTTON);
	PORTB = 0;
	
	// Пины для реле
	// Пин для движения вперед
	DDRA |= (1 << FRELAY);
	PORTA &= ~(1 << FRELAY);
	// Пин для движения назад
	DDRA |= (1 << RRELAY);
	PORTA &= ~(1 << RRELAY);
	// Пин для вкл/выкл двигателя
	DDRA |= (1 << ONOFFRELAY);
	PORTA &= ~(1 << ONOFFRELAY);
	PORTA = 0;

	// Настройка прерываний
	GIMSK |= (1 << PCIE1); // Разрешаем внешние прерывания.
	PCMSK1 |= (1 << ONOFFBUTTON);
	PCMSK1 |= (1 << RBUTTON); // Разрешаем по маске прерывания на ногак кнопок

	sei();					 		// Разрешаем прерывания глобально
	while (1)
	{
	}
}