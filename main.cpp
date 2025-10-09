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

unsigned char keypad_tbl2[] = {
	'1','2','3','A',
	'4','5','6','B',
	'7','8','9','C',
	'*','0','#','D'
};

void UART_init(unsigned int ubrr);
void UART_send_char(unsigned char data);
void UART_send_string(const char* str);
void UART_send_int(int data);
unsigned char read_keypad(void);

int main(void)
{
	unsigned char key_pressed = 0;
	UART_init(MYUBRR);

	UART_send_string("MYUBRR= ");
	UART_send_int(MYUBRR);
	UART_send_string("\nF_CPU = ");
	UART_send_int((int)(F_CPU / 1000000));
	UART_send_string("\n\nPress a key:\n");

	while (1)
	{
		key_pressed = read_keypad();
		if (key_pressed != 0)
		{
			_delay_ms(200);  // Debounce
			UART_send_char(keypad_tbl2[key_pressed - 1]);
			key_pressed = 0;
		}
	}
	return 0;
}

unsigned char read_keypad()
{
	const unsigned char row_pins[4] = {0,1,2,3}; // PB0-PB3
	const unsigned char col_pins[4] = {4,5,6,7}; // PD4-PD7
	unsigned char r, c;

	KEYPAD_DDR_ROWS &= 0xF0;  // Rows input
	KEYPAD_PORT_ROWS |= 0x0F; // Pull-ups
	KEYPAD_DDR_COLS |= 0xF0;  // Columns output
	KEYPAD_PORT_COLS |= 0xF0; // Default HIGH

	for (c = 0; c < 4; c++)
	{
		KEYPAD_PORT_COLS |= 0xF0;                // Reset all HIGH
		KEYPAD_PORT_COLS &= ~(1 << col_pins[c]); // Drive one LOW
		_delay_us(5);

		for (r = 0; r < 4; r++)
		{
			if (!(KEYPAD_PINS_ROWS & (1 << row_pins[r])))
			{
				return r * 4 + c + 1;
			}
		}
	}
	return 0;
}

void UART_init(unsigned int ubrr)
{
	UBRR0H = (unsigned char)(ubrr >> 8);
	UBRR0L = (unsigned char)ubrr;
	UCSR0A |= (1 << U2X0);          // Double speed
	UCSR0B = (1 << RXEN0) | (1 << TXEN0);
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); // 8-bit data
}

void UART_send_char(unsigned char data)
{
	while (!(UCSR0A & (1 << UDRE0)));
	UDR0 = data;
}

void UART_send_string(const char* str)
{
	while (*str)
	UART_send_char(*str++);
}

void UART_send_int(int data)
{
	char buffer[10];
	itoa(data, buffer, 10);
	UART_send_string(buffer);
}
