#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>

typedef struct scoala {
    unsigned int id;
    char* nume;
    int nrClase;
    int nrTotalElevi;
} scoala;

typedef struct nod {
    scoala info;
    struct nod* next;
} nod;

typedef struct HashTable {
    int dim;
    nod** vector;
} HashTable;

scoala initializareScoala(unsigned int id, const char* nume, int nrClase, int nrTotalElevi) {
    scoala s;
    s.id = id;
    s.nume = (char*)malloc(strlen(nume) + 1);
    if (s.nume)
        strcpy(s.nume, nume);
    s.nrClase = nrClase;
    s.nrTotalElevi = nrTotalElevi;
    return s;
}


void afisareScoala(scoala s) {
    printf("ID: %d, Nume: %s, Nr clase: %d, Nr total elevi: %d\n",
        s.id, s.nume, s.nrClase, s.nrTotalElevi);
}

int functieHash(HashTable tabela, const char* nume) {
    return strlen(nume) % tabela.dim;
}

int inserareHash(HashTable tabela, scoala s) {
    int pozitie = 0;
    if (tabela.vector) {
        pozitie = functieHash(tabela, s.nume);
        nod* nou = (nod*)malloc(sizeof(nod));
        nou->info = initializareScoala(s.id, s.nume, s.nrClase, s.nrTotalElevi);
        nou->next = NULL;
        if (tabela.vector[pozitie] == NULL) {
            tabela.vector[pozitie] = nou;
        }
        else {
            nod* aux = tabela.vector[pozitie];
            while (aux->next) {
                aux = aux->next;
            }
            aux->next = nou;
        }
    }
    return pozitie;
}

void traversareHash(HashTable tabela) {
    if (tabela.vector) {
        for (int i = 0; i < tabela.dim; i++) {
            if (tabela.vector[i]) {
                printf("Pozitie %d:\n", i);
                nod* aux = tabela.vector[i];
                while (aux) {
                    afisareScoala(aux->info);
                    aux = aux->next;
                }
            }
        }
    }
}


void dezalocare(HashTable tabela) {
    if (tabela.vector) {
        for (int i = 0; i < tabela.dim; i++) {
            nod* aux = tabela.vector[i];
            while (aux) {
                free(aux->info.nume);
                nod* temp = aux;
                aux = aux->next;
                free(temp);
            }
        }
        free(tabela.vector);
    }
}


int main() {
    scoala s;
    HashTable tabela;
    tabela.dim = 20;
    tabela.vector = (nod**)malloc(sizeof(nod*) * tabela.dim);
    for (int i = 0; i < tabela.dim; i++) {
        tabela.vector[i] = NULL;
    }

    FILE* f = fopen("p6.txt", "r");
    if (!f) {
        printf("Eroare la deschiderea fi?ierului!\n");
        return -1;
    }

    char linieBuffer[200];
    char* aux;
    const char sep[] = ",";

    while (fgets(linieBuffer, sizeof(linieBuffer), f)) {
        aux = strtok(linieBuffer, sep);
        s.id = atoi(aux);

        aux = strtok(NULL, sep);
        s.nume = aux;  

        aux = strtok(NULL, sep);
        s.nrClase = atoi(aux);

        aux = strtok(NULL, sep);
        s.nrTotalElevi = atoi(aux);

        inserareHash(tabela, s);
    }

    fclose(f);

    traversareHash(tabela);
    dezalocare(tabela);

    return 0;
}