// tools
#include "tools.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


void listView_init(listView_t *lv, char *head, char **items, int items_len, int viewportlen, uint8_t area[4]){
	lv->head = head;
	for(int i=0; i<4; i++) lv->area[i] = area[i];
	lv->items = items;
	lv->items_len = items_len;
	lv->vp_len = viewportlen;
	lv->selection = 0;
	lv->vp_selection = 0;
	lv->vp_topItem = 0;
	lv->space = 2;
	lv->viewport = (char**)malloc((viewportlen) * sizeof(char*));
	for(int i=0; i<lv->vp_len; i++){
		lv->viewport[i] = lv->items[i];
	}
}

int listView_deinit(listView_t *lv){ free(lv->viewport); lv = NULL; }

int listView_move(listView_t *lv, bool up, uint8_t len){
	int ret = len;
	if(up){
		if(len > lv->vp_topItem){ ret = lv->vp_topItem; lv->vp_topItem = 0;}
		else lv->vp_topItem -= len;
	}
	else{
		if(len + lv->vp_topItem + lv->vp_len > lv->items_len){
			ret = lv->items_len - (lv->vp_topItem + lv->vp_len);
			lv->vp_topItem = lv->items_len - lv->vp_len;
		}
		else lv->vp_topItem += len;
	}
	int z = 0;
	for(int i=lv->vp_topItem; i<(lv->vp_topItem + lv->vp_len); i++){
		lv->viewport[z] = lv->items[i];
		z++;
	}
	return ret;
}

int listView_scroll(listView_t *lv, bool up, uint8_t len){
	int ret = len;
	int maxmove = lv->vp_len - 2*lv->space;
	if(up){
		if(len > lv->selection){
			ret = lv->selection;
			lv->selection = 0;
			lv->vp_selection = 0;
			listView_move(lv, up, lv->vp_topItem);
		}
		else{
			if(lv->vp_selection - len >=  lv->space  || lv->vp_topItem == 0){
				lv->selection -= len;
				lv->vp_selection = lv->selection - lv->vp_topItem;
			}
			else{
				if(maxmove > lv->vp_topItem) maxmove = lv->vp_topItem;
				if(len > maxmove) len = maxmove;
				listView_move(lv, up, maxmove);
				lv->selection -= len;
				lv->vp_selection = lv->selection - lv->vp_topItem;
				ret = len;
			}
		}
	}
	else{
		if(lv->selection + len > lv->items_len - 1){
			ret = lv->items_len - 1 - lv->selection;
			lv->selection = lv->items_len - 1;
			lv->vp_selection = lv->vp_len - 1;
		}
		else{
			if(lv->vp_selection + len <  lv->vp_len - lv->space  \
						|| lv->vp_topItem + lv->vp_len-1 == lv->items_len - 1){
				lv->selection += len;
				lv->vp_selection = lv->selection - lv->vp_topItem;
			}
			else{
				int tmpmove = lv->items_len - 1 - (lv->vp_topItem + lv->vp_len - 1);
				if(maxmove > tmpmove) maxmove = tmpmove;
				if(len > maxmove) len = maxmove;
				listView_move(lv, up, maxmove);
				lv->selection += len;
				lv->vp_selection = lv->selection - lv->vp_topItem;
				ret = len;
			}
		}
	}
	return ret;
}

// fügt *text mit Leerzeichen aufgefüllte Zeilenumbrüche "\n" nach width Zeichen hinzu.
// Rückgabewert ist die Anzahl an Zeilen
int text_break(char *text, uint8_t weite){
	int char_count = 0, zeilen = 1;
    char* token = strtok(text, " ");
    char tmp[1000];
    strcpy(tmp, "");

    while (token != NULL){
		int token_len = strlen(token);
		if(token_len + char_count < weite){
			char t[token_len+1];
			sprintf(t, "%s ",token, strlen(token));
			strcat(tmp, t);
			char_count += token_len + 1;;
			token = strtok(NULL, " ");
			continue;
		}
		else if(token_len > weite){
			int z =  strlen(tmp);
			for(int i=0; i<token_len; i++){
				if(char_count >= weite){
					tmp[z] = '\n';
					char_count = 0;
					zeilen++;
					z++;
				}
				tmp[z] = token[i];
				char_count++;
				z++;
			}
			tmp[z] = ' ';
			char_count++;
			token = strtok(NULL, " ");
			continue;
		}
		else{
			for(int i=char_count; i<weite-1; i++){
				char t[] = " ";
				strcat(tmp, t);
			}
			zeilen++;
			char t[token_len+3];
			sprintf(t, "\n%s ",token);
			strcat(tmp, t);
			char_count = token_len + 1;
		}
		token = strtok(NULL, " ");
	}
	strncpy(text, tmp, strlen(tmp)-1);
	return zeilen;
}
// wandelt den durch Zeilenumbrüche "\n" getrennten **text in ein Array um
char **text_rows(char *text, char *delemiter, int zeilen, int weite){
	char **result = (char**)malloc(zeilen * sizeof(char*));
	for(int i=0; i<zeilen; i++) result[i] = (char*)malloc((weite + 1) * sizeof(char));
	char* token = strtok(text, delemiter);
	int z = 0;
    while (token != NULL){
		result[z] = token;
		z++;
		token = strtok(NULL, delemiter);
	}
	return result;
}


