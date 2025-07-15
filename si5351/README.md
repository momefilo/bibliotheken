**SI5351**\
Diese Bibliothek bietet zwei voneinender unabhängige Taktgeber mit einem si5351.\
Der Frequenzbereich ist 8kHz bis 160Mhz.\
Der Taktgeber mit der freq0 wird an ckl0, und invertiert an ckl1 ausgeben\
Der Taktgeber mit der freq2 wird an ckl2 ausgeben\
Bei Frequenzen <8kHz und >160MHz wird der entsprechende Ausgang HiZ\
Default Eingangs-Kapazitaet = 10pf

**Bugs**\
Bei Frequenzen < 100kHz ist die Ausgabe ungenau

**Festwerte der Eingangskapazitaet**\
LOAD_C6pF = 6pF\
LOAD_C8pF = 8pF\
LOAD_C10pF = 10pF\

**void si5351_init(uint8_t bus, uint8_t sda, uint8_t scl, uint32_t freq0, uint32_t freq2)**\
initialisiert den Taktgeber\
bus = i2c-bus\
sda = i2c-sda-pin\
scl = i2c-scl-pin\
freq0 >= 8 * 100 && freq0 <= 160 * 1000 * 1000\
freq2 >= 8 * 100 && freq0 <= 160 * 1000 * 1000\

**bool si5351_setFreq(uint32_t f_out0, uint32_t f_out2)**\
setzt die Taktgeber\
f_out0 >= 8 * 100 && freq0 <= 160 * 1000 * 1000\
f_out2 >= 8 * 100 && freq0 <= 160 * 1000 * 1000\

**void si5351_setLoad(uint8_t load)**\
setzte die Eingangs-Kapazitaeten\
load = LOAD_C6pF | LOAD_C8pF | LOAD_C10pF\
