#define _CRT_SECURE_NO_WARNINGS
#include<stdlib.h>
#include<stdio.h>
#include<malloc.h>
#include<string.h>

typedef struct reteta reteta;
typedef struct nod nod;
typedef struct HashTable HashTable;

struct reteta {
	unsigned int nr;
	unsigned char medicamPrescrise;
	char* numePacient;
	char* numeMedic;
	float valoareReteta;
};

reteta initializareReteta(unsigned int nr, unsigned char medicamPrescrise, const char* numePacient, const char* numeMedic, float valoareReteta) {
	reteta r;
	r.nr = nr;
	r.medicamPrescrise = medicamPrescrise;
	r.numePacient = malloc(sizeof(char) * strlen(numePacient) + 1);
	strcpy_s(r.numePacient, sizeof(char) * strlen(numePacient) + 1, numePacient);
	r.numeMedic = malloc(sizeof(char) * strlen(numeMedic) + 1);
	strcpy_s(r.numeMedic, sizeof(char) * strlen(numeMedic) + 1, numeMedic);
	r.valoareReteta = valoareReteta;
	return r;
}
void afisareReteta(reteta r) {
	printf("%d, %u, %s, %s, %.2f", r.nr, r.medicamPrescrise, r.numePacient, r.numePacient, r.valoareReteta);
	printf("\n");
}


struct nod {
	reteta info;
	nod* next;
};

struct HashTable {
	int dim;
	nod** vector;
};

int functieHashNr(HashTable tabela, unsigned int nr) {
	return nr % tabela.dim;
}

int functieHashNumePacient(HashTable tabela, const char* numePacient) {
	return strlen(numePacient) % tabela.dim;
}

int inserareHash(HashTable tabela, reteta r) {
	int pozitie = 0;
	if (tabela.vector) {
		pozitie = functieHashNr(tabela, r.nr);
		nod* nou = (nod*)malloc(sizeof(nod));
		nou->info = initializareReteta(r.nr, r.medicamPrescrise, r.numePacient, r.numePacient, r.valoareReteta);
		nou->next = NULL;
		if (tabela.vector[pozitie] == NULL) {
			tabela.vector[pozitie] = nou;
		}
		else {
			nod* aux = tabela.vector[pozitie];
			while (aux->next!=NULL) {
				aux = aux->next;
			}
			aux->next = nou;
		}
		return pozitie;
	}
}

int traversareHash(HashTable tabela) {
	if (tabela.vector) {
		for (int i = 0; i < tabela.dim; i++) {
			printf("Pozitia: %d\n", i);
			nod* aux = tabela.vector[i];
			while (aux) {
				afisareReteta(aux->info);
				aux = aux->next;
			}
		}
	}
}
void dezalocare(HashTable tabela) {
	if (tabela.vector) {
		for (int i = 0; i < tabela.dim; i++) {
			if (tabela.vector[i]) {
				nod* aux = tabela.vector[i];
				while (aux) {
					free(aux->info.numePacient);
					free(aux->info.numeMedic);
					nod* temp = aux->next;
					free(aux);
					aux = temp;
				}
			}
		}
		free(tabela.vector);
	}
}
double valoareTotalaRetete(HashTable tabela, const char* numePacient) {
	double suma = 0;
	if (tabela.vector) {
		for (int i = 0; i < tabela.dim;i++) {
			nod* aux = tabela.vector[i];
			while (aux) {
				if (strcmp(aux->info.numePacient, numePacient) == 0) {
					suma += aux->info.valoareReteta;
				}
				aux = aux->next;
			}
		}
	}
	return suma;
}
void main() {
	reteta r;
	HashTable tabela;
	tabela.dim = 8;
	tabela.vector = (nod*)malloc(sizeof(nod) * tabela.dim);
	for (int i = 0; i < tabela.dim; i++) {
		tabela.vector[i] = NULL;
	}

	char linieBuffer[100];
	char* aux;

	FILE* f = fopen("p3.txt", "r");
	if (f) {
		while (fgets(linieBuffer, sizeof(linieBuffer), f)) {
			aux = strtok(linieBuffer, ",");
			r.nr = atoi(aux);

			aux = strtok(NULL, ",");
			r.medicamPrescrise = atoi(aux);

			aux = strtok(NULL, ",");
			r.numePacient = malloc(sizeof(char) * strlen(aux) + 1);
			strcpy_s(r.numePacient, sizeof(char) * strlen(aux) + 1, aux);

			aux = strtok(NULL, ",");
			r.numeMedic = malloc(sizeof(char) * strlen(aux) + 1);
			strcpy_s(r.numeMedic, sizeof(char) * strlen(aux) + 1, aux);

			aux = strtok(NULL, ",");
			r.valoareReteta = atof(aux);

			inserareHash(tabela, r);

			free(r.numePacient);
			free(r.numeMedic);
		}
	}
	fclose(f);
	traversareHash(tabela);

	printf("Valoarea totala a retetelor: %.2f", valoareTotalaRetete(tabela, "Ilie Gabriela"));
}
