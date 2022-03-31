#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"
#include "text.h"

struct txt{
    char type;
    int id;
    float x;
    float y;
    float anchor;
    char *corp;
    char *corb;
    char *text;
};
typedef struct txt Texto;

Text1 criaTxt(){
    Texto *new_txt = calloc(1, sizeof(Texto*));
    return new_txt;
}

Item buildText(FILE *arq, Text1 txt, char infos[], char *eptr){
    printf("\nInicio Build Text\n");
    Texto *Text = (Texto*) txt;

    Text->type = infos[0];

    strcpy(infos, "");
    fscanf(arq, "%d", infos);
    Text->id = atoi(infos);

    strcpy(infos, "");
    fscanf(arq, "%f", infos);
    Text->x = strtod(infos, &eptr);

    strcpy(infos, "");
    fscanf(arq, "%f", infos);
    Text->y = strtod(infos, &eptr);

    strcpy(infos, "");
    fscanf(arq, "%f", infos);
    Text->anchor = strtod(infos, &eptr);

    strcpy(infos, "");
    fscanf(arq, "%s", infos);
    Text->corb = infos;

    strcpy(infos, "");
    fscanf(arq, "%s", infos);
    Text->corp = infos;

    strcpy(infos, "");
    fscanf(arq, "%s", infos);
    Text->text = infos;

    strcpy(infos, "");

    return Text;
}

int getTxtID(Text1 t){
    Texto *txt = (Texto*) t;

    return txt->id;
}

float getTxtX(Text1 t){
    Texto *txt = (Texto*) t;

    return txt->x;
}

float getTxtY(Text1 t){
    Texto *txt = (Texto*) t;

    return txt->y;
}

float getTxtANCHOR(Text1 t){
    Texto *txt = (Texto*) t;

    return txt->anchor;
}

char getTxtFILL(Text1 t){
    Texto *txt = (Texto*) t;

    return txt->corp;
}

char getTxtEDGE(Text1 t){
    Texto *txt = (Texto*) t;

    return txt->corb;
}

char getTxtTEXT(Text1 t){
    Texto *txt = (Texto*) t;

    return txt->text;
}