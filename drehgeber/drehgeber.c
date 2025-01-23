// drehgeber Bibliothek
#include "drehgeber.h"
#include "pico/stdlib.h"

/* Die Mindest-Pulsdauer in ms
 * An Hardware anzupassen */
uint8_t SwPulsdauer = 100, DtPulsdauer = 1, CklPulsdauer = 1;

/* Der Mindestabstand zwischen den pulsen in ms
 * An Hardware anzupassen */
uint16_t SwPausendauer = 400, DtPausendauer = 4, CklPausendauer = 4;

/* Der Mindestabstnd zwischen den Schaltwechseln
 * An Hardware anzupassen */
uint16_t Wechseldauer = 100;

/* Mindestanzahl der Pulse pro Richtung
 * An Hardware anzupassen */
uint8_t Tiks = 3;

// Pulszaehler
uint8_t DtTiks = 0, CklTiks = 0;

// Pulsflanken-Speicher
bool CklDown = false, DtDown= false, SwDown = false;

int drehgeber_get(){
	// CKL vor DT = RECHTS
	if( (! gpio_get(PIN_CKL)) && gpio_get(PIN_DT)){
		DtTiks = 0;
		DtDown = false;
		// Begin des Pulses
		if( ! CklDown){
			CklDown = true;
			sleep_ms(CklPulsdauer);
		}// Wenn der Puls laenger als die Pulsdauer anstand wird er erfasst
		else if(CklTiks < Tiks){
			CklTiks++;
			CklDown = false;
			sleep_ms(CklPausendauer);
		}// Sind vier Pulse erfasst ist der Geber um eine Stelle veraendert
		else{
			CklTiks = 0;
			CklDown = false;
			sleep_ms(Wechseldauer);
			return IN_RIGHT;
		}
	}// DT vor CKL = LINKS
	else if( (! gpio_get(PIN_DT)) && gpio_get(PIN_CKL)){
		CklTiks = 0;
		CklDown = false;
		// Begin des Pulses
		if( ! DtDown){
			DtDown = true;
			sleep_ms(DtPulsdauer);
		}// Wenn der Puls laenger als die Pulsdauer anstand wird er erfasst
		else if(DtTiks < Tiks){
			DtTiks++;
			DtDown = false;
			sleep_ms(DtPausendauer);
		}// Sind vier Pulse erfasst ist der Geber um eine Stelle veraendert
		else{
			DtTiks = 0;
			DtDown = false;
			sleep_ms(Wechseldauer);
			return IN_LEFT;
		}
	}// Ende der Bewegung
	else if(gpio_get(PIN_DT) && gpio_get(PIN_CKL)){
		CklDown = false;
		DtDown = false;
		DtTiks = 0;
		CklTiks = 0;
		if(! gpio_get(PIN_SW)){
			if( ! SwDown){
				SwDown = true;
				sleep_ms(SwPulsdauer);
			}
			else{
				SwDown = false;
				sleep_ms(Wechseldauer);
				return IN_PRESS;
			}
		}
	}
	return 0;
}

void drehgeber_init(){
	gpio_init(PIN_CKL);
	gpio_set_input_enabled(PIN_CKL, true);
	gpio_set_pulls(PIN_CKL, false,false);
	gpio_init(PIN_DT);
	gpio_set_input_enabled(PIN_DT, true);
	gpio_set_pulls(PIN_DT, false,false);
	gpio_init(PIN_SW);
	gpio_set_input_enabled(PIN_SW, true);
	gpio_set_pulls(PIN_SW, false,false);
}
