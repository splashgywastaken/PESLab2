#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

// Макрос, устанавливающий значение для бита BIT на порту PORT = 0
#define CLEAR_BIT(PORT, BIT) (PORT &= ~(1 << BIT))
// Макрос, устанавливающий значение для бита BIT на порту PORT = 1
#define SET_BIT(PORT, BIT) (PORT |= (1 << BIT))
// Переключает сигнал для порта 
#define SWITCHSIGNAL(PORTX, PORTXN) ((PORTX) ^= (1 << (PORTXN)))
// Проверяет сигнал порта на пустоту бита
#define CHECKSIGNAL(PINX, BUTTON) ((PINX & (1 << BUTTON)) == 0)

volatile uint8_t is_listening = 0;
volatile uint8_t seconds = 0;
volatile uint8_t signal_count = 0;

// Обработчик прерывания PCINT0
ISR(PCINT0_vect)
{
	// Проверка и работа с кнопкой переключения режимов работы двигателя
	if (CHECKSIGNAL(PINB, RBUTTON))
	{
		SWITCHSIGNAL(PORTB, FRELAY);
		SWITCHSIGNAL(PORTB, RRELAY);
		_delay_ms(50);
	}

	// Проверка и работа с кнопкой включения и выключения двигателя
	if (CHECKSIGNAL(PINB, ONOFFBUTTON))
	{
		SWITCHSIGNAL(PORTB, ONOFFRELAY);
		_delay_ms(50);
	}
}

ISR(TIMER0_OVF_vect)
{
    // увеличиваем счетчик секунд
    seconds++;
    if (is_listening == 1 && seconds > 2)
    {
        if (signal_count == 1)
        {
			// Старт 1
			SET_BIT(PORTB, ONOFFRELAY);
        }
        if (signal_count == 2)
        {
			// Стоп 5
			CLEAR_BIT(PORTB, ONOFFRELAY);
        }
        if (signal_count == 3)
        {
			// Изменить направление 21
			SWITCHSIGNAL(PORTB, FRELAY);
			SWITCHSIGNAL(PORTB, RRELAY);
        }
        is_listening = 0;
        signal_count = 0;
    }
}

// Основное тело программы
int main(void)
{	
	// установка входного режима 
	// на портах ONOFFBUTTON, RBUTTON, INPUT1
	DDRB &= ~(
		(1 << ONOFFBUTTON) | 
		(1 << RBUTTON) | 
		(1 << SERIAL_INPUT)
		);
	// включение подтягивающих резисторов 
	// на портах ONOFFBUTTON, RBUTTON, INPUT1
	PORTB |= 
		(1 << ONOFFBUTTON) |
		(1 << RBUTTON) |
		(1 << SERIAL_INPUT);  

	// Пины для реле
	// Пин для движения вперед
	// Пин для движения назад
	// Пин для вкл/выкл двигателя
	DDRB |= (
		(1 << FRELAY) |
		(1 << RRELAY) |
		(1 << ONOFFRELAY)
		  );
	PORTB |= ((1 << FRELAY) | (1 << ONOFFRELAY));

	// Настройка таймера
	TCCR0B |= (1 << CS02) | (1 << CS00); // устанавливаем предделитель 1024
    TIMSK |= (1 << TOIE0);
    TCNT0 = 0; // устанавливаем начальное значение счетчика

	// // Настройка прерываний
	// GIMSK |= (1<<PCIE); // Разрешаем внешние прерывания PCINT0.
	// PCMSK |= (1<<ONOFFBUTTON) | (1<<RBUTTON); // Разрешаем по маске прерывания
	sei(); // Разрешаем прерывания глобально: SREG |= (1<<SREG_I)

	while (1)
	{
		// Проверка и работа с кнопкой переключения режимов работы двигателя
		if (CHECKSIGNAL(PINB, RBUTTON))
		{
			SWITCHSIGNAL(PORTB, FRELAY);
			_delay_ms(100);
			SWITCHSIGNAL(PORTB, RRELAY);
			_delay_ms(1000);
		}

		// Проверка и работа с кнопкой включения и выключения двигателя
		if (CHECKSIGNAL(PINB, ONOFFBUTTON))
		{
			SWITCHSIGNAL(PORTB, ONOFFRELAY);
			_delay_ms(1000);
		}

		if (CHECKSIGNAL(PINB, SERIAL_INPUT))
		{
			if (is_listening == 0)
			{
				is_listening = 1;
				seconds = 0;
				while (is_listening == 1 && CHECKSIGNAL(PINB, SERIAL_INPUT))
				{

				}
			} else 
			{
				signal_count++;
				while (is_listening == 1 && CHECKSIGNAL(PINB, SERIAL_INPUT))
				{

				}
			}
		}
	}
}