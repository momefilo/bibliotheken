#include <stdio.h>
#include "fontNew.h"

int main(){
	FILE *fileNew = fopen("dateiNew.txt", "w");
	if(fileNew==0){printf("Dateifehler\n");return 0;}
	for(int charpos=0; charpos<0xFE; charpos++){
		unsigned char toppage[]={0,0,0,0,0,0,0,0,0,0,0,0};
		unsigned char downpage[]={0,0,0,0,0,0,0,0,0,0,0,0};
		unsigned char set_lup_mask = 0x01;
		unsigned char set_ldown_mask = 0x01;
		unsigned char set_rup_mask = 0x01;
		unsigned char set_rdown_mask = 0x01;
		fprintf(fileNew, "\n{");
		for(int bytepos=0; bytepos<32; bytepos++){
			unsigned char get_left_mask = 0x80;
			unsigned char get_right_mask = 0x80;
			if(bytepos % 2 == 0){
				if(bytepos < 16){
					int newpos = 0;
					for(int bitpos=0; bitpos<8; bitpos++){
						if( ! (bitpos==1 || bitpos==4 || bitpos==7)){
							if(font_New[charpos * 32 + bytepos] & get_left_mask){ toppage[newpos] |= set_lup_mask;}
							newpos++;
						}
						get_left_mask >>= 1;
					}
					set_lup_mask <<= 1;
				}
				else{
					int newpos = 0;
					for(int bitpos=0; bitpos<8; bitpos++){
						if( ! (bitpos==1 || bitpos==4 || bitpos==7)){
							if(font_New[charpos * 32 + bytepos] & get_left_mask){ downpage[newpos] |= set_ldown_mask;}
							newpos++;
						}
						get_left_mask >>= 1;
					}
					set_ldown_mask <<= 1;
				}
			}
			else{
				if(bytepos < 16){
					int newpos = 0;
					for(int bitpos=0; bitpos<4; bitpos++){
						if( ! (bitpos==2)){
							if(font_New[charpos * 32 + bytepos] & get_right_mask){ toppage[newpos+5] |= set_rup_mask;}
							newpos++;
						}
						get_right_mask >>= 1;
					}
					set_rup_mask <<= 1;
				}
				else{
					int newpos = 0;
					for(int bitpos=0; bitpos<4; bitpos++){
						if( ! (bitpos==2)){
							if(font_New[charpos * 32 + bytepos] & get_right_mask){ downpage[newpos+5] |= set_rdown_mask;}
							newpos++;
						}
						get_right_mask >>= 1;
					}
					set_rdown_mask <<= 1;
				}
			}
		}
		for(int i=0; i<8; i++){ fprintf(fileNew, "0x%02X,", toppage[i]);}
		fprintf(fileNew, "\n");
		for(int i=0; i<8; i++){ fprintf(fileNew, "0x%02X,", downpage[i]);}
		fprintf(fileNew, "},");
	}


//	fprintf(file16x16, "0x%02X,", toppage16x16[i]);
	fclose(fileNew);
/*
	FILE *file16x16 = fopen("datei16x16.txt", "w");
	FILE *file12x12 = fopen("datei12x12.txt", "w");
	FILE *file12x16 = fopen("datei12x16.txt", "w");
	if(file16x16==0){printf("Dateifehler\n");return 0;}
	if(file12x12==0){printf("Dateifehler\n");return 0;}
	if(file12x16==0){printf("Dateifehler\n");return 0;}
	printf("datei geöffnet\n");

	for(int buchstabe=0; buchstabe<95; buchstabe++){
		unsigned char toppage16x16[]={0,0,0,0,0,0,0,0};
		unsigned char downpage16x16[]={0,0,0,0,0,0,0,0};
		unsigned char toppage12x12[]={0,0,0,0,0,0,0,0};
		unsigned char downpage12x12[]={0,0,0,0,0,0,0,0};
		for(int i=0; i<8; i++){
			if( FONT_8x8[buchstabe][i] & 0x80){ downpage16x16[i] = 0xC0; downpage12x12[i] = 0x0C;}
			if( FONT_8x8[buchstabe][i] & 0x40){ downpage16x16[i] |= 0x30; downpage12x12[i] |= 0x02;}
			if( FONT_8x8[buchstabe][i] & 0x20){ downpage16x16[i] |= 0x0C; downpage12x12[i] |= 0x01; toppage12x12[i] = 0x80;}
			if( FONT_8x8[buchstabe][i] & 0x10){ downpage16x16[i] |= 0x03; toppage12x12[i] |= 0x40;}

			if( FONT_8x8[buchstabe][i] & 0x08){ toppage16x16[i] = 0xC0; toppage12x12[i] |= 0x30;}
			if( FONT_8x8[buchstabe][i] & 0x04){ toppage16x16[i] |= 0x30; toppage12x12[i] |= 0x08;}
			if( FONT_8x8[buchstabe][i] & 0x02){ toppage16x16[i] |= 0x0C; toppage12x12[i] |= 0x06;}
			if( FONT_8x8[buchstabe][i] & 0x01){ toppage16x16[i] |= 0x03; toppage12x12[i] |= 0x01;}
		}
		fprintf(file16x16, "{");
		fprintf(file12x12, "{");
		fprintf(file12x16, "{");
		for(int i=0; i<8; i++){
			//write 16x16 toppage
			fprintf(file16x16, "0x%02X,", toppage16x16[i]);
			fprintf(file16x16, "0x%02X,", toppage16x16[i]);
			//write 12x12 toppage
			fprintf(file12x12, "0x%02X,", toppage12x12[i]);
			if(i % 2 == 0){ fprintf(file12x12, "0x%02X,", toppage12x12[i]);}
			//write 12x16 toppage
			fprintf(file12x16, "0x%02X,", toppage16x16[i]);
			if(i % 2 == 0){ fprintf(file12x16, "0x%02X,", toppage16x16[i]);}
		}
		fprintf(file16x16, "\n");
		fprintf(file12x12, "\n");
		fprintf(file12x16, "\n");
		for(int i=0; i<8; i++){
			fprintf(file16x16, "0x%02X,", downpage16x16[i]);
			fprintf(file16x16, "0x%02X,", downpage16x16[i]);
			fprintf(file12x12, "0x%02X,", downpage12x12[i]);
			if(i % 2 == 0){ fprintf(file12x12, "0x%02X,", downpage12x12[i]);}
			//write 12x16 downpage
			fprintf(file12x16, "0x%02X,", downpage16x16[i]);
			if(i % 2 == 0){ fprintf(file12x16, "0x%02X,", downpage16x16[i]);}
		}
		fprintf(file16x16, "},\n");
		fprintf(file12x12, "},\n");
		fprintf(file12x16, "},\n");
	}
	fclose(file16x16);
	fclose(file12x12);
	fclose(file12x16);
	printf("datei geschrieben\n");
*/
}

