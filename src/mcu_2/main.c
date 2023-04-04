#include <stdbool.h>
#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>


const char DIGITS[] = {DIG1, DIG2, DIG3};
const uint32_t DIGITS_COUNT = 3;
const char SEGS[] = {SEG_A, SEG_B, SEG_C, SEG_D, SEG_E, SEG_F, SEG_G};
const uint32_t SEGS_COUNT = 7;

#define SPACE 10    // char-код для пробела
#define E 11        // char-код для буквы E - используется обозначения числа непредвиденно большой разрядности

// Строка символов для вывода на индикатор.
// Здесь используются не ASCII коды, а обозначения
// символов: (0-9) - это цифры, 10 (SPACE) - пробел, 11 (E) - для чисел слишком большой разрядности
// (код 0 - символ '0' и т.д.).
// Для вывода данных на индикатор следует просто
// поместить коды выводимых символов в эту строку.
// Установка старшего бита отображает точку вместе с символом.
char OUT_STR[]={SPACE, SPACE, SPACE}; 

bool set_OUTSTR (int n) {

    //отображение отрицательных чисел не предусмотрено
    if (n < 0) n = -n;

    char d = 0;
    for (uint32_t i=0; i<DIGITS_COUNT; i++) {
        if (n != 0) {
            d = n % 10;
            n /= 10;
        }
        else if (i == 0 && n == 0)
            d = 0;
        else d = SPACE;
        OUT_STR[DIGITS_COUNT -1 - i] = d;
        d = 0;
    }
    if (n != 0) // число разрядности большей, чем DIGITS_COUNT
    {
        for (uint32_t i=0; i<DIGITS_COUNT; i++) {
            OUT_STR[i] = E;
        }
        return false;
    }
    return true;
}

// выключение всех разрядов - на пинах разрядов ставим единичку
void digs_off() {
    for(uint32_t i=0; i<DIGITS_COUNT; i++) {
        PORTB |= (1 << DIGITS[i]);
    }
}

// Включить разряд ind, индексация начинается с 0 - 
// ind=0 соответствует крайнему левому разряду
void dig_on(uint32_t ind) {
    PORTB &= ~(1 << DIGITS[ind]);
}

// изменить состояние сегмента индикатора с индексом ind на вкл
// сегмент A: ind=0
// сегмент B: ind=1
// сегмент C: ind=2
// сегмент D: ind=3
// сегмент E: ind=4
// сегмент F: ind=5
// сегмент G: ind=6
void seg_on (uint32_t ind) {
    PORTA |= (1 << SEGS[ind]);
} 


// изменить состояние сегментов индикатора с заданными индексами на вкл
// сегмент A: ind=0
// сегмент B: ind=1
// сегмент C: ind=2
// сегмент D: ind=3
// сегмент E: ind=4
// сегмент F: ind=5
// сегмент G: ind=6
void segs_on(uint32_t* inds, uint32_t inds_num) {
    for (uint32_t i = 0; i < inds_num; i++) {
        PORTA |= (1 << SEGS[inds[i]]);
    }
}

// изменить состояние сегмента индикатора с индексом ind на выключен
// сегмент A: ind=0
// сегмент B: ind=1
// сегмент C: ind=2
// сегмент D: ind=3
// сегмент E: ind=4
// сегмент F: ind=5
// сегмент G: ind=6
void seg_off (uint32_t ind) {
    PORTA &= ~(1 << SEGS[ind]);
} 

void segs_off () {
    for (uint32_t i = 0; i < SEGS_COUNT; i++)
        PORTA &= ~(1 << SEGS[i]);
} 

// Вывод на шину управления сегментами символа с кодом digit;
void set_seg_bus(char digit)
{
    segs_off();
    uint32_t* segs_inds;
    switch (digit) {
        case 0: 
            segs_inds = (uint32_t[6]){0, 1, 2, 3, 4, 5};
            segs_on(segs_inds, 6);
            break;
        case 1:
            segs_inds = (uint32_t[2]){1, 2};
            segs_on(segs_inds, 2);
            break;
        case 2: 
            segs_inds = (uint32_t[5]){0, 1, 6, 4, 3};
            segs_on(segs_inds, 5);
            break;
        case 3:
            segs_inds = (uint32_t[5]){0, 1, 2, 3, 6};
            segs_on(segs_inds, 5);
            break;
        case 4:
            segs_inds = (uint32_t[4]){1, 2, 5, 6};
            segs_on(segs_inds, 4);
            break;
        case 5:
            segs_inds = (uint32_t[5]){0, 2, 3, 5, 6};
            segs_on(segs_inds, 5);
            break;
        case 6: 
            segs_inds = (uint32_t[6]){0, 2, 3, 4, 5, 6};
            segs_on(segs_inds, 6);
            break;
        case 7:
            segs_inds = (uint32_t[3]){0, 1, 2};
            segs_on(segs_inds, 3);
            break;
        case 8: 
            segs_inds = (uint32_t[7]){0, 1, 2, 3, 4, 5, 6};
            segs_on(segs_inds, 7);
            break;
        case 9: 
            segs_inds = (uint32_t[6]){0, 1, 2, 3, 5, 6};
            segs_on(segs_inds, 6);
            break;
        case SPACE: 
            break;
        case E: 
            segs_inds = (uint32_t[5]){0, 3, 4, 5, 6};
            segs_on(segs_inds, 5);
            break;
        default:
            seg_on(6);
    }
}

// мгновенное отображение на экране строки OUT_STR
void display() {
    for (uint32_t i = 0; i < DIGITS_COUNT; i++) {
        digs_off();
        dig_on(i);
        set_seg_bus(OUT_STR[i]);
        _delay_ms(30);
    }
}

void pins_setup () {

    // настройка пинов сегментов индикаторов и разрядов как выходных
    for (uint32_t i = 0; i < SEGS_COUNT; i++)
        DDRA |= (1 << SEGS[i]);
    for (uint32_t i=0; i < DIGITS_COUNT; i++)
        DDRB |= (1 << DIGITS[i]);
    digs_off();
    // входные порты
    DDRB &= ~ (1 << INPORT50);
    PORTB |= (1 << INPORT50); // подтянут
    DDRA &= ~ (1 << INPORT75);
    PORTA |= (1 << INPORT75); // подтянут
}

int main(void)
{
    pins_setup();
    _delay_ms(50);
    while (1)
    {
        display();
        // если двигатель работает на 50% от максимальной скорости
        if ((PINB & (1 << INPORT50)) != 0 && (PINA & (1 << INPORT75)) == 0) 
        {
            set_OUTSTR(50);
        }
        // если двигатель работает на 75% от максимальной скорости
        else if ((PINB & (1 << INPORT50)) == 0 && (PINA & (1 << INPORT75)) != 0) 
        {
            set_OUTSTR(75);
        }
        // если двигатель работает на 100% от максимальной скорости
        else if ((PINB & (1 << INPORT50)) != 0 && (PINA & (1 << INPORT75)) != 0)
        {
            set_OUTSTR(100);
        }
        else
        {
            set_OUTSTR(0);
        }
        _delay_ms(10);
    }
}