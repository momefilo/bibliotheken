**Pi Pico Bibliotheken zum Einbinden in Programmcode**\
In der Datei "bibliotheken.c" finden sich Test- und Beispielcode

* **momefilo_flash**\
Komfortable Flashverwaltung mit einer Lese- und zwei Schreibfunktionen\
Siehe Genaueres in momefilo_flash/README.md

* **buzzer_sound**\
ist eine kleine Bibliothek die drei Oktaven auf einem am GPIO des Pico
angeschlossenen Passiv-Buzzer spielt. Der GPIO wird in buzzer_sound.h
zu Beginn festgelegt\
Siehe Genaueres in buzzer_sound/README.md

* **buttons**\
Software-Entprellung von Tastern\
Es werden vier Taster abgefragt die mit zwei Zeitvariablen entprellt sind.\
Genaueres ist in buttons/README.md zu finden

* **drehgeber**\
Eine Entprellung für Drehgeber anhand des KY-040\
Mit der Pulsdauer, der Pauasendauer zwischen den Pulsen,\
der Anzahl an Pulsen pro Richtung sowie der Pausendauer zwischen
den Schalzwechseln/Geberstellungen laesst sich das kleine Programm
an spezifische Hardware anpassen

* **LCD ili_9341**\
Diese Bibliothekt bietet je eine Textfunktion mit 12x12 und eine mit 16x16
Pixel Zeichengroesse. Die Text- und Hintergrundfarbe sind änderbar, und
es können Rechtecke, Farbverlaufsrechtecke und beliebige Farbinformation
gezeichnet werden. Die Touchfunktion des Displays kann aktiviert und die
Koordinaten der Berührung abgefragt werden.\
Siehe Genaueres in ili_9341/README.md

* **ili_9341/tools/rgb2header**\
NON PICO, muss auf dem Entwicklungs-PC ausgefuehrt werden!\
Dieses Programm wandelt die 24Bit-RGB-Pixel einer Eingabedatei
in R5G6B5-Pixel um die viele LCD-Displays darstellen koennen,
und schreibt sie als text in eine Headerdatei. Die Headerdatei enthält ein Datenarray aus uint8 Werten welche die
Bitfolge Hexadezimal darstellt. So können diese Daten mit der
"#include"-Anweisung in Binarys mit einkompiliert werden.\
Wenn die daten in einen ili9314-LCD angezeigt werden muessen nur die
ersten fuenf code-Zeilen, der Graphik entsprechend in "rgb2header.c"
editiert werden. Ansonsten ist in Code-Zeile 53 zusaetzlich das Startbyte
(hier 0x2C) entsprechend zu aendern.\
Eine genaue Dokumentation findet sich in den Kommentaren der "[rgb2header.c](https://github.com/momefilo/bibliotheken/blob/main/ili_9341/tools/rgb2header.c)"-Datei.
Als Beispiel dient im Programmcode der das fraktal_240x135.data,
das Programm muss mit **"gcc -o rgb2header rgb2header.c"** kompiliert werden damit es ausfuehrbar wird

* **LCD st_7735**\
Diese Bibliothek bietet je eine Textfunktion mit 12x12 und eine mit 16x16
Pixel Zeichengroesse. Die Text- und Hintergrundfarbe sind änderbar.
Und ein Funktion um beliebige Pixelinformation auf das display zu schreiben.\
Siehe Genaueres in st7735/README.md

* **OLED sh_1106**\
Bietet je eine Schreibfunktion in font8x8 und 12x16 nur im Querformat\
Die SH1106 Displays ist in Zeilen mit je acht Pixel an Hoehe aufgeteilt.\
Die Bibliothek Teilt die Pixelmatrix des Displays in eine Zeichenmatrix
und verlang als Parameter der Schreibfunktionen die Position in
dieser Zeichenmatrix. Bei einem 132x64 Display gibt es vier Zeilen mit
jeweils 11 Zeichen im Font 8x16 und jeweils doppelt so viele in font 8x8
So ist die Position (2,3) Das zweite Zeichen von rechts in der dritten Zeile.
