#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<malloc.h>
#include<string.h>

typedef struct curs curs;
typedef struct nod nod;
typedef struct LD LD;

struct curs {
    unsigned int codCurs;
    char* titluCurs;
    unsigned char credite;
    float promovabilitate;
};
curs initializareCurs(unsigned int codCurs, const  char* titluCurs, unsigned char credite, float promovabilitate) {
    curs c;
    c.codCurs = codCurs;
    c.titluCurs = malloc(sizeof(char) * strlen(titluCurs) + 1);
    strcpy_s(c.titluCurs, sizeof(char) * strlen(titluCurs) + 1, titluCurs);
    c.credite = credite;
    c.promovabilitate = promovabilitate;
    return c;
}
void afisareCurs(curs c) {
    printf("COD:%d, Titlu curs:%s, Credite:%d, Promovabilitate: %.2f\n", c.codCurs, c.titluCurs, c.credite, c.promovabilitate);
}
struct nod {
    nod* prev;
    curs info;
    nod* next;
};
struct LD {
    nod* prim;
    nod* ultim;
};

LD inserareLDInceput(LD lista, curs c) {
    nod* nou = (nod*)malloc(sizeof(nod) );
    nou->prev = NULL;
    nou->info = initializareCurs(c.codCurs, c.titluCurs, c.credite, c.promovabilitate);
    nou->next = NULL;
    if (lista.prim == NULL) {
        lista.prim = lista.ultim = nou;
    }
    else {
        nou->next = lista.prim;
        lista.prim->prev = nou;
        lista.prim = nou;
    }
    return lista;
}
void traversareListaInceput(LD lista) {
    nod* aux = lista.prim;
    while (aux) {
        afisareCurs(aux->info);
        aux = aux->next;
    }
}

void dezalocare(LD lista) {
    nod* aux = lista.prim;
    while (aux) {
        free(aux->info.titluCurs);
        nod* temp = aux->next;
        free(aux);
        aux = temp;
    }
}

void main() {
    curs c;
    LD lista;
    lista.prim = NULL;
    lista.ultim = NULL;

    char linieBuffer[100];
    char* aux;
    char sep[] = ",";

    FILE* f = fopen("p5.txt", "r");
    if (f) {
        while (fgets(linieBuffer, 100, f)) {
            aux = strtok(linieBuffer, sep);
            c.codCurs = atoi(aux);

            aux = strtok(NULL, sep);
            c.titluCurs = malloc(sizeof(char) * strlen(aux) + 1);
            strcpy_s(c.titluCurs, sizeof(char) * strlen(aux) + 1, aux);

            aux = strtok(NULL, sep);
            c.credite = atoi(aux);

            aux = strtok(NULL, sep);
            c.promovabilitate = atof(aux);

            lista = inserareLDInceput(lista, c);
            free(c.titluCurs);
        }
    }
    fclose(f);
    traversareListaInceput(lista);
    dezalocare(lista);
}