#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<malloc.h>
#include<string.h>

typedef struct comanda comanda;
typedef struct nod nod;

struct comanda {
	unsigned int id;
	unsigned char nrProduseComandate;
	char* client;
	float valoareComanda;
};

comanda initializareComanda(unsigned int id, unsigned char nrProduseComandate, const char* client, float valoareComanda) {
	comanda c;
	c.id = id;
	c.nrProduseComandate = nrProduseComandate;
	c.client = (char*)malloc(sizeof(char) * strlen(client) + 1);
	strcpy_s(c.client, sizeof(char) * strlen(client) + 1, client);
	c.valoareComanda = valoareComanda;
	return c;
}
void afisare(comanda c) {
	printf("%u,%u, %s, %.2f\n", c.id, c.nrProduseComandate, c.client, c.valoareComanda);
}
struct nod {
	comanda info;
	nod* st;
	nod* dr;
};

nod* initializareNod(nod* st, nod* dr, comanda c) {
	nod* nou = (nod*)malloc(sizeof(nod));
	nou->info = initializareComanda(c.id, c.nrProduseComandate, c.client, c.valoareComanda);
	nou->st = st;
	nou->dr = dr;
	return nou;
}
nod* inserareArb(nod* rad, comanda c) {
	if (rad) {
		if (c.id < rad->info.id) {
			rad->st = inserareArb(rad->st, c);
			return rad;
		}
		else if (c.id > rad->info.id) {
			rad->dr = inserareArb(rad->dr, c);
			return rad;
		}
		else {
			return rad;
		}
	}
	else {
		return initializareNod(NULL, NULL, c);
	}
}

void inordine(nod* rad) {
	if (rad) {
		inordine(rad->st);
		printf("%u,%u, %s, %.2f\n", rad->info.id, rad->info.nrProduseComandate, rad->info.client, rad->info.valoareComanda);
		inordine(rad->dr);
	}
}

void dezalocare(nod* rad) {
	if (rad) {
		nod* arbSt = rad->st;
		nod* arbDr = rad->dr;
		free(rad->info.client);
		free(rad);
		dezalocare(arbDr);
		dezalocare(arbSt);
	}
}
int nrComenziCuAcProd(nod* rad, unsigned char nrProduseComandate) {
	int counter = 0;
	if (rad == NULL) {
		return 0;
	}
	if (nrProduseComandate == rad->info.nrProduseComandate) {
		counter = 1;
	}
	return nrComenziCuAcProd(rad->dr, nrProduseComandate) + nrComenziCuAcProd(rad->st, nrProduseComandate) + counter;
}

comanda Cautare(nod* rad, float valoareComanda) {
	if (rad) {
		if (valoareComanda == rad->info.valoareComanda) {
			return initializareComanda(rad->info.id, rad->info.nrProduseComandate, rad->info.client, rad->info.valoareComanda);
		}
		else {
			if (valoareComanda < rad->info.valoareComanda) {
				return Cautare(rad->st, valoareComanda);
			}
			else {
				return Cautare(rad->dr, valoareComanda);
			}
		}
	}
}
void main() {
	comanda c;
	nod* rad = NULL;

	char sep[] = ",";
	char linieBuffer[100];
	char* token;

	FILE* f = fopen("p10.txt", "r");
	while (fgets(linieBuffer, 100, f)) {
		if (f) {
			token = strtok(linieBuffer, sep);
			c.id = atoi(token);

			token = strtok(NULL, sep);
			c.nrProduseComandate = atoi(token);

			token = strtok(NULL, sep);
			c.client = (char*)malloc(sizeof(char) * strlen(token) + 1);
			strcpy_s(c.client, sizeof(char) * strlen(token) + 1, token);

			token = strtok(NULL, sep);
			c.valoareComanda = atof(token);

			rad = inserareArb(rad, c);
			free(c.client);

		}
		else {
			printf("FISIER INVLAID");
		}
	}
	fclose(f);
	printf("INORDINE\n");
	inordine(rad);

	unsigned char produseCaut = 5;
	int counter = nrComenziCuAcProd(rad, produseCaut);
	printf("Prod %u \n", counter);

	comanda comandaCautat = Cautare(rad, 320.0);
	afisare(comandaCautat);

	dezalocare(rad);
}