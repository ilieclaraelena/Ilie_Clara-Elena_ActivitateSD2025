#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<malloc.h>
#include<string.h>


typedef struct Masina {
	int id;
	int caiPutere;
	char* producator;
	float pret;
	char serie;
}Masina;

Masina initializareMasina(int id, int caiPutere, const char* producator, float pret, char serie) {
	Masina m;
	m.id = id;
	m.caiPutere = caiPutere;
	m.producator = (char*)malloc(sizeof(char) * (strlen(producator) + 1));
	strcpy(m.producator, producator);
	m.pret = pret;
	m.serie = serie;
	return m;
}
void afisare(struct Masina m) {
	printf("%d, Masina: %s, Cai putere: %d, Pret: %5.2f, Seria: %c\n", m.id, m.producator, m.caiPutere, m.pret, m.serie);
}

void modificaPret(struct Masina* m, float noulPret) {
	if (noulPret != 0) {
		m->pret = noulPret;

	}
}
void dezalocare(struct Masina* m) {
	if (m->producator != NULL) {
		free(m->producator);
		m->producator = NULL;
	}
}

void main() {
	Masina m;
	m = initializareMasina(1, 300, "Audi", 56.7, 'A');

	afisare(m);

	modificaPret(&m, 99.7);
	afisare(m);

	dezalocare(&m);
	afisare(m);

	return 0;
}