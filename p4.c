#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include<stdlib.h>
#include<malloc.h>
#include<string.h>

typedef struct farmacie farmacie;
typedef struct nod nod;
typedef struct LD LD;

struct farmacie {
	char* denumire;
	float suprafata;
	unsigned char nrAngajati;
	char* program;
	int oraDeschidere;
};
farmacie initializareFarmacie(const char* denumire, float suprafata, unsigned char nrAngajati, const char* program, int oraDeschidere) {
	farmacie f;
	f.denumire = malloc(sizeof(char) * (strlen(denumire) + 1));
	strcpy_s(f.denumire, sizeof(char) * strlen(denumire) + 1, denumire);
	f.suprafata = suprafata;
	f.nrAngajati = nrAngajati;
	f.program = malloc(sizeof(char) * (strlen(program) + 1));
	strcpy_s(f.program, sizeof(char) * strlen(program) + 1, program);
	f.oraDeschidere = oraDeschidere;
	return f;
}
void afisareFarmacie(farmacie f) {
	printf("Denumire:%s, Suprafata:%.2f, Nr angajati:%u, Program: %s, Ora deschidere:%d\n", f.denumire, f.suprafata, f.nrAngajati, f.program, f.oraDeschidere);
}
struct nod {
	nod* prev;
	nod* next;
	farmacie info;
};

struct LD {
	nod* prim;
	nod* ultim;
};

LD inserareLDInceput(LD lista, farmacie f) {
	nod* nou = (nod*)malloc(sizeof(nod));
	nou->prev = NULL;
	nou->info = initializareFarmacie(f.denumire, f.suprafata, f.nrAngajati, f.program, f.oraDeschidere);
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
LD inserareLDFinalt(LD lista, farmacie f) {
	nod* nou = (nod*)malloc(sizeof(nod));
	nou->prev = NULL;
	nou->info = initializareFarmacie(f.denumire, f.suprafata, f.nrAngajati, f.program, f.oraDeschidere);
	nou->next = NULL;
	if (lista.prim == NULL) {
		lista.prim = lista.ultim = nou;
	}
	else {
		lista.ultim->next = nou;
		nou->prev = lista.ultim;
		lista.ultim = nou;
	}
	return lista;
}

void traversareLDFinal(LD lista) {
	nod* aux = lista.ultim;
	while (aux) {
		afisareFarmacie(aux->info);
		aux = aux->prev;
	}

}
void dezalocare(LD lista) {
	nod* aux = lista.prim;
	while (aux) {
		free(aux->info.denumire);
		free(aux->info.program);
		nod* temp = aux->next;
		free(aux);
		aux = temp;
	}
}

int nrFarmaciiDeschiseLaOrASpecificata(LD lista, int oraDeschidere) {
	nod* aux = lista.prim;
	int counter = 0;
	while (aux) {
		if (aux->info.oraDeschidere == oraDeschidere) {
			counter++;
		}
		aux = aux->next;

	}
	return counter;
}

/*
caz 1:s>min && s>p => min=s, p=min;
caz2: s==min &&s>p => min=s, p=min;
caz3: s==min && s=p=> -
caz4: s<min &&s>p => p=s,

*/
int farmaciiCuSupraftaMare(LD lista) {
	int min = -1;
	int penultimMin = -1;
	farmacie* minFarmacie = NULL;
	farmacie* penultimFarmacie = NULL;
	nod* aux = lista.prim;
	while (aux) {
		if (aux->info.suprafata >= min) {
			penultimMin = min;
			min = aux->info.suprafata;
			penultimFarmacie = minFarmacie;
			minFarmacie = &aux->info;
		}
		else if (aux->info.suprafata > penultimMin) {
			penultimMin = aux->info.suprafata;
			penultimFarmacie = &aux->info;

		}
		aux = aux->next;
	}
	printf("Farmacii cu suprafata max:%s si penultim:%s", minFarmacie->denumire, penultimFarmacie->denumire);
	return 0;
}

void main() {
	farmacie f;
	LD lista;
	lista.prim = NULL;
	lista.ultim = NULL;

	char linieBuffer[100];
	char* aux;
	char sep[] = ",";
	FILE* file = fopen("p4.txt", "r");
	if (file) {
		while (fgets(linieBuffer, 100, file)) {
			aux = strtok(linieBuffer, sep);
			f.denumire = malloc(sizeof(char) * strlen(aux) + 1);
			strcpy_s(f.denumire, sizeof(char) * strlen(aux) + 1, aux);

			aux = strtok(NULL, sep);
			f.suprafata = atof(aux);

			aux = strtok(NULL, sep);
			f.nrAngajati = atoi(aux);

			aux = strtok(NULL, sep);
			f.program = malloc(sizeof(char) * strlen(aux) + 1);
			strcpy_s(f.program, sizeof(char) * strlen(aux) + 1, aux);

			aux = strtok(NULL, sep);
			f.oraDeschidere = atoi(aux);

			lista = inserareLDInceput(lista, f);
			free(f.denumire);
			free(f.program);
		}
	}
	fclose(file);
	traversareLDFinal(lista);

	printf("Nr farmacii: %d\n", nrFarmaciiDeschiseLaOrASpecificata(lista, 8));
	farmaciiCuSupraftaMare(lista);
	dezalocare(lista);
}