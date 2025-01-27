// bibliotheken
#include "sh_1106/sh_1106.h"
#include "drehgeber/drehgeber.h"
//#include "buttons/buttons.h"
//#include "st_7735/st_7735.h"// es kann nur ein Display eingebunden werden
//#include "ili_9341/ili_9341.h"// es kann nur ein Display eingebunden werden
//#include "ili_9341/tools/fraktal_240x135.h"// Bilddatei
#include "momefilo_flash/momefilo_flash.h"
#include "buzzer_sound/buzzer_sound.h"
#include "pico/stdlib.h"
#include <stdlib.h>
#include <stdio.h>

int flashError(){

	/* Datenarrea zum testen des Flashspeichers*/
	uint32_t testData[63];

	/* anfang und ende des uint32_t Arreas*/
	uint8_t start = 0;
	uint8_t ende = 62;

	/* Ueberprüft 19 Sektoren von oben harab auf "Datenintegritaet"
	 * des uint32_t Arreas, und füllt diese am Ende zur Ruekueberprufung
	 * mit codierten Daten*/
	for(int sektor=0; sektor<19; sektor++){

		/* Array aus 63 Integerwerten zur Prüfung*/
		for(uint32_t i=0; i<63; i++){ testData[i]=i; }

		/* Initialisiert den Sektor im Flash*/
		flash_init(sektor);

		/* Schreibt testData vollstaendig in die
		 * 63 Speicherbereiche des Flash-Sektors*/
		flash_setDataRow(start, ende, testData);

		/* Ueberschreibt den Sektor 19 mal
		 * und überprüft dabei die Korrektheit der Daten*/
		for(uint8_t i=0; i<19; i++){
			uint32_t *flashData = flash_getData();
			for(uint8_t k=0; k<63; k++){
				if(flashData[k] != testData[k]){
					printf("FEHLER i=%d k=%d\n", i, k);
					return 1;
				}
				testData[k] = i*63 + k;
			}
			flash_setDataRow(start, ende, testData);
		}

		/*fuellt den Flash-Sektor mit codierten Daten*/
		for(uint32_t i=0; i<63; i++){ testData[i] = sektor * i; }
		flash_setDataRow(start, ende, testData);
	}

	/*Fuehrt die Rueckueberpfuefung der codierten Daten aus*/
	for(int sektor=0; sektor<19; sektor++){
		flash_init(sektor);
		for(uint32_t i=0; i<63; i++){
			if(flash_getData()[i] != sektor * i){
				printf("FEHLER Sektor=%d\n", i);
				return 1;
			}
		}
	}
	return 0;
}

void sound_PeerGynt(){
	sound_init();
	uint8_t oktave = 1;
	sound_play(7,4,oktave);
	sound_play(4,4,oktave);
	sound_play(2,4,oktave);
	sound_play(0,4,oktave);
	sound_play(2,4,oktave);
	sound_play(4,4,oktave);
	sound_play(7,4,oktave);
	sound_play(4,4,oktave);
	sound_play(2,4,oktave);
	sound_play(0,4,oktave);
	sound_play(2,8,oktave);
	sound_play(4,8,oktave);
	sound_play(2,8,oktave);
	sound_play(4,8,oktave);
	sound_play(7,4,oktave);
	sound_play(4,4,oktave);
	sound_play(7,4,oktave);
	sound_play(9,4,oktave);
	sound_play(4,4,oktave);
	sound_play(9,4,oktave);
	sound_play(7,4,oktave);
	sound_play(4,4,oktave);
	sound_play(2,4,oktave);
	sound_play(0,1,oktave);
}

void st7735_Test(){
	/* st_7735 Test: muss für ili_9341 Test auskommentiert sein
	 * sowie Headereinbindung hier oben und in CMakeLists.txt*/

/*	// Displayinitialisieren
	st7735_init();

	// Farbe definieren
	uint8_t bgcolor[] = {0xF, 0x0, 0x8}; // Rot, Gruen, Blau
	// Hintergrundfarbe setzen
	setBgColor(bgcolor);
	//Display mit Hintergrundfarbe fuellen
	clearScreen();

	// Text definieren
	char *text = "momefilo";
	// Position des Textes (x,y) in Pixel definieren
	uint8_t pos[] = {16, 5};
	// Text schreiben matrix=false / Pos in Pixel
	writeText16x16(pos, text, 8, false, false);

	// Textfarbe definieren
	uint8_t fgcolor[] = {0x0, 0x0, 0xF};
	// Text definieren
	char *text2 = "desing";
	// Position des Textes in (Spalte,Zeile) Zeichen definieren
	uint8_t pos2[] = {4, 2};
	// Textfarbe setzen
	setFgColor(fgcolor);
	// Text schreiben matrix=true / Pos in Zeichen
	writeText12x12(pos2, text2, 6, false, true);
*/
}

void ili9341_Test(){
	/* ili_9341 Test: muss für st7735 Test auskommentiert sein
	 * sowie Headereinbindung hier oben und in CMakeLists.txt*/
/*
	// Display initialisieren
	ili9341_init();

	// Hintergrundfarbe definiren
	uint16_t bgcolor = 0xF818;

	// Hintergrundfarbe setzen
	setBgColor(bgcolor);

	// Display mit Hintergrundfabr fuellen
	clearScreen();

	// Orientierung auf Vertikal setzen
	setOrientation(VERTICAL);

	// Einen Bereich des Displays in Pixel definieren
	uint16_t area[] = {0, 65, 239, 199};

	// Ein Bild auf das Display schreiben
	// Das Bild ist als #include "ili_9341/tools/fraktal_240x135.h" eingebunden
	drawRect(area, BILD);

	// Textfarbe definieren
	uint16_t fgcolor = 0x881F;
	// Textfarbe setzen
	setFgColor(fgcolor);
	// Text definieren
	char *text = "momefilo";
	// Position des Textes (Spalte,Zeile) in Zeichen definieren
	uint16_t pos[] = {4, 1};
	// Text schreiben
	writeText16x16(pos, text, 8, false,true);

	//weiteren Text schreiben
	uint16_t pos2[] = {7, 3};
	char *text2 = "desing";
	setFgColor(0x07E0);
	writeText12x12(pos2, text2, 6, false,true);

	//einen Gradienten Zeichnen
	setOrientation(HORIZONTAL);
	// Bereich des Displays in Pixel definieren
	uint16_t area2[] ={ 0, 0, 119, 239 };
	// zwei Farben definieren
	uint16_t color1 = 0xF81F, color2 = 0x001F;
	// Farbverlauf zeichnen
	paintRectGradient(area2, color1, color2);
*/
}

int main() {
	stdio_init_all();
	sh1106_init();

	// Flash Test: Flash nicht unnötig belasten, auskommentiert
//	if(! flashError()){ printf("Flashtest ok\n"); }


//	st7735_Test()

//	ili9341_Test();

	// buttons Test
//	buttons_init();
	// wartet bis eine Taste gedrueckt wird
//	while(get_Button() == 100){}

	// Sound Test
//	sound_PeerGynt();
}

