#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<malloc.h>
#include<string.h>

typedef struct carte carte;
typedef struct nod nod;

struct carte {
	int id;
	char* autor;
	int an;
	float pret;
};

carte creareCarte(int id, const char* autor, int an, float pret) {
	carte c;
	c.id = id;
	c.autor = (char*)malloc(sizeof(char) * strlen(autor) + 1);
	strcpy_s(c.autor, sizeof(char) * strlen(autor) + 1, autor);
	c.an = an;
	c.pret = pret;
	return c;
}
struct nod {
	carte info;
	nod* st;
	nod* dr;
};

nod* creeareNod(carte c, nod* st, nod* dr) {
	nod* nou = (nod*)malloc(sizeof(nod) + 1);
	nou->info = creareCarte(c.id, c.autor, c.an, c.pret);
	nou->st = st;
	nou->dr = dr;
	return nou;
}
nod* inserareArb(nod* rad, carte c) {
	if (rad) {
		if (c.id < rad->info.id) {
			rad->st = inserareArb(rad->st, c);
			return rad;
		}
		else if (c.id > rad->info.id) {
			rad->dr = inserareArb(rad->dr, c);
			return rad;
		}
		else return rad;
	}
	else creeareNod(c, NULL, NULL);
}
void preordine(nod* rad) {
	if (rad) {
		printf("%u,%s,%u,%.2f\n", rad->info.id, rad->info.autor, rad->info.an, rad->info.pret);
		preordine(rad->st);
		preordine(rad->dr);

	}
}
void dezalocare(nod* rad) {
	if (rad) {
		nod* arbst = rad->st;
		nod* arbdr = rad->dr;
		free(rad->info.autor);
		free(rad);
		dezalocare(arbst);
		dezalocare(arbdr);
	}
}

void main() {
	carte c;
	nod* rad = NULL;

	char linieBuffer[100];
	char sep[] = ",";
	char* token;

	FILE* f = fopen("p7.txt", "r");
	if (f) {
		while (fgets(linieBuffer, 100, f)) {
			token = strtok(linieBuffer, sep);
			c.id = atoi(token);

			token = strtok(NULL, sep);
			c.autor = (char*)malloc(sizeof(char) * strlen(token) + 1);
			strcpy_s(c.autor, strlen(token) + 1, token);

			token = strtok(NULL, sep);
			c.an = atoi(token);

			token = strtok(NULL, sep);
			c.pret = atof(token);
			rad = inserareArb(rad, c);
			free(c.autor);
		}
	}
	else {
		printf("Fisier invalid");
	}
	fclose(f);

	preordine(rad);
}