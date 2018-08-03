
/*
information from http://www.pisi.com.pl/piotr433/mk85hwe.htm
*/

#include <LiquidCrystal.h>
LiquidCrystal lcd(8, 9, 10, 11, 12, 13, 14);


volatile byte address = 0;
volatile byte data    = 0;

volatile byte LCD_MK85[105];

volatile boolean print = 0;

         byte n_Cursor = 0;

         boolean  state_mode = 0;
         boolean nstate_mode = 0;

         boolean mode_screen = 0;


ISR(INT1_vect) {

/* http://www.pisi.com.pl/piotr433/mk85dbus.png */

if (bitRead(PIND, 3) == 1) {address = PINC - 0x80;}
else                       {data    = PINA;

if (address <= 0xE8 - 0x80) {LCD_MK85[address] = data; print = 1;}

}

}


void setup() {

lcd.begin(16, 2);

for(byte i = 0; i < 105; i++) LCD_MK85[i] = 0;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/*
LOW     прерывание вызывается, когда на порту LOW
CHANGE  прерывание вызывается при смене значения на порту
RISING  прерывание вызывается при смене значения на порту с LOW на HIGH
FALLING прерывание вызывается при смене значения на порту с HIGH на LOW
*/

// cli(); // запрет прерываний глобально
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
// sei(); // разрешение прерываний глобально
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/*
заставка дисплея.
стирается после перехода МК-85 из состояния "выключено" в состояние "включено".
*/
/*
LCD_MK85[0xE0 - 0x80] = 1;

lcd.setCursor(1, 0);
lcd.print("display MK-85");
lcd.setCursor(1, 1);
lcd.print("off--on MK-85");
while(LCD_MK85[0xE0 - 0x80] == 1);
lcd.clear();
*/
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

print = 1;

}


void printBits(byte myByte) {
for (byte mask = 0x80; mask; mask >>= 1) {
if (mask & myByte) lcd.print ('1'); else lcd.print ('0');
}
}


void print_LCD_debug() {

if (print == 1) {print = 0;

lcd.clear();

lcd.setCursor(2, 0);
lcd.print(byte(address - 0x80), HEX);


lcd.setCursor(5, 0);
lcd.print(byte(address       ), HEX);

lcd.setCursor(5, 1);
lcd.print(byte(data          ), HEX);

lcd.setCursor(8, 0);
printBits(address);

lcd.setCursor(8, 1);
printBits(data);

}

}


byte invertBits(byte a) {return (((a&1)<<4)|((a&2)<<2)|(a&4)|((a&8)>>2)|((a&16)>>4));}

void print_MK85() {


byte cursor0[8] = {
0b00000,
0b00000,
0b00000,
0b00000,
0b00000,
0b00000,
0b11111,
0b00000
};

byte cursor1[8] = {
0b11111,
0b11111,
0b11111,
0b11111,
0b11111,
0b11111,
0b11111,
0b00000
};


byte LCD1602[12][8] = {

{
invertBits(LCD_MK85[0x01]),
invertBits(LCD_MK85[0x02]),
invertBits(LCD_MK85[0x03]),
invertBits(LCD_MK85[0x04]),
invertBits(LCD_MK85[0x05]),
invertBits(LCD_MK85[0x06]),
invertBits(LCD_MK85[0x07]),
0b00000
},

{
invertBits(LCD_MK85[0x09]),
invertBits(LCD_MK85[0x0A]),
invertBits(LCD_MK85[0x0B]),
invertBits(LCD_MK85[0x0C]),
invertBits(LCD_MK85[0x0D]),
invertBits(LCD_MK85[0x0E]),
invertBits(LCD_MK85[0x0F]),
0b00000
},

{
invertBits(LCD_MK85[0x11]),
invertBits(LCD_MK85[0x12]),
invertBits(LCD_MK85[0x13]),
invertBits(LCD_MK85[0x14]),
invertBits(LCD_MK85[0x15]),
invertBits(LCD_MK85[0x16]),
invertBits(LCD_MK85[0x17]),
0b00000
},

{
invertBits(LCD_MK85[0x19]),
invertBits(LCD_MK85[0x1A]),
invertBits(LCD_MK85[0x1B]),
invertBits(LCD_MK85[0x1C]),
invertBits(LCD_MK85[0x1D]),
invertBits(LCD_MK85[0x1E]),
invertBits(LCD_MK85[0x1F]),
0b00000
},

{
invertBits(LCD_MK85[0x21]),
invertBits(LCD_MK85[0x22]),
invertBits(LCD_MK85[0x23]),
invertBits(LCD_MK85[0x24]),
invertBits(LCD_MK85[0x25]),
invertBits(LCD_MK85[0x26]),
invertBits(LCD_MK85[0x27]),
0b00000
},

{
invertBits(LCD_MK85[0x29]),
invertBits(LCD_MK85[0x2A]),
invertBits(LCD_MK85[0x2B]),
invertBits(LCD_MK85[0x2C]),
invertBits(LCD_MK85[0x2D]),
invertBits(LCD_MK85[0x2E]),
invertBits(LCD_MK85[0x2F]),
0b00000
},

{
invertBits(LCD_MK85[0x31]),
invertBits(LCD_MK85[0x32]),
invertBits(LCD_MK85[0x33]),
invertBits(LCD_MK85[0x34]),
invertBits(LCD_MK85[0x35]),
invertBits(LCD_MK85[0x36]),
invertBits(LCD_MK85[0x37]),
0b00000
},

{
invertBits(LCD_MK85[0x39]),
invertBits(LCD_MK85[0x3A]),
invertBits(LCD_MK85[0x3B]),
invertBits(LCD_MK85[0x3C]),
invertBits(LCD_MK85[0x3D]),
invertBits(LCD_MK85[0x3E]),
invertBits(LCD_MK85[0x3F]),
0b00000
},

{
invertBits(LCD_MK85[0x41]),
invertBits(LCD_MK85[0x42]),
invertBits(LCD_MK85[0x43]),
invertBits(LCD_MK85[0x44]),
invertBits(LCD_MK85[0x45]),
invertBits(LCD_MK85[0x46]),
invertBits(LCD_MK85[0x47]),
0b00000
},

{
invertBits(LCD_MK85[0x49]),
invertBits(LCD_MK85[0x4A]),
invertBits(LCD_MK85[0x4B]),
invertBits(LCD_MK85[0x4C]),
invertBits(LCD_MK85[0x4D]),
invertBits(LCD_MK85[0x4E]),
invertBits(LCD_MK85[0x4F]),
0b00000
},

{
invertBits(LCD_MK85[0x51]),
invertBits(LCD_MK85[0x52]),
invertBits(LCD_MK85[0x53]),
invertBits(LCD_MK85[0x54]),
invertBits(LCD_MK85[0x55]),
invertBits(LCD_MK85[0x56]),
invertBits(LCD_MK85[0x57]),
0b00000
},

{
invertBits(LCD_MK85[0x59]),
invertBits(LCD_MK85[0x5A]),
invertBits(LCD_MK85[0x5B]),
invertBits(LCD_MK85[0x5C]),
invertBits(LCD_MK85[0x5D]),
invertBits(LCD_MK85[0x5E]),
invertBits(LCD_MK85[0x5F]),
0b00000
}

};


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
if (bitRead(LCD_MK85[0x0 ], 0) /*S*/ == 0) {lcd.setCursor(0, 0); lcd.print("   ");}
else                                       {lcd.setCursor(0, 0); lcd.print("EXT");}

// S, F, MODE
if (bitRead(LCD_MK85[0x0 ], 1) /*S*/ == 0 && bitRead(LCD_MK85[0x0 ], 2) /*F*/ == 0) {lcd.setCursor(4, 0); lcd.print(" ");}
else {
if (bitRead(LCD_MK85[0x0 ], 1) /*S*/ == 1 && bitRead(LCD_MK85[0x0 ], 2) /*F*/ == 1) {lcd.setCursor(4, 0); lcd.print("M");}
if (bitRead(LCD_MK85[0x0 ], 1) /*S*/ == 1 && bitRead(LCD_MK85[0x0 ], 2) /*F*/ == 0) {lcd.setCursor(4, 0); lcd.print("S");}
if (bitRead(LCD_MK85[0x0 ], 1) /*S*/ == 0 && bitRead(LCD_MK85[0x0 ], 2) /*F*/ == 1) {lcd.setCursor(4, 0); lcd.print("F");}
}

// RUN, WRT
if (bitRead(LCD_MK85[0x8 ], 0) /*RUN*/ == 0 && bitRead(LCD_MK85[0x8 ], 1) /*WRT*/ == 0) {lcd.setCursor(6, 0); lcd.print("   ");}
else {
if (bitRead(LCD_MK85[0x8 ], 0) /*RUN*/ == 1                                           ) {lcd.setCursor(6, 0); lcd.print("RUN");}
if (bitRead(LCD_MK85[0x8 ], 1) /*WRT*/ == 1                                           ) {lcd.setCursor(6, 0); lcd.print("WRT");}
}

// DEG, RAD, GRA
if (bitRead(LCD_MK85[0x8 ], 4) /*DEG*/ == 0 && bitRead(LCD_MK85[0x18], 0) /*RAD*/ == 0 && bitRead(LCD_MK85[0x20], 0) /*GRA*/ == 0) {lcd.setCursor(10, 0); lcd.print("   ");}
else {
if (bitRead(LCD_MK85[0x8 ], 4) /*DEG*/ == 1                                                                                      ) {lcd.setCursor(10, 0); lcd.print("DEG");}
if (bitRead(LCD_MK85[0x18], 0) /*RAD*/ == 1                                                                                      ) {lcd.setCursor(10, 0); lcd.print("RAD");}
if (bitRead(LCD_MK85[0x20], 0) /*GRA*/ == 1                                                                                      ) {lcd.setCursor(10, 0); lcd.print("GRA");}
}

// TR
if (bitRead(LCD_MK85[0x28], 0) /*TR*/ == 0) {lcd.setCursor(14, 0); lcd.print("  ");}
else                                        {lcd.setCursor(14, 0); lcd.print("TR");}

// STOP
if (bitRead(LCD_MK85[0x58], 3) /*STOP*/ == 0) {lcd.setCursor(12, 1); lcd.print("    ");}
else                                          {lcd.setCursor(12, 1); lcd.print("STOP");}


n_Cursor = LCD_MK85[0xE0 - 0x80]; bitClear(n_Cursor, 4);


if (                n_Cursor <  4) mode_screen = 0;
if (n_Cursor > 7 && n_Cursor < 12) mode_screen = 1;


byte offset_screen = 0;

if (mode_screen == 0) {
offset_screen = 0;
lcd.setCursor(8, 1);
} else {
offset_screen = 4;
lcd.setCursor(0, 1);
}

lcd.print("    ");

for (byte i = offset_screen; i < 8 + offset_screen; i++) {
lcd.createChar(i, LCD1602[i]);
lcd.setCursor(i, 1);
lcd.print((char) i);
}

}


//~~~ курсор ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
if (n_Cursor < 12) {

if (state_mode             == 0) {/*напечатать курсор*/

if (bitRead(LCD_MK85[0xE0 - 0x80], 4) == 1) {lcd.createChar(n_Cursor, cursor0); /*курсор "нижняя черта"*/        }
else                                        {lcd.createChar(n_Cursor, cursor1); /*курсор "чёрный прямоугольник"*/}

lcd.setCursor(n_Cursor, 1);
lcd.print((char)n_Cursor);

}

}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

}


void loop() {while(1) {

// print_LCD_debug();
// или
print_MK85();

}

}



