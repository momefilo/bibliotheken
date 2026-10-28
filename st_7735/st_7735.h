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
void display_fgcolor(uint16_t color);
void display_bgcolor(uint16_t color);
void display_secolor(uint16_t color);
void getFontsize(uint8_t ret[2], uint8_t font);
listView_t *newListView(char *head,  char **list, int len, uint8_t area[4]);
int listView_up(listView_t *lv, bool up);
void listView_show(listView_t *lv);

#endif
