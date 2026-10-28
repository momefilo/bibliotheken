**LCD st_7735**\
listview_t ist eine vertikale Zeichenketten-Scrolliste

**initialisierung eines listView_t:**\
listView_t lv = newListView(*char header, char **strings, int string_anzahl, uint8_t area[4]);

Der *header kann NULL sein
Die Listelemente **strings werden nicht auf ueberlaenge geprueft!!
Die Lange der Liste string_anzahl
area ist der Zeichenbereich der Liste
(uint8_t area[] = {x_start, y_start, x_end, y_end};)
Die Zeichengroeße der Liste ist 9x11 Pixel, des headers 10x16 Pixel
Der selektierte Listeneintrag ist in lv->selection gespechert

**scrollen im listView_t:**\
listView_up(listView_t *lv, bool up)

lv das listView zum scrollen**\
up true=scroll nach oben, false = scroll nach unten

**neu aufs display zeichen:**\
listView_show(listView_t *lv);
