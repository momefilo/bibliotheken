/* Eine Entprellung für Drehgeber anhand des KY-040
 * Mit der Pulsdauer, der Pauasendauer zwischen den Pulsen,
 * der Anzahl an Pulsen pro Richtung sowie der Pausendauer zwischen
 * den Schalzwechseln/Geberstellungen laesst sich das kleine Programm
 * an spezifische Hardware anpassen */

#ifndef my_drehgeber_h
#define my_drehgeber_h 1

#include "pico/stdlib.h"

#define PIN_CKL 16
#define PIN_DT 17
#define PIN_SW 18
#define IN_LEFT 1
#define IN_RIGHT 2
#define IN_PRESS 3

void drehgeber_init();

int drehgeber_get();

#endif
