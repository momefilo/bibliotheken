/* Bietet je eine Schreibfunktion in font8x8 und 12x16 nur im Querformat
 * Die SH1106 Displays ist in Zeilen mit je acht Pixel an Hoehe aufgeteilt.
 * Die Bibliothek Teilt die Pixelmatrix des Displays in eine Zeichenmatrix
 * und verlang als Parameter der Schreibfunktionen die Position in
 * dieser Zeichenmatrix. Bei einem 132x64 Display gibt es vier Zeilen mit
 * jeweils 11 Zeichen im Font 8x16 und jeweils doppelt so viele in font 8x8
 * So ist die Position (2,3) Das zweite Zeichen von rechts in der dritten Zeile.
 */

#ifndef my_sh_1106_h
#define my_sh_1106_h 1

#define I2C_BAUDRATE (100 * 1000)
#define I2C_ADR 0x3C

// GPIO Belegung
#define I2C_BUS 0
#define PIN_SDA 4
#define PIN_SCL 5

#include "pico/stdlib.h"
/* Initialisiert das Display und ist einmalig vor Benutzung der
 * aufzurufen */
void sh1106_init();

/* Leert das Display */
void sh1106_clear_screen();

/* pos:= uint8_t[x,y]
 * text:= uint8_t["text"]
 * invert:= text wird invertiert dargestellt */
void sh1106_text12x16(uint8_t *pos, uint8_t *text, bool invert);

/* pos:= uint8_t[x,y]
 * text:= uint8_t["text"]
 * invert:= text wird invertiert dargestellt */
void sh1106_text8x8(uint8_t *pos, uint8_t *text, bool invert);

/* Schreibt das Byte als Spalte in die Page
 * col:= Die Spalte die geschrieben wird
 * page:= Die Page in der die Spalte geschrieben wird
 * data:= Das Byte das geschrieben wird */
void sh1106_datacol(uint8_t col, uint8_t page, uint8_t *data);

/* Schreibt die Bytes als Spalten in eine Page
 * startcol:= Die erste der Spalten die geschrieben wird
 * page:= Die Page in der die Spalten geschrieben wird
 * data:= Die Bytes welche als Spalten in die Page geschrieben werden */
void sh1106_datapage(uint8_t startcol, uint8_t page, uint8_t *data, uint8_t len);

#endif
