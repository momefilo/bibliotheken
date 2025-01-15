#ifndef my_buzzer_sound_h
#define my_buzzer_sound_h 1
#include "pico/stdlib.h"

#define BUZZER_PIN 5

/* Initialisiert den BUZZER_PIN
 * und muss einmal  vor nutzung
 * von sound_play() aufgerufen sein*/
void sound_init();

/* spielt die
 * note: 0=C bis 11=B mit einer Dauer von 1,5 Sekunden.
 * dauer: teilt die Dauer (1,5sec/dauer)
 * oktave: 0=Grundoktave, 1=eine Oktave hoeher, 2=zwei Oktaven hoeher*/
void sound_play(uint8_t note, uint8_t dauer, uint8_t oktave);

#endif
