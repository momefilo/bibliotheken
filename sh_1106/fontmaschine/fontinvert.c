#include <stdio.h>
#include "font12x16.h"
#include "font8x8.h"

int conf_12x16(){
	FILE *fileNew = fopen("dateiInvert.txt", "w");
	if(fileNew==0){printf("Dateifehler\n");return 0;}

	for(int charpos=0; charpos<0xFE; charpos++){
		fprintf(fileNew, "\n{");
		for(int bytepos=0; bytepos<24; bytepos++){
			unsigned char erg= ~(FONT_12x16[charpos][bytepos]);
			fprintf(fileNew, "0x%02X,", erg);
		}
		fprintf(fileNew, "},");
	}
	fclose(fileNew);
}
int conf_8x8(){
	FILE *fileNew = fopen("dateiInvert8x8.txt", "w");
	if(fileNew==0){printf("Dateifehler\n");return 0;}

	for(int charpos=0; charpos<0xFE; charpos++){
		fprintf(fileNew, "\n{");
		for(int bytepos=0; bytepos<8; bytepos++){
			unsigned char erg= ~(FONT_8x8[charpos][bytepos]);
			fprintf(fileNew, "0x%02X,", erg);
		}
		fprintf(fileNew, "},");
	}
	fclose(fileNew);
}
int main(){
	conf_8x8();
	return 0;
}
