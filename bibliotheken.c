// bibliotheken
#include "momefilo_flash/momefilo_flash.h"
#include <stdio.h>
#include "pico/stdlib.h"

void flashTest(){
	/* Array aus 62 Integerwerten zur Prüfung*/
	uint32_t ChkData[63];
	for(uint32_t i=0; i<63; i++){ ChkData[i]=i; }

	/* Initialisiert den letzten Sektor im Flash*/
	flash_init(0);

	/* Schreibt ChkData vollstaendig in die
	 * 63 Speicherbereiche des Flash*/
	uint8_t start = 0;
	uint8_t ende = 62;
	flash_setDataRow(start, ende, ChkData);

	/* Ueberschreibt das Array 19 mal
	 * und überprüft dabei die Korrektheit der Daten*/
	for(uint8_t i=0; i<19; i++){
		uint32_t *flashData = flash_getData();
		for(uint8_t k=0; k<63; k++){
			if(flashData[k] != ChkData[k]){ printf("FEHLER i=%d k=%d\n", i, k);}
			ChkData[k] = i*63 + k;
		}
		flash_setDataRow(start, ende, ChkData);
	}
}

int main() {
	stdio_init_all();
	// flashTest-programm
	for(int i=0; i<19; i++){
		printf("\n---Sektor %d ---\n", i);
		flash_init(i);
		flashTest();
	}
	printf("Flashtest ende\n");
}

