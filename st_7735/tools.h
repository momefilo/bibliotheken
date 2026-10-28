#ifndef my_tools_h
#define my_tools_h 1

#include "pico/stdlib.h"

typedef struct{
	uint8_t area[4];
	char *head;
	int items_len;
	int vp_topItem;
	char **items;
	int selection;
	int vp_len;
	char **viewport;
	int vp_selection;
	int space;
}
listView_t;

void listView_init(listView_t *lv, char *head, char **items, int items_len, int viewportlen, uint8_t area[4]);
int listView_deinit(listView_t *lv);
int listView_scroll(listView_t *lv, bool up, uint8_t len);

// fügt *text mit Leerzeichen aufgefüllte Zeilenumbrüche "\n" nach width Zeichen hinzu.
// Rückgabewert ist die Anzahl an Zeilen
int text_break(char *text, uint8_t weite);
// wandelt den durch Zeilenumbrüche "\n" (*delemiter) getrennten *text in ein Array[zeilen][weite] um
char **text_rows(char *text, char *delemiter, int zeilen, int weite);
#endif
