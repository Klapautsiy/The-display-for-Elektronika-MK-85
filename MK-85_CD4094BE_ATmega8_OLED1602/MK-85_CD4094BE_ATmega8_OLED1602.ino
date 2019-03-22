
/*
http://www.pisi.com.pl/piotr433/mk85hwe.htm
https://klapautsiy.github.io/The-display-for-Elektronika-MK-85/
*/

#include <avr/power.h>
#include <LiquidCrystal.h>
// LiquidCrystal oled(RS,  E, D4, D5, D6, D7)
   LiquidCrystal oled( 4,  5,  0,  1,  6,  7);


volatile byte LCD_MK85[105];

//=================================================

/*
BELL - LETC "4DSTSD4"
https://klapautsiy.github.io/The-display-for-Elektronika-MK-85/pictures/4DSTSD4.png
*/

const byte LETC[7] {
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

byte address = (~PINB) - 0x80;
byte data    = (~PINC) & 0x1F;

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


const byte font_A[5] = {
0b00011110,
0b00000101,
0b00000101,
0b00000101,
0b00011110
};

const byte font_D[5] = {
0b00011111,
0b00010001,
0b00010001,
0b00010001,
0b00001110
};

const byte font_E[5] = {
0b00011111,
0b00010101,
0b00010101,
0b00010101,
0b00010001
};

const byte font_F[5] = {
0b00011111,
0b00000101,
0b00000101,
0b00000101,
0b00000001
};

const byte font_G[5] = {
0b00001110,
0b00010001,
0b00010101,
0b00010101,
0b00011101
};

const byte font_M[5] = {
0b00011111,
0b00000010,
0b00000100,
0b00000010,
0b00011111
};

const byte font_N[5] = {
0b00011111,
0b00000010,
0b00000100,
0b00001000,
0b00011111
};

const byte font_R[5] = {
0b00011111,
0b00000101,
0b00000101,
0b00001101,
0b00010010,
};

const byte font_S[5] = {
0b00010010,
0b00010101,
0b00010101,
0b00010101,
0b00001001
};

const byte font_T[5] = {
0b00000001,
0b00000001,
0b00011111,
0b00000001,
0b00000001
};

const byte font_U[5] = {
0b00001111,
0b00010000,
0b00010000,
0b00010000,
0b00001111
};

const byte font_W[5] = {
0b00011111,
0b00001000,
0b00000100,
0b00001000,
0b00011111
};

const byte font_X[5] = {
0b00010001,
0b00001010,
0b00000100,
0b00001010,
0b00010001
};

const byte font_O[5] = {
0b00001110,
0b00010001,
0b00010001,
0b00010001,
0b00001110
};

const byte font_P[5] = {
0b00011111,
0b00000101,
0b00000101,
0b00000101,
0b00000010
};

const byte font_ [5] = {
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


void typewrite(const byte array[5]) {
for (byte i = 0; i < 5; i++) oled.write(array[i]);
}


void     EXT_ () {typewrite(font_E); typewrite(font_X); typewrite(font_T);                   }
void     M_   () {typewrite(font_M);                                                         }
void     S_   () {typewrite(font_S);                                                         }
void     F_   () {typewrite(font_F);                                                         }
void     RUN_ () {typewrite(font_R); typewrite(font_U); typewrite(font_N);                   }
void     WRT_ () {typewrite(font_W); typewrite(font_R); typewrite(font_T);                   }
void     DEG_ () {typewrite(font_D); typewrite(font_E); typewrite(font_G);                   }
void     RAD_ () {typewrite(font_R); typewrite(font_A); typewrite(font_D);                   }
void     GRA_ () {typewrite(font_G); typewrite(font_R); typewrite(font_A);                   }
void     TR_  () {typewrite(font_T); typewrite(font_R);                                      }
void     STOP_() {typewrite(font_S); typewrite(font_T); typewrite(font_O); typewrite(font_P);}

void     _    () {typewrite(font_ );                                                         }
void     __   () {typewrite(font_ ); typewrite(font_ );                                      }
void     ___  () {typewrite(font_ ); typewrite(font_ ); typewrite(font_ );                   }


void indicators() {

boolean _EXT  = bitRead(LCD_MK85[0x00], 0);
boolean _S    = bitRead(LCD_MK85[0x00], 1);
boolean _F    = bitRead(LCD_MK85[0x00], 2);
boolean _RUN  = bitRead(LCD_MK85[0x08], 0);
boolean _WRT  = bitRead(LCD_MK85[0x08], 1);
boolean _DEG  = bitRead(LCD_MK85[0x08], 4);
boolean _RAD  = bitRead(LCD_MK85[0x18], 0);
boolean _GRA  = bitRead(LCD_MK85[0x20], 0);
boolean _TR   = bitRead(LCD_MK85[0x28], 0);
boolean _STOP = bitRead(LCD_MK85[0x58], 3);

if (_EXT  == 0                          ) {setGraphicCursor( 0, 0); ___  ();}
else                                      {setGraphicCursor( 0, 0); EXT_ ();}

if (_S    == 0 && _F   == 0             ) {setGraphicCursor(20, 0); _    ();}
else                                      {
if (_S    == 1 && _F   == 1             ) {setGraphicCursor(20, 0); M_   ();}
if (_S    == 1 && _F   == 0             ) {setGraphicCursor(20, 0); S_   ();}
if (_S    == 0 && _F   == 1             ) {setGraphicCursor(20, 0); F_   ();}
}

if (_RUN  == 0 && _WRT == 0             ) {setGraphicCursor(30, 0); ___  ();}
else                                      {
if (_RUN  == 1                          ) {setGraphicCursor(30, 0); RUN_ ();}
if (              _WRT == 1             ) {setGraphicCursor(30, 0); WRT_ ();}
}

if (_DEG  == 0 && _RAD == 0 && _GRA == 0) {setGraphicCursor(50, 0); ___  ();}
else                                      {
if (_DEG  == 1                          ) {setGraphicCursor(50, 0); DEG_ ();}
if (              _RAD == 1             ) {setGraphicCursor(50, 0); RAD_ ();}
if (                           _GRA == 1) {setGraphicCursor(50, 0); GRA_ ();}
}

if (_TR   == 0                          ) {setGraphicCursor(70, 0); __   ();}
else                                      {setGraphicCursor(70, 0); TR_  ();}

if (_STOP == 1                          ) {setGraphicCursor( 0, 1); STOP_();}
else                                      {counter_WRT();                   }

}


void counter_WRT() {

byte adr = 0x30;
byte bit =    0;

for (byte n_digit   = 0; n_digit   < 4; n_digit++  ) {

byte digit[5] = {
0b00000000,
0b00000000,
0b00000000,
0b00000000,
0b00000000
};

for (byte n_segment = 0; n_segment < 7; n_segment++) {

if (bitRead(LCD_MK85[adr], bit) == 1) {
if (n_segment == 0) {                             digit[0 + 1] |= 0b00001110;}
if (n_segment == 1) {for (byte i = 0; i < 3; i++) digit[i + 1] |= 0b00001000;}
if (n_segment == 2) {                             digit[0 + 1] |= 0b00111000;}
if (n_segment == 3) {for (byte i = 0; i < 3; i++) digit[i + 1] |= 0b00100000;}
if (n_segment == 4) {                             digit[2 + 1] |= 0b00111000;}
if (n_segment == 5) {                             digit[2 + 1] |= 0b00001110;}
if (n_segment == 6) {for (byte i = 0; i < 3; i++) digit[i + 1] |= 0b00000010;}
}

bit++; if (bit == 5) {adr += 8; bit = 0;}

}

setGraphicCursor(n_digit * 5,  1);
typewrite(digit);

}

}


void result() {

static unsigned long n_time = 0;
static byte       n_cursor_ = 0;
       byte       n_cursor  = LCD_MK85[0x60] & 0x0F;

n_time++;

if (n_cursor_ != n_cursor) {
    n_cursor_  = n_cursor;
n_time = 0;
}

boolean show_cursor = !((n_time >> 3) & 1);

setGraphicCursor(20, 1);

for (byte i = 0; i < 60; i++) {

byte column = 0b00000000;

if (show_cursor == 1 && n_cursor_ == i / 5) {

boolean form_cursor = (LCD_MK85[0x60] >> 4);

if (form_cursor == 1) column = 0b01000000;
else                  column = 0b01111111;

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

indicators();

result();

}


void loop() {while(1) {

print_MK85();

}

}

