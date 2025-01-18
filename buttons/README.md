**buttons**\
Diese Bibliothek bietet Softwareentprellung von Tastern\
Zur Anpassung an die Taster sind der "buttons.c"-Datei zwei Zeiten definiert\
WaitTime: ist die Zeit in ms die Mindest-Impulsdauer um als Tastendruck erkannt zu werden\
HoldTime: ist die Zeit in ms die nach erkanntem Tastendruck vergehen muss bis ein Neuer erkannt werden kann\
Die GPIO-Zuordnung erfolg in der "buttons.h"-Datei

**void buttons_init()**\
Ist einmalig vor Benutzung der Funktionen aufzurufen

**void get_Button()**\
Gibt den, laut GPIO-Zuordnung, aktuell gedrueckten Taster zuruek oder 100 wenn kein Taster betaetigt ist.

