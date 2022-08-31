#include "srb_tree.h"
#include "list.h"
#include "shapes.h"

struct node {
    Info value;
    double x, y;
    double mbbX1, mbbX2, mbbY1, mbbY2;
    struct node *parent;
    struct node *left;
    struct node *right;
    char color[10];
};
typedef struct node Red_Black_Node;

struct tree {
    Red_Black_Node *root;
    Red_Black_Node *nil;
    double epislon;
    int size;
};
typedef struct tree Red_Black_Root;

// ---> HEADERS DE FUNÇÕES EXTRAS

Node newNode(Info i, double x, double y, double mbbX1, double mbbY1, double mbbX2, double mbbY2);
Node insertAux(SRBTree t, Node n, double x, double y, double mbbX1, double mbbY1, double mbbX2, double mbbY2, Info info);
void fixRBinsert(SRBTree t, Node n);
void rotateLeft(SRBTree t, Node n);
void rotateRight(SRBTree t, Node n);
bool isBlack(Node n);
void paintBlack(Node n);
bool isRed(Node n);
void paintRed(Node n);
void rectangleOverlaps(Node n, double x, double y, double w, double h, Lista resultado);
void mbbFullyInside(Node n, double x, double y, double w, double h, Lista resultado);
Node searchNode(SRBTree t, Node n, double xa, double ya, double *mbbX1, double *mbbY1, double *mbbX2, double *mbbY2);
void transplantRB(SRBTree t, Node n, Node n2);
void fixRBdelete(SRBTree t, Node n); 
Node getLargestLeft(Node n);
Node getSmallestRight(Node n);
void traverseAux(Node root, FvisitaNo f, void *aux);
void levelOrderAux(Node root, int level);
int heightOfLevel(Node n);void mbbFullyInside(Node n, double x, double y, double w, double h, Lista resultado);
void traverseSim(Node root, FvisitaNo f, void *aux);
void removeNils(SRBTree t, Node root);
void freeAux(Node n);

// FIM <---

SRBTree createSRB(double epsilon) {
    Red_Black_Root *new_tree = calloc(1, sizeof(Red_Black_Root));

    new_tree->nil = calloc(1, sizeof(Red_Black_Node));
    paintBlack(new_tree->nil);
    new_tree->nil->value = NULL;

    new_tree->epislon = epsilon;
    new_tree->root = NULL;
    new_tree->size = 0;

    return new_tree;
}

Node newNode(Info i, double x, double y, double mbbX1, double mbbY1, double mbbX2, double mbbY2) {
    Red_Black_Node *new_node = calloc(1, sizeof(Red_Black_Node));
    new_node->value = i;
    new_node->x = x;
    new_node->y = y;
    new_node->mbbX1 = mbbX1;
    new_node->mbbX2 = mbbX2;
    new_node->mbbY1 = mbbY1;
    new_node->mbbY2 = mbbY2;
    new_node->parent = NULL;
    new_node->left = NULL;
    new_node->right = NULL;
    strcpy(new_node->color, "RED");

    return new_node;
}

Node insertSRB(SRBTree t, double x, double y, double mbbX1, double mbbY1, double mbbX2, double mbbY2, Info info) {
    Red_Black_Root *red_black_tree = t;
    insertAux(t, red_black_tree->root, x, y, mbbX1, mbbY1, mbbX2, mbbY2, info);
    Red_Black_Node *new_node = getNodeSRB(t, x, y, &mbbX1, &mbbY1, &mbbX2, &mbbY2);
    fixRBinsert(t, new_node);
    return new_node;
}

Node insertAux(SRBTree t, Node n, double x, double y, double mbbX1, double mbbY1, double mbbX2, double mbbY2, Info info) {
    Red_Black_Root *red_black_tree = t;
    Red_Black_Node *new_node = n;

    if (!new_node) {
        new_node = newNode(info, x, y, mbbX1, mbbY1, mbbX2, mbbY2);
        if (!red_black_tree->root) {
            red_black_tree->root = new_node;
            paintBlack(new_node);
        }
        if (new_node) {
            red_black_tree->size++;
            return new_node;
        } else {
            printf("ERROR: YOU ARE OUT OF MEMORY\n");
            return NULL;
        }
    }

    if ((x < new_node->x) || (x == new_node->x && y < new_node->y)) {
        new_node->left = insertAux(red_black_tree, new_node->left, x, y, mbbX1, mbbY1, mbbX2, mbbY2, info);
        new_node->left->parent = new_node;

    } else {
        new_node->right = insertAux(red_black_tree, new_node->right, x, y, mbbX1, mbbY1, mbbX2, mbbY2, info);
        new_node->right->parent = new_node;
    }
    return new_node;
}

// TODO
// Node insertBBSRB(SRBTree t, double mbbX1, double mbbY1, double mbbX2, double mbbY2, Info info) {}

void fixRBinsert(SRBTree t, Node n) {
    Red_Black_Root *rb_tree = t;
    Red_Black_Node *root = rb_tree->root;
    Red_Black_Node *z = n;
    Red_Black_Node *uncle_z = NULL;

    while ((z != root) && (isRed(z)) && (isRed(z->parent))) {
        // CASO A: pai de z é filho a esquerda do avô de z
        if (z->parent == z->parent->parent->left) {
            if (z->parent->parent->right) {
                uncle_z = z->parent->parent->right;
            }

            // CASO 1A: o tio de z é vermelho, precisa recolorir
            if (uncle_z && isRed(uncle_z)) {
                paintRed(z->parent->parent);
                paintBlack(z->parent);
                paintBlack(uncle_z);
                z = z->parent->parent;

            } else {
                // CASO 2A: tio preto, z é filho a direita de seu pai, precisa rotacionar para a esquerda
                if (z == z->parent->right) {
                    z = z->parent;
                    rotateLeft(t, z);
                }

                // CASO 3A: tio preto, z é filho a esquerda de seu pai, precisa rotacionar para a direita
                paintBlack(z->parent);
                paintRed(z->parent->parent);
                rotateRight(t, z->parent->parent);
            }

            // CASO B: o pai de z é o filho direito do avô de z
        } else if (z->parent && z->parent == z->parent->parent->right) {
            if (z->parent->parent->left) {
                uncle_z = z->parent->parent->left;
            }

            // CASO 1B: o tio de z é vermelho, precisa recolorir
            if (uncle_z && isRed(uncle_z)) {
                paintRed(z->parent->parent);
                paintBlack(z->parent);
                paintBlack(uncle_z);
                z = z->parent->parent;

            } else {
                // CASO 2B: tio preto, z é filho a esquerda de seu pai, precisa rotacionar para a direita
                if (z == z->parent->left) {
                    z = z->parent;
                    rotateRight(t, z);
                }

                // CASO 3B: z é filho a direita de seu pai, precisa rotacionar para a esquerda
                paintBlack(z->parent);
                paintRed(z->parent->parent);
                rotateLeft(t, z->parent->parent);
            }
        }
    }
    paintBlack(root);
}

void rotateLeft(SRBTree t, Node n) {
    Red_Black_Root *red_black_tree = t;
    Red_Black_Node *node = n;                   // avô 2
    Red_Black_Node *right_child = node->right;  // pai (direita) 7

    node->right = right_child->left;
    if (node->right) {
        node->right->parent = node;
    }
    right_child->parent = node->parent;

    if (!node->parent) {
        red_black_tree->root = right_child;

    } else if ((node == node->parent->left)) {
        node->parent->left = right_child;

    } else {
        node->parent->right = right_child;
    }

    right_child->left = node;
    node->parent = right_child;
}

void rotateRight(SRBTree t, Node n) {
    Red_Black_Root *red_black_tree = t;
    Red_Black_Node *node = n;                 // recebo o AVÔ
    Red_Black_Node *left_child = node->left;  // pai

    node->left = left_child->right;  // esqueda do avô aponta para direita do pai
    if (node->left) {
        node->left->parent = node;
    }
    left_child->parent = node->parent;

    if (!node->parent) {
        red_black_tree->root = left_child;

    } else if ((node == node->parent->left)) {
        node->parent->left = left_child;

    } else {
        node->parent->right = left_child;
    }

    left_child->right = node;
    node->parent = left_child;
}

bool isBlack(Node n) {
    Red_Black_Node *node = n;
    if (!strcmp(node->color, "BLACK")) {
        return true;
    }
    return false;
}

void paintBlack(Node n) {
    Red_Black_Node *node = n;
    strcpy(node->color, "BLACK");
}

bool isRed(Node n) {
    Red_Black_Node *node = n;
    if (!strcmp(node->color, "RED")) {
        return true;
    }
    return false;
}

void paintRed(Node n) {
    Red_Black_Node *node = n;
    strcpy(node->color, "RED");
}

void getBBPartSRB(SRBTree t, double x, double y, double w, double h, Lista resultado) { // MBB do node e retangulo tem alguma intersecção
    Red_Black_Root *tree = t;
    rectangleOverlaps(tree->root, x, y, w, h, resultado);
}  

void rectangleOverlaps(Node n, double x, double y, double w, double h, Lista resultado) {
    Red_Black_Node *node = n;
    double l1x = node->mbbX1;
    double l1y = node->mbbY1;
    double r1x = node->mbbX2; 
    double r1y = node->mbbY2;
    double l2x = x;
    double l2y = y;
    double r2x = x + w;
    double r2y = y + h;
    int i = 0;

    if (!node) {
        return;
    }

    if (l1x == r1x || l1y == r1y || r2x == l2x || l2y == r2y) {
        i++;
    }
    if (l1x > r2x || l2x > r1x) {
        i++;
    }
    if (r1y > l2y || r2y > l1y) {
        i++;
    }
    if (i == 0) {
        insereFim(resultado, node);
    }

    rectangleOverlaps(node->left, x, y, w, h, resultado);
    rectangleOverlaps(node->right, x, y, w, h, resultado);
}

void getBBSRB(SRBTree t, double x, double y, double w, double h, Lista resultado) { // MBB completamente dentro do retangulo
    Red_Black_Root *red_black_tree = t;
    mbbFullyInside(red_black_tree->root, x, y, w, h, resultado);
} 

void mbbFullyInside(Node n, double x, double y, double w, double h, Lista resultado) {
    Red_Black_Node *node = n;
    if (!node) {
        return;
    }
    double x1 = node->mbbX1;
    double y1 = node->mbbY1;
    double w1 = node->mbbX2 - x1;
    double h1 = node->mbbY2 - y1;
    if (((x + w) >= (x1 + w1))) {
        if (((y + h) >= (y1 + h1))) {
            if (x <= x1 && y <= y1) {
                insereFim(resultado, n);
            }
        }
    }
    mbbFullyInside(node->left, x, y, w, h, resultado);
    mbbFullyInside(node->right, x, y, w, h, resultado);
}

Info getInfoSRB(SRBTree t, Node n, double xa, double ya, double *mbbX1, double *mbbY1, double *mbbX2, double *mbbY2) {
    Red_Black_Node *node = n;
    *mbbX1 = node->mbbX1;
    *mbbY1 = node->mbbY1;
    *mbbX2 = node->mbbX2;
    *mbbY2 = node->mbbY2;
    return node->value;
}

Node getNodeSRB(SRBTree t, double xa, double ya, double *mbbX1, double *mbbY1, double *mbbX2, double *mbbY2) {
    Red_Black_Root *tree = t;
    Red_Black_Node *node = searchNode(t, tree->root, xa, ya, mbbX1, mbbY1, mbbX2, mbbY2);
    return node;
}

Node searchNode(SRBTree t, Node n, double xa, double ya, double *mbbX1, double *mbbY1, double *mbbX2, double *mbbY2) {
    Red_Black_Root *red_black_tree = t;
    Red_Black_Node *new_node = n;
    if (!new_node) {
        return NULL;
    }

    if (fabs(new_node->x - xa) <= red_black_tree->epislon && fabs(new_node->y - ya) <= red_black_tree->epislon) {
        *mbbX1 = new_node->mbbX1;
        *mbbY1 = new_node->mbbY1;
        *mbbX2 = new_node->mbbX2;
        *mbbY2 = new_node->mbbY2;
        return new_node;
    }

    if ((xa < new_node->x) || (xa == new_node->x && ya < new_node->y)) {
        return searchNode(t, new_node->left, xa, ya, mbbX1, mbbY1, mbbX2, mbbY2);
    
    } else {
        return searchNode(t, new_node->right, xa, ya, mbbX1, mbbY1, mbbX2, mbbY2);
    } 
}

void updateInfoSRB(SRBTree t, Node n, Info i) {
    Red_Black_Root *red_black_tree = t;
    Red_Black_Node *node = n;
    double x = getX(i);
    double y = getY(i);
    if (fabs(node->x - x) <= red_black_tree->epislon && fabs(node->y - y) <= red_black_tree->epislon) {
        node->value = i;
        printf("Sucesso ao atualizar a info\n");
    } else {
        printf("Não foi possivel atualizar a info\n");
    }
}

// Cases and algorithm of deletion: https://www.geeksforgeeks.org/red-black-tree-set-3-delete-2/
void removeSRB(SRBTree t, double xa, double ya, double *mbbX1, double *mbbY1, double *mbbX2, double *mbbY2) {
    Red_Black_Root *tree = t;
    Red_Black_Node *rb_node = getNodeSRB(tree, xa, ya, mbbX1, mbbY1, mbbX2, mbbY2);
    Red_Black_Node *nil = tree->nil;
    if (!rb_node) {
        printf("NO NODE FOUND\n");
        return;
    }

    if (fabs(rb_node->x - xa) <= tree->epislon && fabs(rb_node->y - ya) <= tree->epislon) {
        Red_Black_Node *y = rb_node;
        Red_Black_Node *x = NULL;
        char y_original_color[10];
        strcpy(y_original_color, y->color);

        if (!rb_node->left && !rb_node->right && isRed(rb_node)) {
            if (rb_node == rb_node->parent->left) {
                rb_node->parent->left = NULL;
            } else {
                rb_node->parent->right = NULL;
            }
            free(rb_node);
            return;

        } else if (!rb_node->left && !rb_node->right && isBlack(rb_node)) {
            Red_Black_Node *aux_p = rb_node->parent;
            int i = -1;
            if (rb_node == rb_node->parent->left) {
                i = 0;
            } else {
                i = 1;
            }

            fixRBdelete(tree, rb_node);
            if (i == 0) {
                aux_p->left = nil;
            } else if (i == 1) {
                aux_p->right = nil;
            }
            removeNils(tree, tree->root);
            free(rb_node);
            return;
        }

        if (!rb_node->left && rb_node->right) {
            x = rb_node->right;
            transplantRB(tree, rb_node, rb_node->right);

        } else if (!rb_node->right && rb_node->left) {
            x = rb_node->left;
            transplantRB(tree, rb_node, rb_node->left);

        } else if (rb_node->left && rb_node->right) {
            y = getLargestLeft(rb_node->left);
            strcpy(y_original_color, y->color);
            if (!y->left) {
                x = nil;
                nil->parent = y;
            } else {
                x = y->left;
            }
            if (y->parent == rb_node) {
                x->parent = y;
            } else {
                transplantRB(tree, y, y->left);
                y->left = rb_node->left;
                y->left->parent = y;
            }
            if (!y->right) {
                y->right = nil;
                nil->parent = y;
            }
            if (!y->left) {
                y->left = nil;
                nil->parent = y;
            }
            transplantRB(tree, rb_node, y);
            y->right = rb_node->right;
            y->right->parent = y;
            strcpy(y->color, rb_node->color);
        }

        if (!strcmp(y_original_color, "BLACK")) {
            fixRBdelete(tree, x);
        }
    }
    if (rb_node) {
        free(rb_node);
    }
    removeNils(tree, tree->root);
}

void transplantRB(SRBTree t, Node n, Node n2) {
    Red_Black_Root *red_black_tree = t;
    Red_Black_Node *u = n;
    Red_Black_Node *v = n2;

    if (!u->parent) {  // u é raiz
        red_black_tree->root = v;

    } else if (u == u->parent->left) {  // u é filho da esquerda
        u->parent->left = v;
    } else {  // u é filho da direita
        u->parent->right = v;
    }
    if (v) {
        v->parent = u->parent;
    }
}

void fixRBdelete(SRBTree t, Node n) {
    Red_Black_Root *rb_tree = t;
    Red_Black_Node *rb_node = n;
    Red_Black_Node *root = rb_tree->root;
    Red_Black_Node *nil = rb_tree->nil;
    Red_Black_Node *w = NULL;

    while (root != rb_node && isBlack(rb_node)) {
        if (rb_node && rb_node == rb_node->parent->left) {
            w = rb_node->parent->right;
            if (!w->left) {
                w->left = nil;
            }
            if (!w->right) {
                w->right = nil;
            }

            // Caso 1A: irmão w é vermelho
            if (w && isRed(w)) {
                paintRed(rb_node->parent);
                paintBlack(w);
                rotateLeft(t, rb_node->parent);
                w = rb_node->parent->right;
            }

            // Caso 2A: Ambos os filhos do irmão w são pretos
            if (isBlack(w) && (isBlack(w->left) && isBlack(w->right))) {
                paintRed(w);
                rb_node = rb_node->parent;

                // Caso 3A: Filho direito do irmão é vermelho Caso Right right
            } else if ((w->right && isRed(w->right))) {
                rotateLeft(t, w->parent);
                paintRed(w);
                paintBlack(w->right);
                paintBlack(w->left);
                rb_node = w;
            }

        } else if (rb_node && rb_node == rb_node->parent->right) {
            w = rb_node->parent->left;
            if (!w->left) {
                w->left = nil;
            }
            if (!w->right) {
                w->right = nil;
            }

            // Caso 1B: irmão w é vermelho
            if (w && isRed(w)) {
                paintRed(rb_node->parent);
                paintBlack(w);
                rotateRight(t, rb_node->parent);
                w = rb_node->parent->left;
            }
            // Caso 2B: Ambos os filhos do irmão w são pretos
            if ((isBlack(w->left) && isBlack(w->right))) {
                paintRed(w);
                rb_node = rb_node->parent;

                // Caso 3B: Filho esquerdo do irmão é vermelho caso Left left
            } else if ((w->left && isRed(w->left))) {
                paintBlack(w->left);
                rotateRight(t, w->parent);
                paintRed(w);
                paintBlack(w->left);
                paintBlack(w->right);
                rb_node = w;
            }
        }
    }
}

Node getLargestLeft(Node n) {
    Red_Black_Node *node = n;
    while (node->right) {
        node = node->right;
    }
    return node;
}

Node getSmallestRight(Node n) {
    Red_Black_Node *node = n;
    while (node->left) {
        node = node->left;
    }
    return node;
}

// TODO
// void printSRB(SRBTree t, char *nomeArq) {}

void percursoProfundidade(SRBTree t, FvisitaNo f, void *aux) {
    Red_Black_Root *br_tree = t;
    traverseAux(br_tree->root, f, aux);
}

void traverseAux(Node root, FvisitaNo f, void *aux) {
    Red_Black_Node *node = root;
    if (!node) {
        return;
    }

    f(node->value, node->x, node->y, node->mbbX1, node->mbbY1, node->mbbX2, node->mbbY2, aux);
    traverseAux(node->left, f, aux);
    traverseAux(node->right, f, aux);
}

void percursoLargura(SRBTree t, FvisitaNo f, void *aux) {
    Red_Black_Root *rb_tree = t;
    int h = heightOfLevel(rb_tree->root);
    printf("\n\tTamanho da árvore: %d\n\n", rb_tree->size);
    for (int i = 0; i <= h; i++) {
        levelOrderAux(rb_tree->root, i);
    }
}

void levelOrderAux(Node root, int level) {
    Red_Black_Node *node = root;
    if (!node) {
        return;
    }
    if (level == 1) {
        if (node->parent) {
            printf("\n\n\n\t%.2lf %.2lf %s\n", node->parent->x, node->parent->y, node->parent->color);
            printf("\t      |\n");
        }

        printf("\t%.2lf %.2lf %s\n", node->x, node->y, node->color);
        printf("\t /");
        printf("\t    \\\n");

        if (node->left) {
            printf("%.2lf %.2lf %s", node->left->x, node->left->y, node->left->color);
        }
        if (node->right) {
            printf("  %.2lf %.2lf %s", node->right->x, node->right->y, node->right->color);
        }

    } else if (level > 1) {
        levelOrderAux(node->left, level - 1);
        levelOrderAux(node->right, level - 1);
    }
}

int heightOfLevel(Node n) {
    Red_Black_Node *node = n;
    if (!node) {
        return 0;
    }
    int left_height = heightOfLevel(node->left);
    int right_height = heightOfLevel(node->right);
    if (left_height > right_height) {
        return (left_height + 1);
    } else {
        return (right_height + 1);
    }
}
void percursoSimetrico(SRBTree t, FvisitaNo f, void *aux) {
    Red_Black_Root *br_tree = t;
    traverseSim(br_tree->root, f, aux);
}

void traverseSim(Node root, FvisitaNo f, void *aux) {
    Red_Black_Node *node = root;
    if (!node) {
        return;
    }

    traverseSim(node->left, f, aux);
    f(node->value, node->x, node->y, node->mbbX1, node->mbbY1, node->mbbX2, node->mbbY2, aux);
    traverseSim(node->right, f, aux);
}

void removeNils(SRBTree t, Node root) {
    Red_Black_Root *tree = t;
    Red_Black_Node *node = root;
    Red_Black_Node *nil = tree->nil;
    if (!node) {
        return;
    }
    if (node->left == nil) {
        node->left = NULL;
    }
    if (node->right == nil) {
        node->right = NULL;
    }
    removeNils(t, node->left);
    removeNils(t, node->right);
}

void killSRB(SRBTree t) {
    Red_Black_Root *red_black_tree = t;
    free(red_black_tree->nil);
    freeAux(red_black_tree->root);
    free(red_black_tree);
}

void freeAux(Node root) {
    Red_Black_Node *node = root;
    if (!node) {
        return;
    }

    freeAux(node->left);
    freeAux(node->right);
    if (node) {
        free(node);
    }
}