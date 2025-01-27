// sh_1106 Bibliothek
#include "sh_1106.h"
#include "font12x16.h"
#include "font8x8.h"
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/gpio.h"
#include <stdio.h>
#include <string.h>

void snd_cmd(uint8_t cmd){
	uint8_t buf[2] = {0x80, cmd};
	//Attempt to write specified number of bytes to address, blocking.
	i2c_write_blocking(i2c_get_instance(I2C_BUS), I2C_ADR, buf, 2, false);
}

void snd_data(uint8_t *data, uint16_t len){
	uint8_t buf[len +1];
	buf[0] = 0x40;
	for(int i=1; i<len+1; i++){ buf[i] = data[i-1]; }
	i2c_write_blocking(i2c_get_instance(I2C_BUS), I2C_ADR, buf, len + 1, false);
}

void set_startpoit(uint8_t col, uint8_t page){
	snd_cmd(0x0F & col); // col low-bits
	snd_cmd(0x10 | (0x0F & (col>>4))); // col high-bits
	snd_cmd(0xB0 | (0x0F & page)); //page bits
}

void sh1106_clear_screen(){
	uint8_t buf[132];
	for(int i=0; i<132; i++){ buf[i] = 0x00; }
	for(int i=0; i<8; i++){
		set_startpoit(0,i);
		snd_data(buf, 132);
	}
}

void sh1106_datacol(uint8_t col, uint8_t page, uint8_t *data){
	set_startpoit(col+2, page);
	snd_data(data, 1);
}

void sh1106_datapage(uint8_t startcol, uint8_t page, uint8_t *data, uint8_t len){
	set_startpoit(startcol+2, page);
	snd_data(data, len);
}

void sh1106_text12x16(uint8_t *pos, uint8_t *text, bool invert){
	// Zeichen fuer Zeichen
	for(int i=0; i<strlen(text); i++){
		//Es werden je 8x12 Bit fuer jede Page geschrieben
		for(int page=0; page<2; page++){
			uint8_t *array;
			if(page<1){// Dies ist die obere Page
				set_startpoit(i*12 + pos[0]*12 + 2, pos[1] * 2);
				array = FONT_12x16[text[i]];
				if(invert)array = FONT_12x16invert[text[i]];
			}
			else{// Des ist die untere Page
				set_startpoit(i*12 + pos[0]*12 + 2, pos[1] * 2 + 1);
				array = FONT_12x16[text[i]] + 12;
				if(invert)array = FONT_12x16invert[text[i]] + 12;
			}
			snd_data(array, 12);
		}
	}
}

void sh1106_text8x8(uint8_t *pos, uint8_t *text, bool invert){
	// Zeichen fuer Zeichen
	for(int i=0; i<strlen(text); i++){
		//Zeichen um 90grad im Uhrzeiersinn drehen
		uint8_t *zeichen = FONT_8x8[text[i]];
		if(invert)zeichen = FONT_8x8invert[text[i]];
		uint8_t zeichen_gedreht[8];
		for(int k=0; k<8; k++){
			uint8_t col = 0, colmask = 0x01, rowmask = 0x01;
			for(int a=0; a<8; a++){
				if(zeichen[a] & rowmask << k){
					col = col | colmask << a;
				}
			}
			zeichen_gedreht[k] = col;
		}

		set_startpoit(i*8 + pos[0]*8 + 2, pos[1]);
		snd_data(zeichen_gedreht, 8);
	}
}

void sh1106_init(){
	i2c_init(i2c_get_instance(I2C_BUS) , I2C_BAUDRATE);
    gpio_init(PIN_SCL);
    gpio_init(PIN_SDA);
    gpio_set_function(PIN_SCL, GPIO_FUNC_I2C);
    gpio_set_function(PIN_SDA, GPIO_FUNC_I2C);
    gpio_pull_up(PIN_SCL);
    gpio_pull_up(PIN_SDA);

    snd_cmd(0xAE); // off
    snd_cmd(0x40); // start_line 0
    snd_cmd(0xD3); // ofset double byte
    snd_cmd(0x00); // ofset 0
    snd_cmd(0xA1); // von re nach li
    snd_cmd(0xC8); // von unten nach oben
    snd_cmd(0x81); // kontrast double byte
    snd_cmd(0xFF); // voll kontrast
    snd_cmd(0xA4); // ganzen Display nutzen
    snd_cmd(0xA6); // nicht invertiert (A7 = invertiert)
    snd_cmd(0xAF); // on
}

