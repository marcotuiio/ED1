#include "list.h"

typedef struct StListEdge {
    InfoEdge valueEdge;
    struct StListEdge *next;
    struct StListEdge *prev;
    Node from, to;
    int td, tf, d;  // tempo de descoberta, tempo de finalização e distância
    bool enabled;
} StListEdge;

typedef struct StListVertex {
    InfoNode valueNode;
    char visitedType;  // 'w' = white, 'g' = gray, 'b' = black
    char id[40];
    StListEdge *inicio;
    StListEdge *fim;
} StListVertex;

Lista criaLista() {
    StListVertex *novaLista = calloc(1, sizeof(StListVertex));
    novaLista->inicio = NULL;
    novaLista->fim = NULL;
    return novaLista;
}

void setId(Lista l, char *id) {
    StListVertex *lista = l;
    strcpy(lista->id, id);
}

char *getId(Lista l) {
    StListVertex *lista = l;
    return lista->id;
}

void setTD(Edge e, int td) {
    StListEdge *edge = e;
    edge->td = td;
}

int getTD(Edge e) {
    StListEdge *edge = e;
    return edge->td;
}

void setTF(Edge e, int tf) {
    StListEdge *edge = e;
    edge->tf = tf;
}

int getTF(Edge e) {
    StListEdge *edge = e;
    return edge->tf;
}

void setD(Edge e, int d) {
    StListEdge *edge = e;
    edge->d = d;
}

int getD(Edge e) {
    StListEdge *edge = e;
    return edge->d;
}

void printList(Lista l) {
    StListVertex *aux = l;
    StListEdge *temporary = aux->inicio;

    while (temporary) {
        printf("%p ", temporary->valueEdge);
        temporary = temporary->next;
    }
    free(temporary);
}

Edge encontraAresta(Lista l, Node from, Node to) {
    StListVertex *aux = l;
    StListEdge *edge = aux->inicio;
    StListEdge *result = NULL;

    while (edge) {
        if (edge->to == to && edge->from == from) {
            result = edge;
            return result;
        }
        edge = edge->next;
    }
    if (!result) {
        printf("VALOR NAO ENCONTRADO");
        return NULL;
    }
    return NULL;
}

Edge insereFim(Lista l, InfoEdge n, Node from, Node to) {
    StListVertex *lista = l;

    // Cria celula
    StListEdge *novaCelula = calloc(1, sizeof(StListEdge));
    novaCelula->valueEdge = n;  // n pode ser int, char etc, recebido com void pointer
    novaCelula->next = NULL;
    novaCelula->prev = NULL;
    novaCelula->enabled = true;
    novaCelula->from = from;
    novaCelula->to = to;

    if (lista->fim) {  // fim <-> nova --
        novaCelula->prev = lista->fim;
        novaCelula->prev->next = novaCelula;
        lista->fim = novaCelula;
    } else {  // lista vazia
        lista->fim = novaCelula;
        lista->inicio = novaCelula;
    }
    return novaCelula;
}

Edge insereInicio(Lista l, void *n) {
    StListVertex *lista = l;

    // Cria celula
    StListEdge *novaCelula = calloc(1, sizeof(StListEdge));
    novaCelula->valueEdge = n;  // n pode ser int, char etc, recebido com void pointer
    novaCelula->next = NULL;
    novaCelula->prev = NULL;

    if (lista->inicio) {
        novaCelula->next = lista->inicio;  // nova <-> primeira
        novaCelula->next->prev = novaCelula;
        lista->inicio = novaCelula;
    } else {  //(Lista vazia)
        lista->fim = novaCelula;
        lista->inicio = novaCelula;  //(inicio)nova(fim)
    }
    return novaCelula;
}

Edge insereDepois(Lista l, void *n, void *x) {
    StListVertex *aux = l;
    StListEdge *lista = aux->inicio;
    StListEdge *celulaAnterior;

    // Buscando a celula com valor n desejado
    while (lista) {
        if (lista->valueEdge == n) {
            celulaAnterior = lista;
            break;
        }
        lista = lista->next;
    }

    // Criando celula com valor x desejado
    StListEdge *novaCelula = calloc(1, sizeof(StListEdge));
    novaCelula->valueEdge = x;
    novaCelula->next = NULL;
    novaCelula->prev = NULL;

    novaCelula->next = celulaAnterior->next;  // ant -> nova -> aux

    if (novaCelula->next) {  // ant -> new <- next
        novaCelula->next->prev = novaCelula;
    } else if (celulaAnterior == aux->fim) {
        aux->fim = novaCelula;  // ant <-> new -> fim
    }
    novaCelula->prev = celulaAnterior;
    celulaAnterior->next = novaCelula;
    return novaCelula;
}

void removeAresta(Lista l, void *n) {
    StListVertex *aux = l;
    StListEdge *lista = aux->inicio;
    StListEdge *celulaARemover = NULL;

    // Buscando a celula com valor desejado

    while (lista) {
        if (lista->next == n) {  // se n inteiro ou char
            celulaARemover = lista;
        }
        lista = lista->next;
    }

    if (!celulaARemover) {
        // printf("ELEMENTO INEXISTENTE NA LISTA\n");
        return;
    }

    if (aux->inicio == celulaARemover && aux->fim == celulaARemover) {
        aux->inicio = NULL;
        aux->fim = NULL;

    } else if (aux->inicio == celulaARemover) {  // celula a remover -> slkkefd
        aux->inicio = celulaARemover->next;
        celulaARemover->next->prev = NULL;

    } else if (aux->fim == celulaARemover) {  // vfbvff -> celula a remover
        aux->fim = celulaARemover->prev;
        celulaARemover->prev->next = NULL;

    } else {  // ant -> remove -> next
        celulaARemover->prev->next = celulaARemover->next;
        if (celulaARemover->next) {
            celulaARemover->next->prev = celulaARemover->prev;
        }
    }

    free(celulaARemover);
}

void setVisited(Lista l, char b) {
    StListVertex *lista = l;
    lista->visitedType = b;
}

char getVisited(Lista l) {
    StListVertex *lista = l;
    return lista->visitedType;
}

void setEnabled(Edge e, bool b) {
    StListEdge *aux = e;
    aux->enabled = b;
}

bool getEnabled(Edge e) {
    StListEdge *aux = e;
    return aux->enabled;
}

Node getFromAresta(Edge e) {
    StListEdge *aux = e;
    return aux->from;
}

Node getToAresta(Edge e) {
    StListEdge *aux = e;
    return aux->to;
}

Edge getFirst(Lista l) {
    StListVertex *lista = l;
    return lista->inicio;
}

Edge getLast(Lista l) {
    StListVertex *lista = l;
    return lista->fim;
}

Edge getNext(Edge at) {
    StListEdge *node = at;
    return node->next;
}

Edge getPrevious(Edge at) {
    StListEdge *node = at;
    return node->prev;
}

void setInfoFromVertex(Lista l, InfoNode info) {
    StListVertex *list = l;
    list->valueNode = info;
}

InfoNode getInfoFromVertex(Lista l) {
    StListVertex *list = l;
    return list->valueNode;
}

void setInfoFromEdge(Lista l, InfoEdge info) {
    StListEdge *list = l;
    list->valueEdge = info;
}

InfoEdge getInfoFromEdge(Edge e) {
    StListEdge *edge = e;
    return edge->valueEdge;
}

void freeEdgeInfo(Edge e) {
    StListEdge *edge = e;
    free(edge->valueEdge);
}

void getLenght(Lista l) {
    StListVertex *lista = l;
    StListEdge *aux = lista->inicio;
    int contador = 0;

    while (aux) {
        contador++;
        aux = aux->next;
    }
    printf("\nO tamanho da lista é de %d elementos\n", contador);
}

void freeList(Lista l) {
    StListVertex *lista = l;
    if (!lista->inicio) {
        return;
    }

    StListEdge *head = lista->inicio;
    StListEdge *tmp;

    while (head != NULL) {
        tmp = head;
        head = tmp->next;

        free(tmp->valueEdge);
        free(tmp);
    }
}
