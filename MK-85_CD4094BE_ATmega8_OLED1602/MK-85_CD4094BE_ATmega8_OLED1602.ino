
/* information from
http://www.pisi.com.pl/piotr433/mk85hwe.htm
https://klapautsiy.github.io/The-display-for-Elektronika-MK-85/ */

#include <avr/power.h>
#include <LiquidCrystal.h>
// LiquidCrystal oled(RS,  E, D4, D5, D6, D7)
   LiquidCrystal oled( 4,  5,  0,  1,  6,  7);


volatile byte LCD_MK85[105];
volatile byte address = 0;
volatile byte data    = 0;

//=================================================

// BELL - LETC "4DSTSD4"

byte LETC[7] {
0b00000100,
0b00010110,
0b00000111,
0b00010111,
0b00000111,
0b00010110,
0b00000100
};


boolean BELL() {
for(byte i = 0; i < 7; i++) {
if (LCD_MK85[0x59 + i] != LETC[i]) return 0;
}
                                   return 1;
}


ISR(INT0_vect) {

address = (~PINB) - 0x80;
data    = (~PINC) & 0x1F;

if (address <= 0x68 /*0xE8 - 0x80*/ && LCD_MK85[address] != data) {

LCD_MK85[address] = data;

if (address == 0x5F && BELL() == 1) {
bitWrite(PORTC, 5, !(bitRead(PORTC, 5)));
tone( 3, 1000, 100);
}

}

}

//=================================================


void setup() {

pinMode(19, OUTPUT);
digitalWrite(19, 0);

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// графический режим OLED:
oled.command(0x08); // выключить экран
oled.command(0x1F); // переключиться в графический режим
oled.command(0x01); // очистить ОЗУ
oled.command(0x08|0x04); // включить экран

oled.begin(16, 2);

/*
// test OLED
setGraphicCursor(0, 0);
for(byte i = 0; i < 80; i++) oled.write(0b11111111);
setGraphicCursor(0, 1);
for(byte i = 0; i < 80; i++) oled.write(0b11111111);

// test BELL
bitWrite(PORTC, 5, !(bitRead(PORTC, 5)));
tone( 3, 1000, 100);

// test LED
digitalWrite(19, 1);
*/

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//===================================================

ADCSRA = 0; // disable ADC
power_all_disable(); // turn off various modules

//===================================================

/*
LOW     когда на порту LOW
CHANGE  при смене значения на порту
RISING  при смене значения на порту с LOW на HIGH
FALLING при смене значения на порту с HIGH на LOW
*/

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// MCUCR |= (1<<ISC00)|(1<<ISC01); /*INT0 RISING*/
MCUCR |= (1<<ISC01);            /*INT0 FALLING*/
// MCUCR |= (1<<ISC00);            /*INT0 CHANGE*/
GICR  |= (1<<INT0);             /*INT0 enabled*/
// GICR  &= ~(1<<INT0);            /*INT0 disabled*/
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

for(byte i = 0; i < 105; i++) LCD_MK85[i] = 0;

/*
      LCD_MK85[0xE8 - 0x80]  = 1;
while(LCD_MK85[0xE8 - 0x80] == 1);
*/

}


byte font_A[5] = {
0b00011110,
0b00000101,
0b00000101,
0b00000101,
0b00011110
};

byte font_D[5] = {
0b00011111,
0b00010001,
0b00010001,
0b00010001,
0b00001110
};

byte font_E[5] = {
0b00011111,
0b00010101,
0b00010101,
0b00010101,
0b00010001
};

byte font_F[5] = {
0b00011111,
0b00000101,
0b00000101,
0b00000101,
0b00000001
};

byte font_G[5] = {
0b00001110,
0b00010001,
0b00010101,
0b00010101,
0b00011101
};

byte font_M[5] = {
0b00011111,
0b00000010,
0b00000100,
0b00000010,
0b00011111
};

byte font_N[5] = {
0b00011111,
0b00000010,
0b00000100,
0b00001000,
0b00011111
};

byte font_R[5] = {
0b00011111,
0b00000101,
0b00000101,
0b00001101,
0b00010010,
};

byte font_S[5] = {
0b00010010,
0b00010101,
0b00010101,
0b00010101,
0b00001001
};

byte font_T[5] = {
0b00000001,
0b00000001,
0b00011111,
0b00000001,
0b00000001
};

byte font_U[5] = {
0b00001111,
0b00010000,
0b00010000,
0b00010000,
0b00001111
};

byte font_W[5] = {
0b00011111,
0b00001000,
0b00000100,
0b00001000,
0b00011111
};

byte font_X[5] = {
0b00010001,
0b00001010,
0b00000100,
0b00001010,
0b00010001
};

byte font_O[5] = {
0b00001110,
0b00010001,
0b00010001,
0b00010001,
0b00001110
};

byte font_P[5] = {
0b00011111,
0b00000101,
0b00000101,
0b00000101,
0b00000010
};

byte font_ [15] = {
0b00000000,
0b00000000,
0b00000000,
0b00000000,
0b00000000,

0b00000000,
0b00000000,
0b00000000,
0b00000000,
0b00000000,

0b00000000,
0b00000000,
0b00000000,
0b00000000,
0b00000000
};


void setGraphicCursor(byte x, byte y) {
oled.command(LCD_SETDDRAMADDR | x);
oled.command(LCD_SETCGRAMADDR | y);
}


void typewrite(byte array[], byte sizeof_array, byte x, byte y) {
setGraphicCursor(x, y);
for (byte i = 0; i < sizeof_array; i++) oled.write(array[i]);
}


boolean _EXT_ () {return bitRead(LCD_MK85[0x00], 0);}
boolean _S_   () {return bitRead(LCD_MK85[0x00], 1);}
boolean _F_   () {return bitRead(LCD_MK85[0x00], 2);}
boolean _RUN_ () {return bitRead(LCD_MK85[0x08], 0);}
boolean _WRT_ () {return bitRead(LCD_MK85[0x08], 1);}
boolean _DEG_ () {return bitRead(LCD_MK85[0x08], 4);}
boolean _RAD_ () {return bitRead(LCD_MK85[0x18], 0);}
boolean _GRA_ () {return bitRead(LCD_MK85[0x20], 0);}
boolean _TR_  () {return bitRead(LCD_MK85[0x28], 0);}
boolean _STOP_() {return bitRead(LCD_MK85[0x58], 3);}


void indicators() {

if (_EXT_ () == 0                                  ) {typewrite(font_ ,15,  0, 0);                                                                                       }
else                                                 {typewrite(font_E, 5,  0, 0); typewrite(font_X, 5,  5, 0); typewrite(font_T, 5, 10, 0);                             }

if (_S_   () == 0 && _F_   () == 0                 ) {typewrite(font_ , 5, 20, 0);                                                                                       }
else                                                 {
if (_S_   () == 1 && _F_   () == 1                 ) {typewrite(font_M, 5, 20, 0);                                                                                       }
if (_S_   () == 1 && _F_   () == 0                 ) {typewrite(font_S, 5, 20, 0);                                                                                       }
if (_S_   () == 0 && _F_   () == 1                 ) {typewrite(font_F, 5, 20, 0);                                                                                       }
}

if (_RUN_ () == 0 && _WRT_ () == 0                 ) {typewrite(font_ ,15, 30, 0);                                                                                       }
else                                                 {
if (_RUN_ () == 1                                  ) {typewrite(font_R, 5, 30, 0); typewrite(font_U, 5, 35, 0); typewrite(font_N, 5, 40, 0);                             }
if (_WRT_ () == 1                                  ) {typewrite(font_W, 5, 30, 0); typewrite(font_R, 5, 35, 0); typewrite(font_T, 5, 40, 0);                             }
}

if (_DEG_ () == 0 && _RAD_ () == 0 && _GRA_ () == 0) {typewrite(font_ ,15, 50, 0);                                                                                       }
else                                                 {
if (_DEG_ () == 1                                  ) {typewrite(font_D, 5, 50, 0); typewrite(font_E, 5, 55, 0); typewrite(font_G, 5, 60, 0);                             }
if (_RAD_ () == 1                                  ) {typewrite(font_R, 5, 50, 0); typewrite(font_A, 5, 55, 0); typewrite(font_D, 5, 60, 0);                             }
if (_GRA_ () == 1                                  ) {typewrite(font_G, 5, 50, 0); typewrite(font_R, 5, 55, 0); typewrite(font_A, 5, 60, 0);                             }
}

if (_TR_  () == 0                                  ) {typewrite(font_ ,10, 70, 0);                                                                                       }
else                                                 {typewrite(font_T, 5, 70, 0); typewrite(font_R, 5, 75, 0);                                                          }

if (_STOP_() == 1                                  ) {typewrite(font_S, 5,  0, 1); typewrite(font_T, 5,  5, 1); typewrite(font_O, 5, 10, 1); typewrite(font_P, 5, 15, 1);}

}


void counter_WRT() {

if (_STOP_() == 0) {

byte digit[4][5];

for (byte j = 0; j < 4; j++  ) {
for (byte i = 0; i < 5; i++) {
digit[j][i] = 0;
}
}

byte adr = 0x30;
byte bit =    0;

for (byte n_digit   = 0; n_digit   < 4; n_digit++  ) {
for (byte n_segment = 0; n_segment < 7; n_segment++) {

if (bitRead(LCD_MK85[adr], bit) == 1) {
if (n_segment == 0) {                             digit[n_digit][0] |= 0b00000111;}
if (n_segment == 1) {for (byte i = 0; i < 3; i++) digit[n_digit][i] |= 0b00000100;}
if (n_segment == 2) {                             digit[n_digit][0] |= 0b00011100;}
if (n_segment == 3) {for (byte i = 0; i < 3; i++) digit[n_digit][i] |= 0b00010000;}
if (n_segment == 4) {                             digit[n_digit][2] |= 0b00011100;}
if (n_segment == 5) {                             digit[n_digit][2] |= 0b00000111;}
if (n_segment == 6) {for (byte i = 0; i < 3; i++) digit[n_digit][i] |= 0b00000001;}
}


bit++; if (bit == 5) {adr += 8; bit = 0;}

}

typewrite(digit[n_digit], 5,  n_digit * 5,  1);

}

}

}

unsigned long n_time = 0;
byte       n_cursor_ = 0;
byte       n_cursor() {return (LCD_MK85[0x60] & 0x0F);}
boolean form_cursor() {return (LCD_MK85[0x60] >> 4)  ;}

void result() {

n_time++;

if (n_cursor_ != n_cursor()) {
    n_cursor_  = n_cursor();
n_time = 0;
}

boolean show_cursor = !((n_time >> 3) & 1);

setGraphicCursor(20, 1);

for (byte i = 0; i < 60; i++) {

byte column = 0;

if (show_cursor == 1 && n_cursor() == i / 5) {
if (form_cursor() == 1) column = 0b01000000;
else                    column = 0b01111111;
}
else {
for (byte j = 1; j < 8; j++) {
bitWrite(column, j - 1, bitRead(LCD_MK85[((i / 5) * 8) + j], i % 5));
}
}

oled.write(column);

}

}


void print_MK85() {

//~~ экран ~~~~~~~~~~~~~~~~~
indicators();  //
counter_WRT(); //
result();      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~

}


void loop() {while(1) {

print_MK85();

}

}

