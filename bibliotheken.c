// bibliotheken
#include "buzzer_sound/buzzer_sound.h"
#include "momefilo_flash/momefilo_flash.h"
#include <stdio.h>
#include "pico/stdlib.h"
#include <stdlib.h>

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

int main() {
	stdio_init_all();

	// Flash Test
//	if(! flashError()){ printf("Flashtest ok\n"); }

	// Sound Test
	sound_PeerGynt();
}

