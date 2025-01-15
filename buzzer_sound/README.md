**buzzer_sound**
ist eine kleine Bibliothe die drei Oktaven auf einem am GPIO des Pico
angeschlossenen Passiv-Buzzer spielt. Der GPIO word in buzzer_sound.h
zu Beginn festgelegt

Die Bibliothek bietet zwei Funktionen welche folgend erläutert sind:

**void sound_init()**\
Muss einmal vor Benutzung von sound_play() aufgerufen sein

**void sound_play(uint8_t note, uint8_t dauer, uint8_t oktave)**\
Gibt Töne auf dem Passiv-Buzzer aus\
note: 0=C bis 11=B\
dauer: teilt die 1,5 Sekuden Tondauer\
oktave: 0=Grundoktave, 1=eine Oktave hoeher, 2=zwei Oktaven hoeher
