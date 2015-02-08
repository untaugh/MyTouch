/*
 *  mytouch.c
 * 
 * Copyright 2015 Oskari Rundgren <orundg@gmail.com>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */

/*  Testing capacitive sensing on AVR. 
 *  A 10nF Capacitor is connected between PD7 and PD6.
 *  The electrode is connected to PD7.
 *  I'm getting counter readings around 160 when not touched.
 *  And 155 when touched.
 *  Result is printed to serial at 9600 Baud. 
 * */


#include <avr/io.h>
#include <stdio.h>
#define F_CPU 16000000
#include <util/delay.h>

// A 10nF capacitor is connected between these pins. 
#define sense PD7
#define cap PD6


int usart_printchar(char var, FILE *stream)
{
    while ( !( UCSR0A & (1<<UDRE0)) );
    UDR0 = var;
    return 0;
}

static FILE mystdout = FDEV_SETUP_STREAM(usart_printchar, NULL, _FDEV_SETUP_WRITE); // For printf to serial. 

int main()
{

  /* USART init. */
  UBRR0H = 0;
  UBRR0L = 103;  // 103 = 9600 Baud. 
  UCSR0B |= (1<<TXEN0);
  stdout = &mystdout;  // printf to serial. 

  int counter;

  while(1)
  {
    DDRD |= (1<<sense) | (1<<cap);  // Set to out ports.
    PORTD &= ~(1<<sense) & ~(1<<cap);  // Set port to low. 
    _delay_ms(1); // Wait 1 ms. 

    counter = 0; 

    while(!(PIND & (1<<sense)))
    {
      DDRD &= ~(1<<cap);  // Cap to input. 
      PORTD |= (1<<sense); // Sense to high.
      _delay_us(10);
      DDRD &= ~(1<<sense); // Sense to input. 
      PORTD &= ~(1<<sense); // Sense low / no pullup. 
      DDRD |= (1<<cap); // Cap to low.
      counter++;
    }

    printf("Counter: %d\n",counter);
  }
}

