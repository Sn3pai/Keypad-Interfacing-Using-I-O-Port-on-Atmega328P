/*
 * Lab6Task1.cpp
 * Created: 07/10/2025 10:27:45 pm
 * Author : Usman
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include "debug_prints.c"

#define BAUD 115200
#define MYUBRR ((F_CPU / (8UL * BAUD)) - 1)  // Double speed mode

#define KEYPAD_DDR_ROWS DDRB
#define KEYPAD_PORT_ROWS PORTB
#define KEYPAD_PINS_ROWS PINB
#define KEYPAD_DDR_COLS DDRD
#define KEYPAD_PORT_COLS PORTD
#define KEYPAD_PINS_COLS PIND

// Keypad character mapping
unsigned char keypad_tbl2[16] = {
	'7','8','9','/',    // Row A
	'4','5','6','*',    // Row B
	'1','2','3','-',    // Row C
	'C','0','=','+'      // Row D
};

void UART_init(unsigned int ubrr);
void UART_send_char(unsigned char data);
void UART_send_string(const char* str);
void UART_send_int(int data);
unsigned char read_keypad(void);

int main(void)
{
    UART_init(MYUBRR);

    UART_send_string("MYUBRR= ");
    UART_send_int(MYUBRR);
    UART_send_string("\nF_CPU = ");
    UART_send_int((int)(F_CPU / 1000000));
    UART_send_string("\n\nPress a key:\n");

    while (1)
    {
        unsigned char key_index = read_keypad();
        if (key_index != 255)
        {
            UART_send_char(keypad_tbl2[key_index]);
        }
    }
}

// Read 4x4 keypad
unsigned char read_keypad()
{
    const unsigned char row_pins[4] = {0,1,2,3}; // PB0-PB3
    const unsigned char col_pins[4] = {4,5,6,7}; // PD4-PD7
    unsigned char r, c;

    // Set rows as input with pull-ups
    KEYPAD_DDR_ROWS &= 0xF0;
    KEYPAD_PORT_ROWS |= 0x0F;

    // Set columns as outputs HIGH
    KEYPAD_DDR_COLS |= 0xF0;
    KEYPAD_PORT_COLS |= 0xF0;

    for (c = 0; c < 4; c++)
    {
        KEYPAD_PORT_COLS |= 0xF0;                 // Reset all HIGH
        KEYPAD_PORT_COLS &= ~(1 << col_pins[c]); // Drive one column LOW
        _delay_us(5);

        for (r = 0; r < 4; r++)
        {
            if (!(KEYPAD_PINS_ROWS & (1 << row_pins[r])))
            {
                // Wait until key is released (debounce)
                while (!(KEYPAD_PINS_ROWS & (1 << row_pins[r])))
                {
                    _delay_ms(10);
                }
                return r * 4 + c; // Return key index 0-15
            }
        }
    }
    return 255; // No key pressed
}

// UART initialization
void UART_init(unsigned int ubrr)
{
    UBRR0H = (unsigned char)(ubrr >> 8);
    UBRR0L = (unsigned char)ubrr;
    UCSR0A |= (1 << U2X0);          // Double speed
    UCSR0B = (1 << RXEN0) | (1 << TXEN0);
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); // 8-bit data, 1 stop bit
}

// Send a single character
void UART_send_char(unsigned char data)
{
    while (!(UCSR0A & (1 << UDRE0)));
    UDR0 = data;
}

// Send a string
void UART_send_string(const char* str)
{
    while (*str)
        UART_send_char(*str++);
}

// Send integer as string
void UART_send_int(int data)
{
    char buffer[10];
    itoa(data, buffer, 10);
    UART_send_string(buffer);
}
