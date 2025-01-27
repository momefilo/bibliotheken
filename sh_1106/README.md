**SH1106**\
Ein kleines Programm zum Schreiben und rudimentären Zeichenfunktionen fuer SH1106 OLED-Displays

**void sh1106_init()**\
Ist einmalig vor Benutzung der Funktionen aufzurufen

**sh1106_clear_screen()**\
Leert das Display

**sh1106_text12x16()**\
Schreibt textin 12x16Pixel pro Zeichen in vier Zeilen des Displays

**sh1106_text8x8()**\
Schreibt textin 8x8Pixel pro Zeichen in acht Zeilen des Displays

**sh1106_datacol()**\
Schreibt ein Byte als Spalte in eine 8-Y-Bits umfassende Page

**sh1106_datapage()**\
Schreibt mehrere Bytes als Spalten in eine 8-Y-Bits umfassende Page\

Eine genauere Beschreibung findet sich in den Komantaren der sh_1106.h
