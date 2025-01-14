**Pi Pico c-lib zur Flashverwaltung**

Mit unter will man Ergebnisse auf dem Pico nichtflüchtig speichern um
z.B. Spielstände oder Nutzereinstellungen, die oft nur wenige Byte
umfassen nach einem Neustart wieder zur Verfügung haben. Doch der
Flashspeicher des Pico hat seine Tücken, so lassen sich immer nur ganze
256 Byte schreiben und ganze 4096 Byte löschen um eine Änderung des
Flashspeichers zu erreichen. Auch soll der Flash des Pico nur wenige
solcher Zyklen, im Vergleich zu Datenträgern ohne Verlust vertragen.
Aufgrund dieser Speicherorganisation des Flashs habe ich eine Bibliothek
entwickelt welche die 4kb umfassenden Sektoren des Flash verwaltet, und
je ein 252 Byte umfassendes Array aus 63 uint32 Variablen pro Sektor zur
Verfügung stellt. Die Bibliothek verwaltet die Sektoren von 0 und am
Adressende des Flash beginnend, so das der Bedarf von oben herab zum
Programm-speicher wachen kann und löscht den Sektor nur mit jeder 16ten
Änderung des Arrays. Das Ziel der „Lebensverlängerung" des Flash ist
fragwürdig, da ich nicht weiß ob die Beanspruchung nur beim Löschvorgang
oder auch beim Schreibvorgang die Zyklenzahl mindert. Die Bibliothek
unterbindet während der Lösch/Schreib-Vorgänge alle Interrupts, das
durch Auskommentieren der Zeilen 68 und 86 in momefilo_flash.c geändert
werden kann.

Die Bibliothek bietet vier Funktionen welche folgend erläutert sind:

**void flash_init(uint8_t sektor)**

Der Sektor des Flash muss durch Aufruf dieser Funktion mit seiner id als
Parameter initialisiert sein bevor er mit den folgenden drei Funktionen
genutzt werden kann. Ein wechsel zwischen Sektoren ist einfach durch
wiederholten Aufruf dieser Funktion möglich

**uint32_t \*flash_getData()**

Diese Funktion gibt die Anfangsadresse des uint32 Array zurück anhand
welcher mit den \[0-62\]-Indizes operiert werden kann (uint32_t Datum_i
= flash_getData()\[i\])

**void flash_setData(uint8_t id, uint32_t data)**

Speicher data an der Stelle id im Array des Flash

**void flash_setDataRow(uint8_t start, uint8_t end, uint32_t \*data)**

Speichert das Array Data von dessen Beginn an, in das Array des
Flashspeicher von start bis end
