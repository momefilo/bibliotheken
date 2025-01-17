**LCD ili_9341**\
Diese Bibliothek bietet je eine Textfunktion mit 12x12 und mit 16x16
Pixel Zeichengroesse. Die Text- und Hintergrundfarbe sind änderbar, und
es können Rechtecke, Farbverlaufsrechtecke und beliebige Farbinformation
gezeichnet werden. Die Touchfunktion des Displays kann aktiviert und die
Koordinaten der Berührung abgefragt werden. Die GPIO-Zuordnng ist zu
Beginn in der "ili_9341.c"-Datei anzupassen.\
Die Funktionen der Bibliothek sind folgend erlaeutert

**void ili9341_init()**\
Ist einmalig vor Benutzung der Funktionen aufzurufen

**void setOrientation(uint8_t ori)**\
Setzt die Orientierung des Displays, Parameter ori sind HORIZONTAL und VERTICAL

**void setFgColor(uint16_t color)**\
Setzt die Textfarbe als 565-Bit RGB-Farbwert

**void setSeColor(uint16_t color)**\
Setzt die Alternativ-Textfarbe als 565-Bit RGB-Farbwert

**void setBgColor(uint16_t color)**\
Setzt die Hintergrundfarbe als 565-Bit RGB-Farbwert

**void clearScreen()**\
Fuellt das Display mit der Hintergrundfarbe

**void writeText16x16(uint16_t *pos, char *text, int len, bool sel, bool matrix)**\
Schreibt char \*text mit 16x16 Pixeln\
pos[0]: ist die Spalte, pos[1] die Zeile, in Pixel wenn matrix = false.\
text: der Text der angezeigt wird\
len: ist die anzahl der Zeichen des Textes\
sel: wenn true wird der Text statt in FGground in Alternativfarbe angezeigt\
matrix: wenn true dann beziehen sich die Koordinatn in pos auf eine Matrix\
deren Felder Quadrate mit der Kantelänge von 16 Pixeln sind.

**void writeText12x12(uint16_t *pos, char *text, int len, bool sel, bool matrix)**\
Schreibt char \*text mit 12x12 Pixel\
pos[0]: ist die Spalte, pos[1] die Zeile, in Pixel wenn matrix = false.\
text: der der Text der angezeigt wird\
len: ist die anzahl der Zeichen des Textes\
sel: wenn true wird der Text statt in FGground in SelColor angezeigt.\
matrix: wenn true dann beziehen sich die Koordinatn in pos auf eine Matrix
deren Felder Quadrate mit der Kantelänge von 12 Pixeln sind.

**void paintRect(uint16_t *area, uint16_t color)**\
Zeichnet ein Rechteck in der 565-Bit RGB-Farbwert codierten
Farbe in den durch area bezeichneten Bereich\
area[0]: position x.start, area[1] position y.start\
area[2] position x.end, area[3] position y.end

**void drawRect(uint16_t *area, uint8_t *data)**\
Zeichnet die 565bit-farbcodierten Daten in den
durch area bezeichneten Bereich\
area[0]: position x.start, area[1] position y.start\
area[2] position x.end, area[3] position y.end\
\*data: Die Anfangsadresse des 565-Farbcodierten Speicherberichs der
auf das Display geschrieben wird

**void paintRectGradient(uint16_t *area, uint16_t color1, uint16_t color2)**\
565-Bit RGB-Farbwert
zeichnet einen horizontalen Verlauf von color1 zu color2
durch "setOrientation(HORIZONTAL/VERTICAL) kann der Verlauf auch vertikal erfolgen

**void ili9341_touch_init()**\
Initialisiert die Touchfunktions des Displays und muss vor Benutzung
einmalig aufgerufen sein

**uint16_t *ili9341_getTouch()**\
Gibt die Position als Array [x,y]=[0..240, 0..320] der Berührung
oder [0xffff, 0xffff] bei keiner Berührung des Displays zurueck.
Unabhaengig von der Orientierung werden die Koordinaten X der schmalen
Seite und Y der breiten Seite zugeordnet
