/* momefilo Desing
 * Diese Bibliothek bietet 2 von einender unabhängige Taktgeber mit ein si5351.
 * Der Frequenzbereich ist 8kHz bis 160Mhz.
 * Der Taktgeber mit der freq0 wird an ckl0, und invertiert an ckl1 ausgeben
 * Der Taktgeber mit der freq2 wird an ckl2 ausgeben
 * Bei Frequenzen <8kHz und >160MHz wird der entsprechende Ausgang HiZ
 **/
#ifndef my_si5351_h
#define my_si5351_h 1
#include "pico/stdlib.h"

// i2c_Busadresse
#define ADDR 0x60

// PLL-Grenzen
#define F_XTAL (25 * 1000 * 1000)
#define F_PLL_MIN (600 * 1000 * 1000)
#define F_PLL_MAX (900 * 1000 * 1000)
#define DIV_PLL_MAX (F_PLL_MAX / F_XTAL)
#define DIV_PLL_MIN (F_PLL_MIN / F_XTAL)

// Load-Kapazitaeten
#define LOAD_C6pF 1
#define LOAD_C8pF 2
#define LOAD_C10pF 3

typedef struct{// Divisor-Parameter
	uint16_t a;
	uint32_t b;
	uint32_t c;
	bool even;
	bool frac;
}DivPara;

//initialisiert den Taktgeber
// bus = i2c-bus
// sda = i2c-sda-pin
// scl = i2c-scl-pin
// freq0 >= 8 * 100 && freq0 <= 160 * 1000 * 1000
// freq2 >= 8 * 100 && freq0 <= 160 * 1000 * 1000
void si5351_init(uint8_t bus, uint8_t sda, uint8_t scl, uint32_t freq0, uint32_t freq2);

// setzt die Taktgeber
// freq0 >= 8 * 100 && freq0 <= 160 * 1000 * 1000
// freq2 >= 8 * 100 && freq0 <= 160 * 1000 * 1000
void si5351_setFreq(uint32_t f_out0, uint32_t f_out2);

// setzte die Eingangs-Kapazitaeten
// load = LOAD_C6pF | LOAD_C8pF | LOAD_C10pF
void si5351_setLoad(uint8_t load);

#endif
