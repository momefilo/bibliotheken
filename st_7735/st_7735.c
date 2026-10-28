// st_7735 Bibliothek
#include "font.h"
#include "hardware/spi.h"
#include "hardware/gpio.h"
#include "pico/double.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "st_7735.h"

#define SPI_PORT spi1
#define PIN_SCK	10
#define PIN_SDA	11
#define PIN_A0  12
#define PIN_CS  13
#define PIN_RST 28
#define PIN_LED 5

uint16_t FgColor = 0xFFE0;
uint16_t BgColor = 0x0000;
uint16_t SeColor = 0xF81F;
uint8_t Width, Height;

uint8_t getIliVektor(uint16_t grad, uint8_t begin, uint8_t end){
	int8_t fak = 1; if(grad > 179){ grad -= 180; fak = -1;}
	double_t sinus = sin(grad * (2 * 3.14) / 360);
	double_t cosin = cos(grad * (2 * 3.14) / 360);
	uint8_t cnt = 0;
	for(int i=begin; i<end; i++){
		if(grad == 90){
			IliVektor[cnt][0] = fak * i;
			IliVektor[cnt][1] = 0;
			cnt++;
		}
		else if(grad == 0){
			IliVektor[cnt][0] = 0;
			IliVektor[cnt][1] = fak * -i;
			cnt++;
		}
		else{
			double_t cos_ = -i * cosin;
			double_t sin_ = i * sinus;
			int16_t y = cos_*fak, x = sin_*fak;
			if(i < begin + 1){
				IliVektor[cnt][0] = x;
				IliVektor[cnt][1] = y;
				cnt++;
			}
			else if(i > begin && ((IliVektor[cnt-1][0] != x) || (IliVektor[cnt-1][1] != y))){
				IliVektor[cnt][0] = x;
				IliVektor[cnt][1] = y;
				cnt++;
			}
		}
	}
	return cnt;
}

static inline void cs_select() {
	asm volatile("nop \n nop \n nop");
	gpio_put(PIN_CS, 0);
	asm volatile("nop \n nop \n nop");
}
static inline void cs_deselect() {
	asm volatile("nop \n nop \n nop");
	gpio_put(PIN_CS, 1);
	asm volatile("nop \n nop \n nop");
}
void write_cmd(uint8_t *cmd, int len) {
	cs_select();
	spi_write_blocking(SPI_PORT, cmd, 1);
	if(len > 1){
		gpio_put(PIN_A0, 1);
		spi_write_blocking(SPI_PORT, cmd + (uint8_t)1, len - 1);
		gpio_put(PIN_A0, 0);
	}
	cs_deselect();
}
void set_col(int start, int end){
	uint8_t cmd[5];
	cmd[0] = 0x2A;
	cmd[1] = 0x00;
	cmd[2] = start;
	cmd[3] = 0x00;
	cmd[4] = end;
	write_cmd(cmd, 5);
}
void set_row(int start, int end){
	uint8_t cmd[5];
	cmd[0] = 0x2B;
	cmd[1] = 0x00;
	cmd[2] = start;
	cmd[3] = 0x00;
	cmd[4] = end;
	write_cmd(cmd, 5);
}

void write_font16x16(uint8_t *pos, uint8_t zeichen){
	//create buffer for font
	int len = 16*16*2+1;
	uint8_t buf[len];
	buf[0] = 0x2C;
	int z = 1; // Schleifen-Zaehlvariable
	//write font in buffer
	for(int i=0; i<32; i=i+1){ // 32 Byte umfasst ein 16x16Bit Zeichen
		for(int k=0; k<8; k=k+1){ // Acht Bits pro Byte
			if(FONT16x16[zeichen*32+i] & (0x80 >> k)){ // ist das Bit gesetzt
				buf[z] = (( FgColor & 0xFF00)  >> 8);
				buf[z+1] = FgColor & 0x00FF;
			}
			else{ // Das Bit ist nicht gesetzt
				buf[z] = (( BgColor & 0xFF00)  >> 8);
				buf[z+1] = BgColor & 0x00FF;
			}
			z=z+2;
		}
	}
	//write buffer to display
	set_col(pos[0],pos[0]+15);
	set_row(pos[1],pos[1]+15);
	write_cmd(buf,len);
}
void write_font12x12(uint8_t *pos, uint8_t zeichen){
	//create buffer for font
	int len = 12*12*2+1;
	uint8_t buf[len];
	buf[0] = 0x2C;
	int z = 1; // Schleifen-Zaehlvariable
	//write font in buffer
	for(int i=0; i<18; i=i+1){ // 18 Byte umfasst ein 12x12Bit Zeichen
		for(int k=0; k<8; k=k+1){ // Acht Bits pro Byte
			if(FONT12x12[zeichen*18+i] & (0x80 >> k)){ // ist das Bit gesetzt
				buf[z] = (FgColor & 0xFF00)>>8;
				buf[z+1] = (FgColor & 0x00FF);
			}
			else{ // Das Bit ist nicht gesetzt
				buf[z] = (BgColor & 0xFF00)>>8;
				buf[z+1] = (BgColor & 0x00FF);
			}
			z=z+2;
		}
	}
	//write buffer to display
	set_col(pos[0],pos[0]+11);
	set_row(pos[1],pos[1]+11);
	write_cmd(buf,len);
}
void write_font6x8(uint8_t *pos, uint8_t zeichen){
	uint8_t font_width = 6;
	uint8_t font_height = 8;
	uint8_t leer_col = 0;// Die linke Spalte ist leer da nur 7 bit
	//create buffer for font
	int len = font_width * font_height * 2 + 1;
	uint8_t buf[len];// buffer fuer das Zeichen
	buf[0] = 0x2C;// Startbyte
	int z = 1; // Schleifen-Zaehlvariable
	//write font in buffer
	for(int y=font_height; y>0; y--){  // 6 Byte umfasst ein 6x8Bit Zeichen
		for(int x=0; x<font_width; x++){ // 8 Bits pro Byte
			if(FONT6x8[zeichen][x] & (0x80 >> y)){ // ist das Bit gesetzt
				buf[z] = (FgColor & 0xFF00)>>8;
				buf[z+1] = (FgColor & 0x00FF);
			}
			else{ // Das Bit ist nicht gesetzt
				buf[z] = (BgColor & 0xFF00)>>8;
				buf[z+1] = (BgColor & 0x00FF);
			}
			z=z+2;
		}
	}
	//write buffer to display
	set_col(pos[0], pos[0] + font_width - 1);
	set_row(pos[1], pos[1] + font_height - 1);
	write_cmd(buf,len);
}
void write_font7x11(uint8_t *pos, uint8_t zeichen){
	uint8_t font_ofset = 0x20;// Zeichensatz-Anfang
	uint8_t font_width = 7;
	uint8_t font_height = 11;
	uint8_t leer_col = 1;// Die linke Spalte ist leer da nur 7 bit
	//create buffer for font
	uint16_t leng = font_width * font_height * 2 + 1;
	uint8_t buf[leng];
	buf[0] = 0x2C; // Buffer mit Startbyte
	//write font in buffer
	if(zeichen > 127 || zeichen < font_ofset) zeichen = font_ofset;
	int z = 1;
	for(int y=0; y<font_height; y++){  // 11 Byte umfasst ein 7x11Bit Zeichen
		for(int x=leer_col; x<8; x++){  // 7 Bits pro Byte
			if(FONT7x11[zeichen - font_ofset][y] & (0x01 << (x - 1))){ // ist das Bit gesetzt
				buf[z] = ((FgColor & 0xFF00)  >> 8);
				buf[z+1] = (FgColor & 0x00FF);
			}
			else{ // Das Bit ist nicht gesetzt
				buf[z] = ((BgColor & 0xFF00)  >> 8);
				buf[z+1] = (BgColor & 0x00FF);
			}
			z=z+2;
		}
	}
	set_col(pos[0], pos[0] + font_width - 1);
	set_row(pos[1], pos[1] + font_height - 1);
	write_cmd(buf, leng);
}
void write_font10x16(uint8_t *pos, uint8_t zeichen){
	uint8_t font_ofset = 0x20;
	uint8_t font_width = 10;
	uint8_t font_height = 16;
	uint8_t leer_col = 6;
	//create buffer for font
	int len = font_width * font_height * 2 + 1;
	uint8_t buf[len];
	buf[0] = 0x2C;// Startbyte
	int z = 1;// Schleifen-Zaehlvariable
	//write font in buffer
	for(int i=0; i<32; i=i+1){ // 32 Byte umfasst ein 10x16Bit Zeichen
		int anfang = 0;
		if(i % 2 == 0) anfang = 6;
		for(int k=anfang; k<8; k=k+1){ // zwei oder acht Bits pro Byte
//			if(zeichen < 0x21) zeichen = 0x21; // THIS IS A BUG!
			if(FONT10x16[zeichen-font_ofset][i] & (0x80 >> k)){ // ist das Bit gesetzt
				buf[z] = (FgColor & 0xFF00)>>8;
				buf[z+1] = (FgColor & 0x00FF);
			}
			else{ // Das Bit ist nicht gesetzt
				buf[z] = (BgColor & 0xFF00)>>8;
				buf[z+1] = (BgColor & 0x00FF);
			}
			z=z+2;
		}
	}
	//write buffer to display
	set_col(pos[0],pos[0] + font_width - 1);
	set_row(pos[1],pos[1] + font_height - 1);
	write_cmd(buf,len);
}
void write_font12x16(uint8_t *pos, uint8_t zeichen){
	uint8_t font_width = 12;
	uint8_t font_height = 16;
	uint8_t leer_col = 4;
	//create buffer for font
	int len = font_width * font_height * 2 + 1;
	uint8_t buf[len];
	buf[0] = 0x2C;// Startbyte
	int z = 1;// Schleifen-Zaehlvariable
	//write font in buffer
	for(int i=0; i<32; i=i+1){ // 32 Byte umfasst ein 16x16Bit Zeichen
		int ofset = i + 1;
		int ende = 8;
		if(ofset % 2 == 0) ende = 4;
		for(int k=0; k<ende; k=k+1){ // acht oder vier Bits pro Byte
			if(FONT12x16[zeichen*32+i] & (0x80 >> k)){ // ist das Bit gesetzt
				buf[z] = (FgColor & 0xFF00)>>8;
				buf[z+1] = (FgColor & 0x00FF);
			}
			else{ // Das Bit ist nicht gesetzt
				buf[z] = (BgColor & 0xFF00)>>8;
				buf[z+1] = (BgColor & 0x00FF);
			}
			z=z+2;
		}
	}
	//write buffer to display
	set_col(pos[0],pos[0] + font_width - 1);
	set_row(pos[1],pos[1] + font_height - 1);
	write_cmd(buf,len);
}

void display_clear(){
	int len = Width*Height*2+1;
	uint8_t area[len];
	area[0] = 0x2C;
	for(int i=1; i<len-1; i=i+2){
		area[i] = ((BgColor & 0xFF00)  >> 8);
		area[i+1] = (BgColor & 0x00FF);
	 }
	set_col(0, Width-1);
	set_row(0, Height-1);
	write_cmd(area, len);
}
void display_init(){
	if(true){//spi setup
		spi_init(SPI_PORT, 100* 1000 * 1000);
		gpio_set_function(PIN_SCK, GPIO_FUNC_SPI);
		gpio_set_function(PIN_SDA, GPIO_FUNC_SPI);
		gpio_init(PIN_CS);
		gpio_set_dir(PIN_CS, GPIO_OUT);
		gpio_put(PIN_CS, 1);
		gpio_init(PIN_A0);
		gpio_set_dir(PIN_A0, GPIO_OUT);
		gpio_put(PIN_A0, 0);
		gpio_init(PIN_RST);
		gpio_set_dir(PIN_RST, GPIO_OUT);
		gpio_put(PIN_RST, 1);
	}
	sleep_ms(200);
	if(true){//display init
		uint8_t cmd[2];
		cmd[1] = 0;
		//sleep out
		cmd[0] = 0x11;
		write_cmd(cmd,1);
		sleep_ms(200);
		//display on
		cmd[0]=0x29;
		write_cmd(cmd,1);
		//12-bit colormode
		cmd[0] = 0x3A;
		cmd[1] = 0x05;
		write_cmd(cmd,2);
		sleep_ms(100);
		//flip vertikal&hoizontal
		cmd[0] = 0x36;
		cmd[1] = 0x60;
		write_cmd(cmd,2);
		Width = 160;
		Height = 128;
		sleep_ms(200);
	}
	gpio_init(PIN_LED);
	gpio_set_dir(PIN_LED, true);
	gpio_pull_up(PIN_LED);
	gpio_put(PIN_LED, 1);
	display_clear;
}
void display_orientation(bool hor, bool flip){
	uint8_t cmd[] = {0x36, 0};
	if (hor){
		Width = 160;
		Height = 128;
		if(flip) cmd[1]= 0x60;
		else cmd[1] = 0xA0;
	}
	else{
		Width = 128;
		Height = 160;
		if(flip) cmd[1] = 0xC0;
		else cmd[1] = 0x00;
	}
	write_cmd(cmd, 2);
}
/* if(font == 0) fontwidth = 6; fontheight = 8;
 * if(font == 1) { fontwidth = 7; fontheight = 11;}
   else if(font == 2){ fontwidth = 10; fontheight = 16;}
   else if(font == 3){ fontwidth = 12; fontheight = 12;}
   else if(font == 4){ fontwidth = 12; fontheight = 16;}
   else if(font == 5){ fontwidth = 16; fontheight = 16;}
*/
void display_text(uint8_t *pos, char *text, uint8_t font, bool sel){
	uint16_t tmp_color = FgColor;
	uint8_t leng = strlen(text);
	uint8_t fontwidth = 6;
	uint8_t fontheight = 8;
	if(font == 1){ fontwidth = 7; fontheight = 11;}
	else if(font == 2){ fontwidth = 10; fontheight = 16;}
	else if(font == 3){ fontwidth = 12; fontheight = 12;}
	else if(font == 4){ fontwidth = 12; fontheight = 16;}
	else if(font == 5){ fontwidth = 16; fontheight = 16;}
	if(sel) FgColor = SeColor;
	for(int i=0; i<leng; i++){
		uint8_t mypos[] = {pos[0], pos[1]};
		if(i>0) mypos[0] = mypos[0] + i*fontwidth;
		if(font == 0) write_font6x8( mypos, text[i]);
		else if (font == 1) write_font7x11(mypos, text[i]);
		else if (font == 2) write_font10x16(mypos, text[i]);
		else if (font == 3) write_font12x12(mypos, text[i]);
		else if (font == 4) write_font12x16(mypos, text[i]);
		else if (font == 5) write_font16x16(mypos, text[i]);
	}
	FgColor = tmp_color;
}
void display_drawBytes(uint8_t *area, uint8_t *data){
	set_col(area[0], area[2]);
	set_row(area[1], area[3]);
	write_cmd(data, (area[2]-area[0]+1)*(area[3]-area[1]+1)*2);
}
void display_drawColor(uint8_t *area, uint16_t color){
	int len = (area[2] - area[0] + 1)*(area[3] - area[1] + 1)*2+1;
	uint8_t buf[len];
	buf[0] = 0x2C;
	for(int j=1; j<len-2; j+=2){
		buf[j] = ((color & 0xFF00) >> 8);
		buf[j+1] = (color & 0x00FF);
	}
	set_col(area[0], area[2]);
	set_row(area[1], area[3]);
	write_cmd(buf, len);
}
void display_off(bool off){
	uint8_t cmd[] = {0x29};
	uint8_t on = 1;
	if(off){ cmd[0] = 0x28; on = 0;}
	gpio_put(PIN_LED, on);
	write_cmd(cmd, 1);
}
void display_invert(bool inv){
	uint8_t cmd[] = {0x20};
	if(inv) cmd[0] = 0x21;
	write_cmd(cmd, 1);
}
void display_pixel(uint8_t x, uint8_t y, uint16_t color){
	uint8_t area[] = { 0x2C, (color & 0xFF00) >> 8, color & 0x00FF};
	set_col(x, x);
	set_row(y, y);
	write_cmd(area, 3);
}
void display_kreis(uint8_t x, uint8_t y, uint8_t radius, uint8_t width, uint16_t color){
	for(int i=0; i<360; i += 1){
		uint8_t len = getIliVektor(i, radius, radius + width);
		for(int p=0; p<len; p++){ display_pixel(IliVektor[p][0] + x, IliVektor[p][1] + y, color); }
	}
}
void display_line(uint8_t x, uint8_t y, uint16_t a, uint8_t begin, uint8_t end, uint8_t width, uint16_t color){
	uint8_t vlen = getIliVektor(a, begin, end);
	uint8_t mid  = (width - 1) / 2;
	uint8_t cnt = 0;
	for(int i=0; i<vlen; i++){
		uint8_t startx = IliVektor[i][0] + x;
		uint8_t endx = startx;
		uint8_t starty = IliVektor[i][1] + y;
		uint8_t endy = starty;
		if((a > (7*6) && a < (23*6)) || (a > (37*6) && a < (53*6))){
			starty = IliVektor[i][1] + y - width + mid;
			endy = IliVektor[i][1] + y + width - mid;
		}
		else{
			startx = IliVektor[i][0] + x - width + mid;
			endx = IliVektor[i][0] + x + width - mid;
		}
		uint8_t area[] = { startx, starty, endx, endy};
		display_drawColor(area, color);
	}
}

void display_fgcolor(uint16_t color){ FgColor = color; }
void display_bgcolor(uint16_t color){ BgColor = color; }
void display_secolor(uint16_t color){ SeColor = color; }
void getFontsize(uint8_t ret[2], uint8_t font){
	if(font == 0){ ret[0] = 6; ret[1] = 8; }
	else if(font == 1){ ret[0] = 7; ret[1] = 11; }
	else if(font == 2){ ret[0] = 10; ret[1] = 16; }
	else if(font == 3){ ret[0] = 12; ret[1] = 12; }
	else if(font == 4){ ret[0] = 12; ret[1] = 16; }
	else if(font == 5){ ret[0] = 16; ret[1] = 16; }
}

listView_t *newListView(char *head,  char **list, int len, uint8_t area[4]){
	int width = area[2] - area[0];
	int height = area[3] - area[1];
	int xofset = area[0], yofset = area[1];
	int space = 2;// pixel zwischen den zeilen
	int cfont = 1, hfont = 2;
	uint8_t charsize[2];
	getFontsize(charsize, cfont);
	int cols = (width)/charsize[0];
	int rows = (height)/(charsize[1]+space);
	int headXofset = 0;
	int headHeight = 0;
	uint8_t headCharsize[2];
	if(head){
		getFontsize(headCharsize, hfont);
		headXofset = (width-strlen(head)*headCharsize[0]) / 2;
		rows = (height-headCharsize[1]-space)/(charsize[1]+space);
	}
	listView_t *lv = (listView_t*)malloc(sizeof(listView_t));
	listView_init(lv, head, list, len, rows, area);
	display_clear();
	if(head){
		uint8_t pos[] = {xofset+headXofset, yofset};
		display_text(pos, head, hfont, false);
		yofset += (headCharsize[1]+space);
	}
	for(int i=0; i<lv->vp_len; i++){
		bool sel = false;
		if(i == lv->vp_selection) sel = true;
		uint8_t pos[] = {xofset, yofset + i*(charsize[1]+space)};
		display_text(pos, lv->viewport[i], cfont, sel);
	}
	return lv;
}
int listView_up(listView_t *lv, bool up){
	int ret = listView_scroll(lv, up, 1);
	listView_show(lv);
	return ret;
}
void listView_show(listView_t *lv){
	int width = lv->area[2] - lv->area[0];
	int height = lv->area[3] - lv->area[1];
	int xofset = lv->area[0], yofset = lv->area[1];
	int space = 2;// pixel zwischen den zeilen
	int cfont = 1, hfont = 2;
	uint8_t charsize[2];
	getFontsize(charsize, cfont);
	int cols = (width)/charsize[0];
	int rows = (height)/(charsize[1]+space);
	int headXofset = 0;
	int headHeight = 0;
	uint8_t headCharsize[2];
	display_clear();
	if(lv->head){
		getFontsize(headCharsize, hfont);
		headXofset = (width-strlen(lv->head)*headCharsize[0]) / 2;
		rows = (height-headCharsize[1]-space)/(charsize[1]+space);
		uint8_t pos[] = {xofset+headXofset, yofset};
		display_text(pos, lv->head, hfont, false);
		yofset += (headCharsize[1]+space);
	}
	for(int i=0; i<lv->vp_len; i++){
		bool sel = false;
		if(i == lv->vp_selection) sel = true;
		uint8_t pos[] = {xofset, yofset + i*(charsize[1]+space)};
		display_text(pos, lv->viewport[i], cfont, sel);
	}
}

