/*
  pins_arduino.h - Pin definition functions for Arduino
  Part of Arduino - http://www.arduino.cc/

  Copyright (c) 2007 David A. Mellis

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General
  Public License along with this library; if not, write to the
  Free Software Foundation, Inc., 59 Temple Place, Suite 330,
  Boston, MA  02111-1307  USA

  $Id: wiring.h 249 2007-02-03 16:52:51Z mellis $
*/

#ifndef Pins_Arduino_h
#define Pins_Arduino_h

#include <avr/pgmspace.h>

#define NUM_DIGITAL_PINS            32
#define NUM_ANALOG_INPUTS           8
#define analogInputToDigitalPin(p)  ((p < 8) ? (p) + 24 : -1)

#define digitalPinHasPWM(p)         ((p) == 4 || (p) == 5 || (p) == 7 || (p) == 11)

static const uint8_t SS   = 12;
static const uint8_t MOSI = 13;
static const uint8_t MISO = 14;
static const uint8_t SCK  = 15;

static const uint8_t SDA = 17;
static const uint8_t SCL = 16;
static const uint8_t LED_BUILTIN = 13;

static const uint8_t A0 = 24;
static const uint8_t A1 = 25;
static const uint8_t A2 = 26;
static const uint8_t A3 = 27;
static const uint8_t A4 = 28;
static const uint8_t A5 = 29;
static const uint8_t A6 = 30;
static const uint8_t A7 = 31;

#define digitalPinToPCICR(p)    ((uint8_t *)0)
#define digitalPinToPCICRbit(p) (0)
#define digitalPinToPCMSK(p)    ((uint8_t *)0)
#define digitalPinToPCMSKbit(p) (0)

#ifdef ARDUINO_MAIN

// On the Arduino board, digital pins are also used
// for the analog output (software PWM).  Analog input
// pins are a separate set.

// ATMEL ATMEGA16A / ARDUINO
//
//                  +---\/---+
//     (D  8) PB0  1|        |40 PA0 (D 24) (AI 0)
//     (D  9) PB1  2|        |39 PA1 (D 25) (AI 1)
//     (D 10) PB2  3|        |38 PA2 (D 26) (AI 2)
// PWM (D 11) PB3  4|        |37 PA3 (D 27) (AI 3)
//     (D 12) PB4  5|        |36 PA4 (D 28) (AI 4)
//     (D 13) PB5  6|        |35 PA5 (D 29) (AI 5)
//     (D 14) PB6  7|        |34 PA6 (D 30) (AI 6)
//     (D 15) PB7  8|        |33 PA7 (D 31) (AI 7)
//          RESET  9|        |32 AREF
//            VCC 10|        |31 GND
//            GND 11|        |30 AVCC
//          XTAL2 12|        |29 PC7 (D 23)
//          XTAL1 13|        |28 PC6 (D 22)
//     (D  0) PD0 14|        |27 PC5 (D 21)
//     (D  1) PD1 15|        |26 PC4 (D 20)
//     (D  2) PD2 16|        |25 PC3 (D 19)
//     (D  3) PD3 17|        |24 PC2 (D 18)
//  PWM(D  4) PD4 18|        |23 PC1 (D 17)
//  PWM(D  5) PD5 19|        |22 PC0 (D 16)
//     (D  6) PD6 20|        |21 PD7 (D  7)  PWM
//                  +--------+
//
// D0       PD0           RXD
// D1       PD1           TXD
// D2       PD2           INT0
// D3       PD3           INT1
// D4       PD4           OC1B
// D5       PD5           OC1A
// D6       PD6           ICP1
// D7       PD7           OC2
// D8       PB0           XCK/T0
// D9       PB1           T1
// D10      PB2           INT2/AIN0
// D11      PB3           OC0/AIN1
// D12      PB4           SS#
// D13      PB5           MOSI
//
// A0  D24  PA0           ADC0
// A1  D25  PA1           ADC1
// A2  D26  PA2           ADC2
// A3  D27  PA3           ADC3
// A4  D28  PA4           ADC4
// A5  D29  PA5           ADC5
// A6  D30  PA6           ADC6
// A7  D31  PA7           ADC7
//
// add pins
//
// D14      PB6           MISO
// D15      PB7           SCK
// D16      PC0           SCL
// D17      PC1           SDA
// D18      PC2           TCK
// D19      PC3           TMS
// D20      PC4           TDO
// D21      PC5           TDI
// D22      PC6           TOSC1
// D23      PC7           TOSC2

// these arrays map port names (e.g. port B) to the
// appropriate addresses for various functions (e.g. reading
// and writing)
const uint16_t PROGMEM port_to_mode_PGM[] = {
	NOT_A_PORT,
	(uint16_t) &DDRA,
	(uint16_t) &DDRB,
	(uint16_t) &DDRC,
	(uint16_t) &DDRD,
};

const uint16_t PROGMEM port_to_output_PGM[] = {
	NOT_A_PORT,
	(uint16_t) &PORTA,
	(uint16_t) &PORTB,
	(uint16_t) &PORTC,
	(uint16_t) &PORTD,
};

const uint16_t PROGMEM port_to_input_PGM[] = {
	NOT_A_PORT,
	(uint16_t) &PINA,
	(uint16_t) &PINB,
	(uint16_t) &PINC,
	(uint16_t) &PIND,
};

const uint8_t PROGMEM digital_pin_to_port_PGM[] = {
	PD, /* 0 */
	PD,
	PD,
	PD,
	PD,
	PD,
	PD,
	PD,
	PB, /* 8 */
	PB,
	PB,
	PB,
	PB,
	PB,
	PB,
	PB,
	PC, /* 16 */
	PC,
	PC,
	PC,
	PC,
	PC,
	PC,
	PC,

	PA, /* 24 */
	PA,
	PA,
	PA,
	PA,
	PA,
	PA,
	PA
};

const uint8_t PROGMEM digital_pin_to_bit_mask_PGM[] = {
	_BV(0), /* 0, port D */
	_BV(1),
	_BV(2),
	_BV(3),
	_BV(4),
	_BV(5),
	_BV(6),
	_BV(7),
	_BV(0), /* 8, port B */
	_BV(1),
	_BV(2),
	_BV(3),
	_BV(4),
	_BV(5),
	_BV(6),
	_BV(7),
	_BV(0), /* 16, port C */
	_BV(1),
	_BV(2),
	_BV(3),
	_BV(4),
	_BV(5),
	_BV(6),
	_BV(7),
#if 0
	_BV(6), /* 24, port A */
	_BV(7),
	_BV(0),
	_BV(1),
	_BV(2),
	_BV(3),
	_BV(4),
	_BV(5)
#endif
	_BV(0), /* 24, port A */
	_BV(1),
	_BV(2),
	_BV(3),
	_BV(4),
	_BV(5),
	_BV(6),
	_BV(7)
};

const uint8_t PROGMEM digital_pin_to_timer_PGM[] = {
	NOT_ON_TIMER, /* 0 - port D */
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	TIMER1B,      /* 4 - Port D */
	TIMER1A,      /* 5 - Port D */
	NOT_ON_TIMER,
	TIMER2,       /* 7 - Port D */
	NOT_ON_TIMER, /* 8 */
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	TIMER0A,      /* 11 - Port B */
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER, /* 16 */
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER, /* 24 */
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER
};

#endif

#endif
