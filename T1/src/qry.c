#include "qry.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "circle.h"
#include "fila_circ.h"
#include "line.h"
#include "list.h"
#include "rectangle.h"
#include "svg.h"
#include "system.h"
#include "text.h"

struct pg {
    double x;
    double y;
};
typedef struct pg Ponto;

struct r {
    double x, y, w, h;
};
typedef struct r Rec;

void readComands(FILE *qry_dir, Lista r, Lista c, Lista l, Lista t, FILE *svg, FILE *txt) {
    printf("\n--- INICIO READ QRY ---\n");
    Fila_Circular poligono = criaFila(200);
    Lista selecRec = criaLista();
    Lista selecCirc = criaLista();
    Lista selecLine = criaLista();
    Lista selecTxt = criaLista();
    Lista selAux = criaLista();

    while (!feof(qry_dir)) {
        char *comando[10][30];
        char *eptr;

        fscanf(qry_dir, "%s", comando);

        if (strcmp(comando, "inp") == 0) {  // Inserir no poligono (fila insere no fim)
            printf("\n%s\n", comando);      // *FEITO*
            inp(txt, svg, qry_dir, comando, poligono, r, c, l, t);

        } else if (strcmp(comando, "rmp") == 0) {  // Remove uma coordenada do poligono (primeira da fila)
            printf("\n%s\n", comando);             // *FEITO???*
            rmp(txt, svg, comando, poligono);

        } else if (strcmp(comando, "pol") == 0) {  // Produz um conjunto de linhas e insere no poligono
            printf("\n%s\n", comando);             // *fazer linhas de preenchimento*
            pol(txt, svg, qry_dir, comando, eptr, poligono);

        } else if (strcmp(comando, "clp") == 0) {  // Remove todas as coordenadas
            printf("\n%s\n", comando);             // *FEITO*
            clp(txt, poligono);

        } else if (strcmp(comando, "sel") == 0) {  // Seleciona as figuras inteiramente dentro da regiao
            printf("\n%s\n", comando);             // *FEITO*
            sel(txt, svg, qry_dir, comando, eptr, selAux, selecRec, selecCirc, selecLine, selecTxt, r, c, l, t);

        } else if (strcmp(comando, "sel+") == 0) {  // bla bla ta igual ao sel
            printf("\n%s\n", comando);              // *FEITO????*
            selplus(txt, svg, qry_dir, comando, eptr, selAux, selecRec, selecCirc, selecLine, selecTxt, r, c, l, t);

        } else if (strcmp(comando, "dels") == 0) {  // Remove todas as figuras selecionadas
            printf("\n%s\n", comando);              // *FEITO ARRUMARRRRR seg fault*
            dels(txt, selecRec, selecCirc, selecLine, selecTxt);

        } else if (strcmp(comando, "dps") == 0) {  // Cria novas formas e bla bla
            printf("\n%s\n", comando);             // *FEITO*
            dps(txt, svg, qry_dir, comando, eptr, selecRec, selecCirc, selecLine, selecTxt);

        } else if (strcmp(comando, "ups") == 0) {  // Altera cor e translada as figuras selecionadas
            printf("\n%s\n", comando);             // *FEITO?? parece q ta errado*
            ups(txt, svg, qry_dir, comando, eptr, selecRec, selecCirc, selecLine, selecTxt);
        }
    }

    fclose(qry_dir);
    fclose(txt);
    free(poligono);
    free(selecRec);
    free(selecCirc);
    free(selecLine);
    free(selecTxt);
}

void inp(FILE *txt, FILE *svg, FILE *arq, char *infos[], Fila_Circular q, Lista r, Lista c, Lista l, Lista t) {
    printf("--- INICIO INP ---\n");
    int i, id_aux;
    double x, y;
    double radius = 1.00;
    char *stroke = "red";
    char *fill = "black";
    Item pnt;

    fscanf(arq, "%s", infos);
    i = atoi(infos);

    fprintf(txt, "\n[*] inp\n");

    for (Cell auxC1 = getFirst(r); auxC1 != NULL; auxC1 = getNext(r, auxC1)) {
        Item auxI1 = getInfo(auxC1);

        id_aux = getRectID(auxI1);
        if (i == id_aux) {
            x = getRectX(auxI1);
            y = getRectY(auxI1);
            pnt = criaPonto(x, y);

            enfila_circ(q, pnt);
            fprintf(txt, "Inserido ponto em: x = %lf, y = %lf\n", x, y);
            fprintf(svg, "\t<circle cx=\"%lf\" cy=\"%lf\" r=\"%lf\" stroke=\"%s\" fill=\"%s\" fill-opacity=\"75%%\" />\n", x, y, radius, stroke, fill);
        }
    }

    for (Cell auxC2 = getFirst(c); auxC2 != NULL; auxC2 = getNext(c, auxC2)) {
        Item auxI2 = getInfo(auxC2);

        id_aux = getCircID(auxI2);
        if (i == id_aux) {
            x = getCircX(auxI2);
            y = getCircY(auxI2);
            pnt = criaPonto(x, y);

            enfila_circ(q, pnt);
            fprintf(txt, "Inserido ponto em: x = %lf, y = %lf\n", x, y);
            fprintf(svg, "\t<circle cx=\"%lf\" cy=\"%lf\" r=\"%lf\" stroke=\"%s\" fill=\"%s\" fill-opacity=\"75%%\" />\n", x, y, radius, stroke, fill);
        }
    }

    for (Cell auxC3 = getFirst(t); auxC3 != NULL; auxC3 = getNext(t, auxC3)) {
        Item auxI3 = getInfo(auxC3);

        id_aux = getTxtID(auxI3);
        if (i == id_aux) {
            x = getTxtX(auxI3);
            y = getTxtY(auxI3);
            pnt = criaPonto(x, y);

            enfila_circ(q, pnt);
            fprintf(txt, "Inserido ponto em: x = %lf, y = %lf\n", x, y);
            fprintf(svg, "\t<circle cx=\"%lf\" cy=\"%lf\" r=\"%lf\" stroke=\"%s\" fill=\"%s\" fill-opacity=\"75%%\" />\n", x, y, radius, stroke, fill);
        }
    }

    for (Cell auxC4 = getFirst(l); auxC4 != NULL; auxC4 = getNext(l, auxC4)) {
        Item auxI4 = getInfo(auxC4);

        id_aux = getLineID(auxI4);
        if (i == id_aux) {
            x = getLineX(auxI4);
            y = getLineY(auxI4);
            double x2 = getLineFINALX(auxI4);
            double y2 = getLineFINALY(auxI4);
            if (x < x2) {
                pnt = criaPonto(x, y);
                fprintf(txt, "Inserido ponto em: x = %lf, y = %lf\n", x, y);
                fprintf(svg, "\t<circle cx=\"%lf\" cy=\"%lf\" r=\"%lf\" stroke=\"%s\" fill=\"%s\" fill-opacity=\"75%%\" />\n", x, y, radius, stroke, fill);
            } else if (x2 < x) {
                pnt = criaPonto(x2, y2);
                fprintf(txt, "Inserido ponto em: x = %lf, y = %lf\n", x2, y2);
                fprintf(svg, "\t<circle cx=\"%lf\" cy=\"%lf\" r=\"%lf\" stroke=\"%s\" fill=\"%s\" fill-opacity=\"75%%\" />\n", x2, y2, radius, stroke, fill);
            } else if (x == x2) {
                if (y < y2) {
                    pnt = criaPonto(x, y);
                    fprintf(txt, "Inserido ponto em: x = %lf, y = %lf\n", x, y);
                    fprintf(svg, "\t<circle cx=\"%lf\" cy=\"%lf\" r=\"%lf\" stroke=\"%s\" fill=\"%s\" fill-opacity=\"75%%\" />\n", x, y, radius, stroke, fill);
                } else if (y2 < y) {
                    pnt = criaPonto(x2, y2);
                    fprintf(txt, "Inserido ponto em: x = %lf, y = %lf\n", x2, y2);
                    fprintf(svg, "\t<circle cx=\"%lf\" cy=\"%lf\" r=\"%lf\" stroke=\"%s\" fill=\"%s\" fill-opacity=\"75%%\" />\n", x2, y2, radius, stroke, fill);
                }
            }
            enfila_circ(q, pnt);
        }
    }
    // printf("%d\n", i);
}

Item criaPonto(double x, double y) {
    Ponto *new_point = calloc(1, sizeof(Ponto));

    new_point->x = x;
    new_point->y = y;

    // printf("npx %lf\n", new_point->x);
    // printf("npy %lf\n", new_point->y);

    return new_point;
}

void rmp(FILE *txt, FILE *svg, char *infos[], Fila_Circular q) {
    printf("--- INICIO RMP ---\n");
    Item aux = desenfila_circ(q);
    Ponto *pnt = (Ponto *)aux;
    double rx, ry;
    rx = pnt->x;
    ry = pnt->y;

    fprintf(txt, "\n[*] rmp\n");
    fprintf(txt, "Removido ponto de coordenadas: x = %lf, y = %lf\n", rx, ry);
}

void pol(FILE *txt, FILE *svg, FILE *arq, char *infos[], char *eptr, Fila_Circular q) {
    printf("--- INICIO POL ---\n");
    int i;
    double d, e;
    char corb[15], corp[15];

    fscanf(arq, "%s", infos);
    i = atoi(infos);

    fscanf(arq, "%s", infos);
    d = strtod(infos, &eptr);

    fscanf(arq, "%s", infos);
    e = strtod(infos, &eptr);

    fscanf(arq, "%s", infos);
    strcpy(corb, infos);

    fscanf(arq, "%s", infos);
    strcpy(corp, infos);

    fprintf(txt, "\n[*] pol\n");

    int size = getSize(q);
    int aux = 0, aux2 = 1;
    double menorY = 99999, maiorY = 0;
    double x1, x2, y1, y2;
    Item p1, p2;
    while (aux2 != size) {
        p1 = getElement(q, aux);   // 0 1 2
        p2 = getElement(q, aux2);  // 1 2 3
        aux = aux2;
        aux2++;
        x1 = getpX(p1);
        x2 = getpX(p2);
        y1 = getpY(p1);
        y2 = getpY(p2);

        fprintf(txt, "Criada linha de borda: x1 = %lf, y1 = %lf, x2 = %lf, y2 = %lf, stroke = %s\n", x1, y1, x2, y2, corb);
        fprintf(svg, "\t<line id=\"%d\" x1=\"%lf\" y1=\"%lf\" x2=\"%lf\" y2=\"%lf\" stroke=\"%s\" />\n", i, x1, y1, x2, y2, corb);
        i++;

        if (y1 < menorY) {
            menorY = y1;
        }
        if (y2 < menorY) {
            menorY = y2;
        }
        if (y1 > maiorY) {
            maiorY = y2;
        }
        if (y2 > maiorY) {
            maiorY = y2;
        }
    }
    p1 = getElement(q, 0);
    x1 = getpX(p1);
    y1 = getpY(p1);
    fprintf(txt, "Criada linha de borda: x1 = %lf, y1 = %lf, x2 = %lf, y2 = %lf, stroke = %s\n", x2, y2, x1, y1, corb);
    fprintf(svg, "\t<line id=\"%d\" x1=\"%lf\" y1=\"%lf\" x2=\"%lf\" y2=\"%lf\" stroke=\"%s\" />\n", i, x2, y2, x1, y1, corb);

    double Nlinhas = (maiorY - menorY) / d;
    printf("Nlinhas %lf\n", Nlinhas);

    // printf("id %d\n", i);
    // printf("d %lf\n", d);
    // printf("e %lf\n", e);
    // printf("corb %s\n", corb);
    // printf("corp %s\n", corp);
}

double getpX(Item n) {
    Ponto *p1 = (Ponto *)n;

    return p1->x;
}

double getpY(Item n) {
    Ponto *p1 = (Ponto *)n;

    return p1->y;
}

void clp(FILE *txt, Fila_Circular q) {
    printf("--- INICIO CLP ---\n");
    removeTudo(q);

    fprintf(txt, "\n[*] clp\n");
    fprintf(txt, "Todas as coordenadas do polígono corrente removidas\n");
}

void sel(FILE *txt, FILE *svg, FILE *arq, char *infos[], char *eptr, Lista a, Lista sR, Lista sC, Lista sL, Lista sT, Lista r, Lista c, Lista l, Lista t) {
    printf("--- INICIO SEL ---\n");
    double x, y, w, h;
    double radius = 1.75000;
    char stroke[] = "red";
    char fill[] = "white";

    fscanf(arq, "%s", infos);
    x = strtod(infos, &eptr);

    fscanf(arq, "%s", infos);
    y = strtod(infos, &eptr);

    fscanf(arq, "%s", infos);
    w = strtod(infos, &eptr);

    fscanf(arq, "%s", infos);
    h = strtod(infos, &eptr);

    criaRecaux(a, x, y, w, h);

    fprintf(svg, "\t<rect x=\"%lf\" y=\"%lf\" width=\"%lf\" height=\"%lf\" stroke=\"%s\" fill=\"%s\" fill-opacity=\"3%%\" />\n", x, y, w, h, stroke, fill);
    fprintf(txt, "\n[*] sel\n");
    fprintf(txt, "Retângulo de seleção: x = %lf, y = %lf, h = %lf, w = %lf, stroke = %s\n", x, y, h, w, stroke);

    for (Cell auxC1 = getFirst(r); auxC1 != NULL; auxC1 = getNext(r, auxC1)) {
        Item auxI1 = getInfo(auxC1);

        int idR = getRectID(auxI1);
        double recX = getRectX(auxI1);
        double recY = getRectY(auxI1);
        double recHeight = getRectHEIGHT(auxI1);
        double recWidth = getRectWIDTH(auxI1);

        if (((x + w) >= (recX + recWidth)) && (recX >= x)) {
            if (((y + h) >= (recY + recHeight)) && (recY >= y)) {
                if (x <= recX && y <=recY) {
                    insereFim(sR, auxI1);

                    fprintf(svg, "\t<circle cx=\"%lf\" cy=\"%lf\" r=\"%lf\" stroke=\"%s\" fill=\"%s\" fill-opacity=\"25%%\" />\n", recX, recY, radius, stroke, fill);
                    fprintf(txt, "Selecionado: Retângulo id %d, x = %lf, y = %lf, height = %lf, width = %lf\n", idR, recX, recY, recHeight, recWidth);
                }
            }
        }
    }

    for (Cell auxC2 = getFirst(c); auxC2 != NULL; auxC2 = getNext(c, auxC2)) {
        Item auxI2 = getInfo(auxC2);

        int idC = getCircID(auxI2);
        double circX = getCircX(auxI2);
        double circY = getCircY(auxI2);
        double circRadius = getCircRADIUS(auxI2);

        if ((x + w) >= (circX + circRadius) && (x) <= (circX - circRadius)) {
            if ((y + h) >= (circY + circRadius) && (y) <= (circY - circRadius)) {
                if (x <= circX && y <= circY) {
                    insereFim(sC, auxI2);

                    fprintf(svg, "\t<circle cx=\"%lf\" cy=\"%lf\" r=\"%lf\" stroke=\"%s\" fill=\"%s\" fill-opacity=\"25%%\" />\n", circX, circY, radius, stroke, fill);
                    fprintf(txt, "Selecionado: Círculo id %d, x = %lf, y = %lf, radius = %lf\n", idC, circX, circY, circRadius);
                }
            }
        }
    }

    for (Cell auxC3 = getFirst(t); auxC3 != NULL; auxC3 = getNext(t, auxC3)) {
        Item auxI3 = getInfo(auxC3);

        int idT = getTxtID(auxI3);
        double txtX = getTxtX(auxI3);
        double txtY = getTxtY(auxI3);

        if ((x + w) >= (txtX) && (y + h) >= (txtY)) {
            if (x <= txtX && y <= txtY) {
                insereFim(sT, auxI3);

                fprintf(svg, "\t<circle cx=\"%lf\" cy=\"%lf\" r=\"%lf\" stroke=\"%s\" fill=\"%s\" fill-opacity=\"25%%\" />\n", txtX, txtY, radius, stroke, fill);
                fprintf(txt, "Selecionado: Texto id %d,  x = %lf, y = %lf\n", idT, txtX, txtY);
            }
        }
    }

    for (Cell auxC4 = getFirst(l); auxC4 != NULL; auxC4 = getNext(l, auxC4)) {
        Item auxI4 = getInfo(auxC4);

        int idL = getLineID(auxI4);
        double linX1 = getLineX(auxI4);
        double linY1 = getLineY(auxI4);
        double linX2 = getLineFINALX(auxI4);
        double linY2 = getLineFINALY(auxI4);

        if ((x + w) >= (linX1) && (y + h) >= (linY1)) {
            if ((x + w) >= (linX2) && (y + h) >= (linY2)) {
                if (x <= linX1 && y <= linY1 && x <= linX2 && y <= linY2) {
                    insereFim(sL, auxI4);

                    fprintf(svg, "\t<circle cx=\"%lf\" cy=\"%lf\" r=\"%lf\" stroke=\"%s\" fill=\"%s\" fill-opacity=\"25%%\" />\n", linX1, linY1, radius, stroke, fill);
                    fprintf(txt, "Selecionado: Linha id %d, x1 = %lf, y1 = %lf, x2 = %lf, y2 = %lf\n", idL, linX1, linY1, linX2, linY2);
                }
            }
        }
    }
    // printf("x %lf\n", x);
    // printf("y %lf\n", y);
    // printf("w %lf\n", w);
    // printf("h %lf\n", h);
}

Item criaRecaux(Lista a, double x, double y, double w, double h) { //mais gambiarra
    Rec *new_rec = calloc(1, sizeof(Rec));

    new_rec->x = x;
    new_rec->y = y;
    new_rec->w = w;
    new_rec->h = h;

    insereFim(a, new_rec)
    return new_rec;
}

double getrX(Item r) {
    Rec *aux = (Rec *) r;

    return aux->x;
}

double getrY(Item r) {
    Rec *aux = (Rec *) r;

    return aux->y;
}

double getrW(Item r) {
    Rec *aux = (Rec *) r;

    return aux->w;
}

double getrH(Item r) {
    Rec *aux = (Rec *) r;

    return aux->h;
}

void selplus(FILE *txt, FILE *svg, FILE *arq, char *infos[], char *eptr, Lista a, Lista sR, Lista sC, Lista sL, Lista sT, Lista r, Lista c, Lista l, Lista t) {
    printf("--- INICIO SEL+ ---\n");
    double x, y, w, h;
    double selx, sely, selw, selh; //x y w h vindos do comando sel COMO?????????
    double finalX, finalY, finalH, finalW;
    double radius = 1.75000;
    char stroke[] = "red";
    char fill[] = "white";

    fscanf(arq, "%s", infos);
    x = strtod(infos, &eptr);

    fscanf(arq, "%s", infos);
    y = strtod(infos, &eptr);

    fscanf(arq, "%s", infos);
    w = strtod(infos, &eptr);

    fscanf(arq, "%s", infos);
    h = strtod(infos, &eptr);

    for (Cell auxA1 = getFirst(a); auxC1 != NULL; auxA1 = getNext(a, auxA1)) { //gambiarraaaaa
        Item auxB1 = getInfo(auxA1);

        selx = getrX(auxA1);
        sely = getrY(auxA1);
        selw = getrW(auxA1);
        selh = getrH(auxA1);

        if (y == sely) {
            finalW = selw + w;
            finalH = h;
            finalY = y;
            if (x < selx) {
                finalX = x;
            } else {
                finalX = selx;
            }
            removeCelula(a, auxA1);
            criaRecaux(a, finalX, finalY, finalH, finalH);
        } else if (x == selx) {
            finalH = selh + h;
            finalW = w;
            finalX = x;
            if (y < sely) {
                finalY = y;
            } else {
                finalY = sely;
            }
            removeCelula(a, auxA1);
            criaRecaux(a, finalX, finalY, finalH, finalH);
        } else {
            finalX = x;
            finalY = y;
            finalH = h;
            finalW = w;
            criaRecaux(a, finalX, finalY, finalH, finalH);
        }
    }

    fprintf(svg, "\t<rect x=\"%lf\" y=\"%lf\" width=\"%lf\" height=\"%lf\" stroke=\"%s\" fill=\"%s\" fill-opacity=\"3%%\" />\n", x, y, w, h, stroke, fill);
    fprintf(txt, "\n[*] sel+\n");
    fprintf(txt, "Retângulo de seleção: x = %lf, y = %lf, h = %lf, w = %lf, stroke = %s\n", x, y, h, w, stroke);

    for (Cell auxC1 = getFirst(r); auxC1 != NULL; auxC1 = getNext(r, auxC1)) {
        Item auxI1 = getInfo(auxC1);

        int idR = getRectID(auxI1);
        double recX = getRectX(auxI1);
        double recY = getRectY(auxI1);
        double recHeight = getRectHEIGHT(auxI1);
        double recWidth = getRectWIDTH(auxI1);

        if (((finalX + finalW) >= (recX + recWidth))) {
            if (((finalY + finalH) >= (recY + recHeight))) {
                if (finalX <= recX && finalY <= recY) {
                    insereFim(sR, auxI1);

                    fprintf(svg, "\t<circle cx=\"%lf\" cy=\"%lf\" r=\"%lf\" stroke=\"%s\" fill=\"%s\" fill-opacity=\"25%%\" />\n", recX, recY, radius, stroke, fill);
                    fprintf(txt, "Selecionado: Retângulo id %d, x = %lf, y = %lf, height = %lf, width = %lf\n", idR, recX, recY, recHeight, recWidth);
                }
            }
        }
    }

    for (Cell auxC2 = getFirst(c); auxC2 != NULL; auxC2 = getNext(c, auxC2)) {
        Item auxI2 = getInfo(auxC2);

        int idC = getCircID(auxI2);
        double circX = getCircX(auxI2);
        double circY = getCircY(auxI2);
        double circRadius = getCircRADIUS(auxI2);

        if ((finalX + finalH) >= (circX + circRadius) && (finalX) <= (circX - circRadius)) {
            if ((finalY + finalH) >= (circY + circRadius) && (finalY) <= (circY - circRadius)) {
                if(finalX <= circX && finalY <= circY) {
                    insereFim(sC, auxI2);

                    fprintf(svg, "\t<circle cx=\"%lf\" cy=\"%lf\" r=\"%lf\" stroke=\"%s\" fill=\"%s\" fill-opacity=\"25%%\" />\n", circX, circY, radius, stroke, fill);
                    fprintf(txt, "Selecionado: Círculo id %d, x = %lf, y = %lf, radius = %lf\n", idC, circX, circY, circRadius);
                }
            }
        }
    }

    for (Cell auxC3 = getFirst(t); auxC3 != NULL; auxC3 = getNext(t, auxC3)) {
        Item auxI3 = getInfo(auxC3);

        int idT = getTxtID(auxI3);
        double txtX = getTxtX(auxI3);
        double txtY = getTxtY(auxI3);

        if ((finalX + finalW) >= (txtX) && (finalY + finalH) >= (txtY)) {
            if (finalX <= txtX && finalY <= txtY) {
                insereFim(sT, auxI3);

                fprintf(svg, "\t<circle cx=\"%lf\" cy=\"%lf\" r=\"%lf\" stroke=\"%s\" fill=\"%s\" fill-opacity=\"25%%\" />\n", txtX, txtY, radius, stroke, fill);
                fprintf(txt, "Selecionado: Texto id %d,  x = %lf, y = %lf\n", idT, txtX, txtY);
            }
        }
    }

    for (Cell auxC4 = getFirst(l); auxC4 != NULL; auxC4 = getNext(l, auxC4)) {
        Item auxI4 = getInfo(auxC4);

        int idL = getLineID(auxI4);
        double linX1 = getLineX(auxI4);
        double linY1 = getLineY(auxI4);
        double linX2 = getLineFINALX(auxI4);
        double linY2 = getLineFINALY(auxI4);

        if ((finalX + finalW) >= (linX1) && (finalY + finalH) >= (linY1)) {
            if ((finalW + finalW) >= (linX2) && (finalY + finalH) >= (linY2)) {
                if(finalX <= linX1 && finalY <= linY1 && finalX <= linX2 && finalY <= linY2) {
                    insereFim(sL, auxI4);

                    fprintf(svg, "\t<circle cx=\"%lf\" cy=\"%lf\" r=\"%lf\" stroke=\"%s\" fill=\"%s\" fill-opacity=\"25%%\" />\n", linX1, linY1, radius, stroke, fill);
                    fprintf(txt, "Selecionado: Linha id %d, x1 = %lf, y1 = %lf, x2 = %lf, y2 = %lf\n", idL, linX1, linY1, linX2, linY2);
                }
            }
        }
    }
    // printf("x %lf\n", x);
    // printf("y %lf\n", y);
    // printf("w %lf\n", w);
    // printf("h %lf\n", h);
}

void dels(FILE *txt, Lista sR, Lista sC, Lista sL, Lista sT) {
    printf("--- INICIO DELS ---\n");

    fprintf(txt, "\n[*] dels\n");

    for (Cell auxC1 = getFirst(sR); auxC1 != NULL; auxC1 = getNext(sR, auxC1)) {
        Item auxI1 = getInfo(auxC1);

        int idR = getRectID(auxI1);
        double recX = getRectX(auxI1);
        double recY = getRectY(auxI1);
        double recHeight = getRectHEIGHT(auxI1);
        double recWidth = getRectWIDTH(auxI1);
        
        removeCelula(sR, auxI1);
        fprintf(txt, "Removido: Retângulo id %d, x = %lf, y = %lf, height = %lf, width = %lf\n", idR, recX, recY, recHeight, recWidth);
    }

    for (Cell auxC2 = getFirst(sC); auxC2 != NULL; auxC2 = getNext(sC, auxC2)) {
        Item auxI2 = getInfo(auxC2);

        int idC = getCircID(auxI2);
        double circX = getCircX(auxI2);
        double circY = getCircY(auxI2);
        double circRadius = getCircRADIUS(auxI2);

        removeCelula(sC, auxI2);
        fprintf(txt, "Removido: Círculo id %d, x = %lf, y = %lf, radius = %lf\n", idC, circX, circY, circRadius);
    }

    for (Cell auxC3 = getFirst(sT); auxC3 != NULL; auxC3 = getNext(sT, auxC3)) {
        Item auxI3 = getInfo(auxC3);

        int idT = getTxtID(auxI3);
        double txtX = getTxtX(auxI3);
        double txtY = getTxtY(auxI3);

        removeCelula(sT, auxI3);
        fprintf(txt, "Removido: Texto id %d,  x = %lf, y = %lf\n", idT, txtX, txtY);
    }

    for (Cell auxC4 = getFirst(sL); auxC4 != NULL; auxC4 = getNext(sL, auxC4)) {
        Item auxI4 = getInfo(auxC4);

        int idL = getLineID(auxI4);
        double linX1 = getLineX(auxI4);
        double linY1 = getLineY(auxI4);
        double linX2 = getLineFINALX(auxI4);
        double linY2 = getLineFINALY(auxI4);

        removeCelula(sL, auxI4);
        fprintf(txt, "Removida: Linha id %d, x1 = %lf, y1 = %lf, x2 = %lf, y2 = %lf\n", idL, linX1, linY1, linX2, linY2);
    }
}

void dps(FILE *txt, FILE *svg, FILE *arq, char *infos[], char *eptr, Lista sR, Lista sC, Lista sL, Lista sT) {
    printf("--- INICIO DPS ---\n");
    int i;
    double dx, dy, aux1, aux2;
    double final_dx, final_dy;
    char corb[15], corp[15], aux3[100], aux4[1];

    fscanf(arq, "%s", infos);
    i = atoi(infos);

    fscanf(arq, "%s", infos);
    dx = strtod(infos, &eptr);

    fscanf(arq, "%s", infos);
    dy = strtod(infos, &eptr);

    fscanf(arq, "%s", infos);
    strcpy(corb, infos);

    fscanf(arq, "%s", infos);
    strcpy(corp, infos);

    fprintf(txt, "\n[*] dps\n");

    for (Cell auxC1 = getFirst(sR); auxC1 != NULL; auxC1 = getNext(sR, auxC1)) {
        Item auxI1 = getInfo(auxC1);

        final_dx = getRectX(auxI1) + dx;
        final_dy = getRectY(auxI1) + dy;
        aux1 = getRectHEIGHT(auxI1);
        aux2 = getRectWIDTH(auxI1);

        fprintf(svg, "\t<rect id=\"%d\" x=\"%lf\" y=\"%lf\" width=\"%lf\" height=\"%lf\" stroke=\"%s\" fill=\"%s\" fill-opacity=\"40%%\" />\n", i, final_dx, final_dy, aux2, aux1, corb, corp);
        fprintf(txt, "Criado retângulo: id = %d, x = %lf, y = %lf, w = %lf, h = %lf, stroke = %s, fill = %s\n", i, final_dx, final_dy, aux2, aux1, corb, corp);

        i++;
    }

    for (Cell auxC2 = getFirst(sC); auxC2 != NULL; auxC2 = getNext(sC, auxC2)) {
        Item auxI2 = getInfo(auxC2);

        final_dx = getCircX(auxI2) + dx;
        final_dy = getCircY(auxI2) + dy;
        aux1 = getCircRADIUS(auxI2);

        fprintf(svg, "\t<circle id=\"%d\" cx=\"%lf\" cy=\"%lf\" r=\"%lf\" stroke=\"%s\" fill=\"%s\" fill-opacity=\"40%%\" />\n", i, final_dx, final_dy, aux1, corb, corp);
        fprintf(txt, "Criado Círculo: id = %d, x = %lf, y = %lf, r = %lf, stroke = %s, fill = %s\n", i, final_dx, final_dy, aux1, corb, corp);

        i++;
    }

    for (Cell auxC3 = getFirst(sT); auxC3 != NULL; auxC3 = getNext(sT, auxC3)) {
        Item auxI3 = getInfo(auxC3);

        final_dx = getTxtX(auxI3) + dx;
        final_dy = getTxtY(auxI3) + dy;
        strcpy(aux3, getTxtTEXT(auxI3));
        strcpy(aux4, getTxtANCHOR(auxI3));

        fprintf(svg, "\t<text id=\"%d\" x=\"%lf\" y=\"%lf\" text-anchor=\"%s\" stroke=\"%s\" fill=\"%s\" fill-opacity=\"40%%\" >\"%s\"</text>\n", i, final_dx, final_dy, aux4, corb, corp, aux3);
        fprintf(txt, "Criado Texto: id = %d, x = %lf, y = %lf, stroke = %s, fill = %s\n", i, final_dx, final_dy, corb, corp);

        i++;
    }

    for (Cell auxC4 = getFirst(sL); auxC4 != NULL; auxC4 = getNext(sL, auxC4)) {
        Item auxI4 = getInfo(auxC4);

        final_dx = getLineX(auxI4) + dx;
        final_dy = getLineY(auxI4) + dy;
        aux1 = getLineFINALX(auxI4) + dx;
        aux2 = getLineFINALY(auxI4) + dy;

        fprintf(svg, "\t<line id=\"%d\" x1=\"%lf\" y1=\"%lf\" x2=\"%lf\" y2=\"%lf\" stroke=\"%s\" fill-opacity=\"50%%\" />\n", i, final_dx, final_dy, aux1, aux2, corb);
        fprintf(txt, "Criada Linha: id = %d, x1 = %lf, y1 = %lf, x2 = %lf, y2 = %lf, color = %s\n", i, final_dx, final_dy, aux1, aux2, corb);

        i++;
    }

    // printf("id %d\n", i);
    // printf("dx %lf\n", dx);
    // printf("dy %lf\n", dy);
    // printf("corb %s\n", corb);
    // printf("corp %s\n", corp);
}

void ups(FILE *txt, FILE *svg, FILE *arq, char *infos[], char *eptr, Lista sR, Lista sC, Lista sL, Lista sT) {
    printf("--- INICIO DPS ---\n");
    int n;
    double dx, dy;
    char corb[15], corp[15];

    fscanf(arq, "%s", infos);
    strcpy(corb, infos);

    fscanf(arq, "%s", infos);
    strcpy(corp, infos);

    fscanf(arq, "%s", infos);
    dx = strtod(infos, &eptr);

    fscanf(arq, "%s", infos);
    dy = strtod(infos, &eptr);

    fscanf(arq, "%s", infos);
    n = atoi(infos);

    fprintf(txt, "\n[*] ups\n");

    // INTEGRAR N NA FUNÇÃO, POIS NÃO DEVO ALTERAR TOAS AS FIGURAS NO BANCO DE DADOS
    //  APENAS ALGUMAS COM RELAÇÃO A N
    while (n != 0) {
        if (n > 0) {
            for (Cell auxC1 = getFirst(sR); auxC1 != NULL; auxC1 = getNext(sR, auxC1)) {
                Item auxI1 = getInfo(auxC1);

                setrectEDGE(auxI1, corb);
                setrectFILL(auxI1, corp);
                setrectX(auxI1, dx);
                setrectY(auxI1, dy);

                drawRectangle(svg, auxI1);
                n--;
                fprintf(txt, "Retângulo atualizado: dx = %lf, dy = %lf, stroke = %s, fill = %s\n", dx, dy, corb, corp);
            }

            for (Cell auxC2 = getFirst(sC); auxC2 != NULL; auxC2 = getNext(sC, auxC2)) {
                Item auxI2 = getInfo(auxC2);

                setcircEDGE(auxI2, corb);
                setcircFILL(auxI2, corp);
                setcircX(auxI2, dx);
                setcircY(auxI2, dy);

                drawCircle(svg, auxI2);
                n--;
                fprintf(txt, "Círculo atualizado: dx = %lf, dy = %lf, stroke = %s, fill = %s\n", dx, dy, corb, corp);
            }

            for (Cell auxC3 = getFirst(sT); auxC3 != NULL; auxC3 = getNext(sT, auxC3)) {
                Item auxI3 = getInfo(auxC3);

                settxtEDGE(auxI3, corb);
                settxtFILL(auxI3, corp);
                settxtX(auxI3, dx);
                settxtY(auxI3, dy);

                drawText(svg, auxI3);
                n--;
                fprintf(txt, "Texto atualizado: dx = %lf, dy = %lf, stroke = %s, fill = %s\n", dx, dy, corb, corp);
            }

            for (Cell auxC4 = getFirst(sL); auxC4 != NULL; auxC4 = getNext(sL, auxC4)) {
                Item auxI4 = getInfo(auxC4);

                setlineCOLOR(auxI4, corb);
                setlineX(auxI4, dx);
                setlineY(auxI4, dy);

                drawLine(svg, auxI4);
                n--;
                fprintf(txt, "Linha atualizada: dx = %lf, dy = %lf, color = %s\n", dx, dy, corb);
            }

        } else if (n < 0) {
            for (Cell auxC1 = getFirst(sR); auxC1 != NULL; auxC1 = getPrevious(sR, auxC1)) {
                Item auxI1 = getInfo(auxC1);

                setrectEDGE(auxI1, corb);
                setrectFILL(auxI1, corp);
                setrectX(auxI1, dx);
                setrectY(auxI1, dy);

                drawRectangle(svg, auxI1);
                n++;
                fprintf(txt, "Retângulo atualizado: dx = %lf, dy = %lf, stroke = %s, fill = %s\n", dx, dy, corb, corp);
            }

            for (Cell auxC2 = getFirst(sC); auxC2 != NULL; auxC2 = getPrevious(sC, auxC2)) {
                Item auxI2 = getInfo(auxC2);

                setcircEDGE(auxI2, corb);
                setcircFILL(auxI2, corp);
                setcircX(auxI2, dx);
                setcircY(auxI2, dy);

                drawCircle(svg, auxI2);
                n++;
                fprintf(txt, "Círculo atualizado: dx = %lf, dy = %lf, stroke = %s, fill = %s\n", dx, dy, corb, corp);
            }

            for (Cell auxC3 = getFirst(sT); auxC3 != NULL; auxC3 = getPrevious(sT, auxC3)) {
                Item auxI3 = getInfo(auxC3);

                settxtEDGE(auxI3, corb);
                settxtFILL(auxI3, corp);
                settxtX(auxI3, dx);
                settxtY(auxI3, dy);

                drawText(svg, auxI3);
                n++;
                fprintf(txt, "Texto atualizado: dx = %lf, dy = %lf, stroke = %s, fill = %s\n", dx, dy, corb, corp);
            }

            for (Cell auxC4 = getFirst(sL); auxC4 != NULL; auxC4 = getPrevious(sL, auxC4)) {
                Item auxI4 = getInfo(auxC4);

                setlineCOLOR(auxI4, corb);
                setlineX(auxI4, dx);
                setlineY(auxI4, dy);

                drawLine(svg, auxI4);
                n++;
                fprintf(txt, "Linha atualizada: dx = %lf, dy = %lf, color = %s\n", dx, dy, corb);
            }
        }
    }

    // printf("corb %s\n", corb);
    // printf("corp %s\n", corp);
    // printf("dx %lf\n", dx);
    // printf("dy %lf\n", dy);
    // printf("n %d\n", n);
}
