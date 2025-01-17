**rgb2header**\
Dieses Programm wandelt die 24Bit-RGB-Pixel einer Eingabedatei
in R5G6B5-Pixel um die viele LCD-Displays darstellen koennen,
und schreibt sie als text in eine Headerdatei.\
Die Headerdatei enthält ein Datenarray aus uint8 Werten welche die
Bitfolge Hexadezimal darstellt. So können diese Daten mit der
"#include"-Anweisung in Binarys mit einkompiliert werden.\
Wenn die daten in einen ili9314-LCD angezeigt werden, muessen nur die
ersten fuenf code-Zeilen der Graphik entsprechend in "rgb2header.c"
editiert werden. Ansonsten ist in Code-Zeile 53 zusaetzlich das erste
Byte (hier 0x2C) entsprechend zu aendern.\
Als Beispielt dient im Programmcode der "rgb2header.c" fraktal_240x135.data,
das Programm muss mit "gcc -o rgb2header rgb2header.c" kompiliert werden damit es ausfuehrbar wird
