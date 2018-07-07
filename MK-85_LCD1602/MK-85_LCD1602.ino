
/*
information from http://www.pisi.com.pl/piotr433/mk85hwe.htm
*/

// #include <LiquidCrystalFast.h>
// LiquidCrystalFast lcd(8, 9, 10, 11, 12, 13, 14);

#include <LiquidCrystal.h>
LiquidCrystal lcd(8, 9, 10, 11, 12, 13, 14);


volatile byte   _PIND_ = 0;

volatile boolean SHIFT = 0;

volatile boolean nSYNC = 0;
volatile boolean  SYNC = 0;
volatile boolean  AD   = 0;

volatile byte   addres = 0;
volatile byte   bits   = 0;

volatile byte i_addres = 0;
volatile byte i_bits   = 0;

volatile byte LCD_bits[105];

volatile boolean print = 0;

         byte n_Cursor = 0;

         boolean  state_mode = 0;
         boolean nstate_mode = 0;

         boolean mode_screen = 0;

ISR(INT1_vect) {

/* http://www.pisi.com.pl/piotr433/mk85dbus.png */

sei();

_PIND_ = PIND;

SHIFT = bitRead(_PIND_, 3);


if (SHIFT == 0) {
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
nSYNC  = bitRead(_PIND_, 1);

if (SYNC == 0 && nSYNC == 1) {

i_addres = 0;
  // addres = 0;

i_bits = 0;
  // bits = 0;

}

SYNC = nSYNC;
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

AD    = bitRead(_PIND_, 0);

if (SYNC == 1) {if (i_addres < 8) bitWrite(addres, i_addres++, !AD);}
else           {if (i_bits   < 8) bitWrite(bits  , i_bits++  , !AD);}

if (addres >= 0x80 && addres <= 0xE8 && i_bits == 8) {LCD_bits[addres - 0x80] = bits; print = 1;}

PORTC &= B11110011;
PORTC |= B00001000;

} else {

PORTC &= B11110011;
PORTC |= B00000100;

}

}


void setup() {

lcd.begin(16, 2);

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/*светодиоды индикации состояний пинов
используется для отладки или визуализации*/
pinMode     ( 4, OUTPUT); /*LED_00*/
pinMode     ( 5, OUTPUT); /*LED_01*/
pinMode     (17, OUTPUT); /*LED_02*/
digitalWrite( 4, 1);      /*LED_00*/
digitalWrite( 5, 1);      /*LED_01*/
digitalWrite(17, 1);      /*LED_02*/
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

pinMode( 3, INPUT); // SHIFT
pinMode( 1, INPUT); // SYNC
pinMode( 0, INPUT); // AD

pinMode(18, OUTPUT); // P0
pinMode(19, OUTPUT); // P1


fix_pin_P();

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/*
LOW     прерывание вызывается, когда на порту LOW
CHANGE  прерывание вызывается при смене значения на порту
RISING  прерывание вызывается при смене значения на порту с LOW на HIGH
FALLING прерывание вызывается при смене значения на порту с HIGH на LOW
*/

cli(); // запрет прерываний глобально
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// GIFR = (1<<INTF1);
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// MCUCR |= (1<<ISC00)|(1<<ISC01); /*INT0 RISING*/
// MCUCR |= (1<<ISC01);            /*INT0 FALLING*/
// MCUCR |= (1<<ISC00);            /*INT0 CHANGE*/
// GICR  |= (1<<INT0);             /*INT0 enabled*/
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// MCUCR |= (1<<ISC10)|(1<<ISC11); /*INT1 RISING*/
// MCUCR |= (1<<ISC11);            /*INT1 FALLING*/
MCUCR |= (1<<ISC10);            /*INT1 CHANGE*/
GICR  |= (1<<INT1);             /*INT1 enabled*/
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
sei(); // разрешение прерываний глобально
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

for(byte i = 0; i < 105; i++) LCD_bits[i] = 0;

_PIND_ = PIND;
 SHIFT = bitRead(_PIND_, 3);
nSYNC  = bitRead(_PIND_, 1);
 SYNC  = nSYNC;
 AD    = bitRead(_PIND_, 0);

print = 1;


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/*
заставка дисплея.
стирается после перехода МК-85 из состояния "выключено" в состояние "включено".
*/
/*LCD_bits[0xE0 - 0x80] = 1;
lcd.setCursor(1, 0);
lcd.print("MK-85 display");
lcd.setCursor(1, 1);
lcd.print("off/on MK-85");
while(LCD_bits[0xE0 - 0x80] == 1);
lcd.clear();*/
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

}


void printBits(byte myByte) {
for (byte mask = 0x80; mask; mask >>= 1) {
if (mask & myByte) lcd.print ('1'); else lcd.print ('0');
}
}


void print_LCD_debug() {

if (print == 1) {print = 0;

lcd.clear();

lcd.setCursor(0, 0);
lcd.print(i_addres);

lcd.setCursor(0, 1);
lcd.print(i_bits);

lcd.setCursor(2, 0);
lcd.print(byte(addres - 0x80), HEX);

lcd.setCursor(5, 0);
lcd.print(byte(addres       ), HEX);

lcd.setCursor(5, 1);
lcd.print(byte(bits         ), HEX);

lcd.setCursor(8, 0);
printBits(addres);

lcd.setCursor(8, 1);
printBits(bits);

}

}



byte invertBits(byte a) {return (((a&1)<<4)|((a&2)<<2)|(a&4)|((a&8)>>2)|((a&16)>>4));}

void print_MK85() {

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
nstate_mode = (millis()>>9) & 1;
if (state_mode != nstate_mode) {
	state_mode  = nstate_mode;

mode_screen = !mode_screen;

print = 1;

}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

if (print == 1) {print = 0;

/*
pixel assignments to the display RAM locations
http://www.pisi.com.pl/piotr433/mk85scr.png
*/

// EXT
if (bitRead(LCD_bits[0x0 ], 0) /*S*/ == 0) {lcd.setCursor(0, 0); lcd.print("   ");}
else                                       {lcd.setCursor(0, 0); lcd.print("EXT");}

// S, F, MODE
if (bitRead(LCD_bits[0x0 ], 1) /*S*/ == 0 && bitRead(LCD_bits[0x0 ], 2) /*F*/ == 0) {lcd.setCursor(4, 0); lcd.print(" ");}
else {
if (bitRead(LCD_bits[0x0 ], 1) /*S*/ == 1 && bitRead(LCD_bits[0x0 ], 2) /*F*/ == 1) {lcd.setCursor(4, 0); lcd.print("M");}
if (bitRead(LCD_bits[0x0 ], 1) /*S*/ == 1 && bitRead(LCD_bits[0x0 ], 2) /*F*/ == 0) {lcd.setCursor(4, 0); lcd.print("S");}
if (bitRead(LCD_bits[0x0 ], 1) /*S*/ == 0 && bitRead(LCD_bits[0x0 ], 2) /*F*/ == 1) {lcd.setCursor(4, 0); lcd.print("F");}
}

// RUN, WRT
if (bitRead(LCD_bits[0x8 ], 0) /*RUN*/ == 0 && bitRead(LCD_bits[0x8 ], 1) /*WRT*/ == 0) {lcd.setCursor(6, 0); lcd.print("   ");}
else {
if (bitRead(LCD_bits[0x8 ], 0) /*RUN*/ == 1                                           ) {lcd.setCursor(6, 0); lcd.print("RUN");}
if (bitRead(LCD_bits[0x8 ], 1) /*WRT*/ == 1                                           ) {lcd.setCursor(6, 0); lcd.print("WRT");}
}

// DEG, RAD, GRA
if (bitRead(LCD_bits[0x8 ], 4) /*DEG*/ == 0 && bitRead(LCD_bits[0x18], 0) /*RAD*/ == 0 && bitRead(LCD_bits[0x20], 0) /*GRA*/ == 0) {lcd.setCursor(10, 0); lcd.print("   ");}
else {
if (bitRead(LCD_bits[0x8 ], 4) /*DEG*/ == 1                                                                                      ) {lcd.setCursor(10, 0); lcd.print("DEG");}
if (bitRead(LCD_bits[0x18], 0) /*RAD*/ == 1                                                                                      ) {lcd.setCursor(10, 0); lcd.print("RAD");}
if (bitRead(LCD_bits[0x20], 0) /*GRA*/ == 1                                                                                      ) {lcd.setCursor(10, 0); lcd.print("GRA");}
}

// TR
if (bitRead(LCD_bits[0x28], 0) /*TR*/ == 0) {lcd.setCursor(14, 0); lcd.print("  ");}
else                                        {lcd.setCursor(14, 0); lcd.print("TR");}

// STOP
if (bitRead(LCD_bits[0x58], 3) /*STOP*/ == 0) {lcd.setCursor(12, 1); lcd.print("    ");}
else                                          {lcd.setCursor(12, 1); lcd.print("STOP");}


byte x00[8] = {
invertBits(LCD_bits[0x01]),
invertBits(LCD_bits[0x02]),
invertBits(LCD_bits[0x03]),
invertBits(LCD_bits[0x04]),
invertBits(LCD_bits[0x05]),
invertBits(LCD_bits[0x06]),
invertBits(LCD_bits[0x07]),
0b00000
};

byte x01[8] = {
invertBits(LCD_bits[0x09]),
invertBits(LCD_bits[0x0A]),
invertBits(LCD_bits[0x0B]),
invertBits(LCD_bits[0x0C]),
invertBits(LCD_bits[0x0D]),
invertBits(LCD_bits[0x0E]),
invertBits(LCD_bits[0x0F]),
0b00000
};

byte x02[8] = {
invertBits(LCD_bits[0x11]),
invertBits(LCD_bits[0x12]),
invertBits(LCD_bits[0x13]),
invertBits(LCD_bits[0x14]),
invertBits(LCD_bits[0x15]),
invertBits(LCD_bits[0x16]),
invertBits(LCD_bits[0x17]),
0b00000
};

byte x03[8] = {
invertBits(LCD_bits[0x19]),
invertBits(LCD_bits[0x1A]),
invertBits(LCD_bits[0x1B]),
invertBits(LCD_bits[0x1C]),
invertBits(LCD_bits[0x1D]),
invertBits(LCD_bits[0x1E]),
invertBits(LCD_bits[0x1F]),
0b00000
};

byte x04[8] = {
invertBits(LCD_bits[0x21]),
invertBits(LCD_bits[0x22]),
invertBits(LCD_bits[0x23]),
invertBits(LCD_bits[0x24]),
invertBits(LCD_bits[0x25]),
invertBits(LCD_bits[0x26]),
invertBits(LCD_bits[0x27]),
0b00000
};

byte x05[8] = {
invertBits(LCD_bits[0x29]),
invertBits(LCD_bits[0x2A]),
invertBits(LCD_bits[0x2B]),
invertBits(LCD_bits[0x2C]),
invertBits(LCD_bits[0x2D]),
invertBits(LCD_bits[0x2E]),
invertBits(LCD_bits[0x2F]),
0b00000
};

byte x06[8] = {
invertBits(LCD_bits[0x31]),
invertBits(LCD_bits[0x32]),
invertBits(LCD_bits[0x33]),
invertBits(LCD_bits[0x34]),
invertBits(LCD_bits[0x35]),
invertBits(LCD_bits[0x36]),
invertBits(LCD_bits[0x37]),
0b00000
};

byte x07[8] = {
invertBits(LCD_bits[0x39]),
invertBits(LCD_bits[0x3A]),
invertBits(LCD_bits[0x3B]),
invertBits(LCD_bits[0x3C]),
invertBits(LCD_bits[0x3D]),
invertBits(LCD_bits[0x3E]),
invertBits(LCD_bits[0x3F]),
0b00000
};

byte x08[8] = {
invertBits(LCD_bits[0x41]),
invertBits(LCD_bits[0x42]),
invertBits(LCD_bits[0x43]),
invertBits(LCD_bits[0x44]),
invertBits(LCD_bits[0x45]),
invertBits(LCD_bits[0x46]),
invertBits(LCD_bits[0x47]),
0b00000
};

byte x09[8] = {
invertBits(LCD_bits[0x49]),
invertBits(LCD_bits[0x4A]),
invertBits(LCD_bits[0x4B]),
invertBits(LCD_bits[0x4C]),
invertBits(LCD_bits[0x4D]),
invertBits(LCD_bits[0x4E]),
invertBits(LCD_bits[0x4F]),
0b00000
};

byte x10[8] = {
invertBits(LCD_bits[0x51]),
invertBits(LCD_bits[0x52]),
invertBits(LCD_bits[0x53]),
invertBits(LCD_bits[0x54]),
invertBits(LCD_bits[0x55]),
invertBits(LCD_bits[0x56]),
invertBits(LCD_bits[0x57]),
0b00000
};

byte x11[8] = {
invertBits(LCD_bits[0x59]),
invertBits(LCD_bits[0x5A]),
invertBits(LCD_bits[0x5B]),
invertBits(LCD_bits[0x5C]),
invertBits(LCD_bits[0x5D]),
invertBits(LCD_bits[0x5E]),
invertBits(LCD_bits[0x5F]),
0b00000
};


lcd.createChar(4, x04);
lcd.createChar(5, x05);
lcd.createChar(6, x06);
lcd.createChar(7, x07);

lcd.setCursor(4, 1);
lcd.print((char)4);
lcd.print((char)5);
lcd.print((char)6);
lcd.print((char)7);


n_Cursor = LCD_bits[0xE0 - 0x80]; bitClear(n_Cursor, 4);


if (                n_Cursor <  4) mode_screen = 0;
if (n_Cursor > 7 && n_Cursor < 12) mode_screen = 1;


if (mode_screen == 0) {
lcd.createChar(0, x00);
lcd.createChar(1, x01);
lcd.createChar(2, x02);
lcd.createChar(3, x03);
lcd.setCursor(8, 1);
lcd.print("    ");
lcd.setCursor(0, 1);
} else {
lcd.createChar(0, x08);
lcd.createChar(1, x09);
lcd.createChar(2, x10);
lcd.createChar(3, x11);
lcd.setCursor(0, 1);
lcd.print("    ");
lcd.setCursor(8, 1);
}

lcd.print((char)0);
lcd.print((char)1);
lcd.print((char)2);
lcd.print((char)3);

lcd.setCursor(n_Cursor, 1);

}

//~~~ курсор ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
if (n_Cursor < 12) {
if (bitRead(LCD_bits[0xE0 - 0x80], 4) == 1) {lcd.noBlink();
if (((millis() >> 9) & 1)             == 0)  lcd.noCursor(); else lcd.cursor();
}
else                                        {lcd.noCursor();      lcd.blink();}
}
else                                        {lcd.noBlink();
                                             lcd.noCursor();
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

}

void fix_pin_P() {
//~~~FIX~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
if (bitRead(PIND, 3) == 0) {
PORTC &= B11110011;
PORTC |= B00001000;
} else {
PORTC &= B11110011;
PORTC |= B00000100;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
}

void loop() {while(1) {

//===отладка======================================

byte    PIND_debug  = PIND;
boolean SHIFT_debug = bitRead(PIND_debug, 3);
boolean SYNC_debug  = bitRead(PIND_debug, 1);
boolean AD_debug    = bitRead(PIND_debug, 0);
digitalWrite( 4,  SHIFT_debug);     /*LED_00*/
digitalWrite( 5, !SYNC_debug );     /*LED_01*/
digitalWrite(17, !AD_debug   );     /*LED_02*/


// digitalWrite(4, digitalRead(18));      /*LED_00*/ /*P0*/
// digitalWrite(5, digitalRead(19));      /*LED_01*/ /*P1*/

//================================================

// print_LCD_debug();
// или
print_MK85();

}

}



