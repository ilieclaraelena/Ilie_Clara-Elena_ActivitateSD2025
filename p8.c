#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<malloc.h>
#include<string.h>

typedef struct rezervare rezervare;
typedef struct nod nod;


struct rezervare {
	unsigned int id;
	char* hotel;
	unsigned char nrCamere; 
	char* client;
	float suma;
};

rezervare creareRezervare(unsigned int id, const char* hotel, unsigned char nrCamere, const char* client, float suma) {
	rezervare r;
	r.id = id;
	r.hotel = (char*)malloc(sizeof(char) * strlen(hotel) + 1);
	strcpy(r.hotel, hotel);
	r.nrCamere = nrCamere;
	r.client = (char*)malloc(sizeof(char) * strlen(client) + 1);
	strcpy(r.client, client);
	r.suma = suma;

	return r;
}

struct nod {
	rezervare info;
	nod* st;
	nod* dr;
	int grad;
};

nod* creeareNod(rezervare r, nod* st, nod* dr) {
	nod* nou = (nod*)malloc(sizeof(nod));
	nou->info = creareRezervare(r.id, r.hotel, r.nrCamere, r.client, r.suma);
	nou->st = st;
	nou->dr = dr;
	return nou;
}

nod* inserareArb(nod* rad, rezervare r) {
	if (rad) {
		if (r.id < rad->info.id) {
			rad->st = inserareArb(rad->st, r);
			return rad;
		}
		else if (r.id > rad->info.id) {
			rad->dr = inserareArb(rad->dr, r);
			return rad;
		}
		else return rad;
	}
	else {
		return creeareNod(r, NULL, NULL);
	}
}

void preordine(nod* rad) {
	if (rad) {
		printf("GRAD: %d, %u, %s,%u,%s,%.2f\n", rad->grad, rad->info.id, rad->info.hotel, rad->info.nrCamere, rad->info.client, rad->info.suma);
		preordine(rad->st);
		preordine(rad->dr);

	}
}

void inordine(nod* rad) {
	if (rad) {
		inordine(rad->st);
		printf("GRAD: %d, %u, %s,%u,%s,%.2f\n", rad->grad, rad->info.id, rad->info.hotel, rad->info.nrCamere, rad->info.client, rad->info.suma);
		inordine(rad->dr);
	}
}

void postordine(nod* rad) {
	if (rad) {
		postordine(rad->st);
		postordine(rad->dr);
		printf("GRAD: %d, %u, %s,%u,%s,%.2f\n", rad->grad, rad->info.id, rad->info.hotel, rad->info.nrCamere, rad->info.client, rad->info.suma);
	}
}

void dezalocare(nod* rad) {
	if (rad) {
		nod* arbst = rad->st;
		nod* arbdr = rad->dr;
		free(rad->info.hotel);
		free(rad->info.client);
		free(rad);
		dezalocare(arbst);
		dezalocare(arbdr);

	}
}

int maxim(int a, int b) {
	return a > b ? a : b;
}
int nrNiveluri(nod* rad) {
	if (rad) {
		return 1 + max(nrNiveluri(rad->st), nrNiveluri(rad->dr));
	}
}
//nr total camere rezervate din arbore
int nrTotalCamere(nod* rad, int* nrTotal) {
	if (rad) {
		*nrTotal += rad->info.nrCamere;
		nrTotalCamere(rad->st, nrTotal);
		nrTotalCamere(rad->dr, nrTotal);
		return (*nrTotal);
	}

}

void calculgrad(nod* rad) {
	if (rad) {
		rad->grad = (nrNiveluri(rad->st) - nrNiveluri(rad->dr));
		calculgrad(rad->st);
		calculgrad(rad->dr);
	}
}
nod* rotatieDreapta(nod* rad) {
	nod* x = rad->st;
	rad->st = x->dr;
	x->dr = rad;
	return x;
	
}
nod* rotatieStanga(nod* rad) {
	nod* x = rad->dr;
	rad->dr = x->st;
	x->st = rad;
	return x;
	;
}

nod* rotatieStangaDreapta(nod* rad) {
	rad->st = rotatieStanga(rad->st);
	return rotatieDreapta(rad);
}
nod* rotatieDreaptaStanga(nod* rad) {
	rad->dr = rotatieDreapta(rad->dr);
	return rotatieStanga(rad);
}
nod* echilibrareAVL(nod* rad) {
	if (rad) {
		rad->st = echilibrareAVL(rad->st);
		rad->dr = echilibrareAVL(rad->dr);
	}
	
	rad->grad = (nrNiveluri(rad->st) - nrNiveluri(rad->dr));
	if (rad->grad > 1) {
		if (rad->st && rad->st->grad >= 0) {
			return rotatieDreapta(rad);
		}
		else {
			return rotatieStangaDreapta(rad);
		}

	}
	else if (rad->grad > -1) {
		if (rad->dr && rad->dr->grad <= 0) {
			return rotatieStanga(rad);
		}
		else {
			return rotatieDreaptaStanga(rad);
		}
	}
	return rad;
}
void main() {
	rezervare r;
	nod* rad = NULL;

	char linieBuffer[100];
	char sep[] = ",\n";
	char* token;
	FILE* f = fopen("p8.txt", "r");
	if (f) {
		while (fgets(linieBuffer, 100, f)) {
			token = strtok(linieBuffer, sep);
			r.id = atoi(token);

			token = strtok(NULL, sep);
			r.hotel = (char*)malloc(sizeof(char) * strlen(token) + 1);
			strcpy(r.hotel, token);

			token = strtok(NULL, sep);
			r.nrCamere = atoi(token);

			token = strtok(NULL, sep);
			r.client = (char*)malloc(sizeof(char) * strlen(token) + 1);
			strcpy(r.client, token);

			token = strtok(NULL, sep);
			r.suma = atof(token);

			rad = inserareArb(rad, r);
			free(r.client);
			free(r.hotel);


		}
	}
	else {
		printf("Fisier invalid!");
	}
	fclose(f);
	calculgrad(rad);
	printf("\n----------Preordine-------\n");
	preordine(rad);


	rad = rotatieStanga(rad);
	calculgrad(rad);
	printf("\n-----Afisare dupa rotati--------\n");
	preordine(rad);

	rad = echilibrareAVL(rad);

}
