#ifndef my_st_7735_h
#define my_st_7735_h 1

#include "tools.h"

#define HORIZONTAL 0
#define VERTICAL 1



// Array fuer Kreis und winkel
static int8_t IliVektor[160][2];

void display_init();
void display_orientation(bool hor, bool flip);
void display_clear();
// font 0=6x8, 1=7x11, 2=10x16, 3=12x12, 4=12x16, 5=16x16
void display_text(uint8_t *pos, char *text, uint8_t font, bool sel);
void display_drawBytes(uint8_t *area, uint8_t *data);
void display_drawColor(uint8_t *area, uint16_t color);
void display_pixel(uint8_t x, uint8_t y, uint16_t color);
void display_kreis(uint8_t x, uint8_t y, uint8_t radius, uint8_t width, uint16_t color);
void display_line(uint8_t x, uint8_t y, uint16_t a, uint8_t begin, uint8_t end, uint8_t width, uint16_t color);
void display_off(bool off);
void display_invert(bool inv);
void display_setBgColor(uint16_t color);
void display_setFgColor(uint16_t color);
void display_setSeColor(uint16_t color);
uint16_t display_getSeColor();
uint16_t display_getFgColor();
uint16_t display_getBgColor();
void getFontsize(uint8_t ret[2], uint8_t font);
listView_t *listView_new(char *head,  char **list, int len, uint8_t area[4]);
int listView_up(listView_t *lv, bool up);
void listView_show(listView_t *lv, uint8_t update);
typedef struct{
	char *header;
	char **texte;
	int32_t *values;
	char **items;
	char **options;
	int items_len;
	int options_len;
	int options_count;//anzahl der ersten items mit option
	listView_t *listView;
	uint8_t area[4];
	int rowwidth;
}menu_t;
menu_t *menu_new(char *header, char **menutext, int32_t *menuvalue, int len, char **options, int optonlen, uint8_t optioncount, uint8_t area[4]);
int menu_select(menu_t *menu, bool up);
void menu_change(menu_t *menu, int32_t value);
void menu_changeValue(menu_t *menu, int index, int32_t value);

#endif
