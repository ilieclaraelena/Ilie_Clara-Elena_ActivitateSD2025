#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<malloc.h>
#include<string.h>

typedef struct Carte Carte;
typedef struct nod nod;

struct Carte {
	int id;
	char* autor;
	int an;
	float pret;
};

Carte creereCarte(int id, const char* autor, int an, float pret) {
	Carte c;
	c.id = id;
	c.autor = (char*)malloc(sizeof(char) * (strlen(autor) + 1));
	strcpy_s(c.autor, sizeof(char) * (strlen(autor) + 1), autor);
	c.an = an;
	c.pret = pret;
	return c;
}

void afisare(struct Carte c) {
	printf("Id: %d\n", c.id);
	printf("Autor: %s\n", c.autor);
	printf("An: %d\n", c.an);
	printf("Pret: %5.2f\n", c.pret);
}

struct nod {
	Carte info;
	nod* next;
};

nod* inserareListaSimpla(nod* cap, Carte c) {
	nod* nou = (nod*)malloc(sizeof(nod));
	nou->info = creereCarte(c.id, c.autor, c.an, c.pret);
	nou->next = NULL;
	if (cap == NULL) {
		cap = nou;
	}
	else {
		nod* p = cap;
		while (p->next != NULL) {
			p = p->next;
		}
		p->next = nou;
	}
	return cap;
}
void afisareLista(nod* cap) {
	nod* aux = cap;
	while (aux) {
		afisare(aux->info);
		aux = aux->next;
	}
}
void dezalocare(nod** cap) {
	nod* temp;
	while (*cap) {
		temp = *cap;
		*cap = (*cap)->next;
		free(temp->info.autor);
		free(temp);
	}
}

void main() {
	Carte c;
	nod* cap = NULL;

	char liniebuffer[100];
	char* aux;
	FILE* f = fopen("p2.txt", "r");
	if (f) {
		while (fgets(liniebuffer, sizeof(liniebuffer), f)) {
			aux = strtok(liniebuffer, ",");
			c.id = atoi(aux);

			aux = strtok(NULL, ",");
			c.autor = malloc(sizeof(char) * strlen(aux) + 1);
			strcpy_s(c.autor, strlen(aux) + 1, aux);

			aux = strtok(NULL, ",");
			c.an = atoi(aux);

			aux = strtok(NULL, ",");
			c.pret = atof(aux);

			cap = inserareListaSimpla(cap, c);
		}
	}
	fclose(f);
	afisareLista(cap);


}