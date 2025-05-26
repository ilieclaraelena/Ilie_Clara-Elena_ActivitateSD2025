#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<malloc.h>
#include<stdlib.h>
#include<string.h>

typedef struct CursaFeroviara CursaFeroviara;
typedef struct nod nod;

struct CursaFeroviara {
	unsigned int nrCursa;
	char* dataCursa;
	unsigned char nrVagoane;
	unsigned int nrBilete;
	float* pretBilete;
};
CursaFeroviara initializareCursa(unsigned int nrCursa, const char* dataCursa, unsigned char nrVagoane, unsigned int nrBilete, float* pretBilete) {
	CursaFeroviara c;
	c.nrCursa = nrCursa;
	c.dataCursa = (char*)malloc(sizeof(char) * strlen(dataCursa) + 1);
	strcpy_s(c.dataCursa, sizeof(char) * strlen(dataCursa) + 1, dataCursa);
	c.nrVagoane = nrVagoane;
	c.nrBilete = nrBilete;
	c.pretBilete = (float*)malloc(sizeof(float)*nrBilete);
	for (int i = 0;i < c.nrBilete;i++) {
		c.pretBilete[i] = pretBilete[i];
	}
	return c;
}

struct nod {
	CursaFeroviara info;
	nod* st;
	nod* dr;
};

nod* creareNod(CursaFeroviara c,nod* st, nod*dr) {
	nod* nou = (nod*)malloc(sizeof(nod));
	nou->info = initializareCursa(c.nrCursa, c.dataCursa, c.nrVagoane, c.nrBilete, c.pretBilete);
	nou->st = st;
	nou->dr = dr;
	return nou;
}
nod* initializareArb(nod* rad, CursaFeroviara c) {
	if (rad) {
		if (c.nrCursa < rad->info.nrCursa) {
			rad->st = initializareArb(rad->st, c);
			return rad;
		}
		else if (c.nrCursa > rad->info.nrCursa) {
			rad->dr = initializareArb(rad->dr, c);
			return rad;
		}
		else { return rad; }
	}
	else {
		return creareNod(c, NULL, NULL);
	}
}
//SRD
void inordine(nod*rad) {
	if (rad) {
		inordine(rad->st);
		printf("%u, %s, %u, %u\n", rad->info.nrCursa, rad->info.dataCursa, rad->info.nrVagoane, rad->info.nrBilete);
		for (int i = 0;i < rad->info.nrBilete;i++) {
			printf("%.2f", rad->info.pretBilete[i]);
		}
		inordine(rad->dr);
	}
}

void dezalocare(nod* rad) {
	if (rad) {
		nod* arbst = rad->st;
		nod* arbDr = rad->dr;
		free(rad->info.dataCursa);
		free(rad->info.pretBilete);
		free(rad);
		dezalocare(arbst);
		dezalocare(arbDr);
	}
}

int main() {
	CursaFeroviara c;
	nod* rad = NULL;

	char linieBuffer[100];
	char sep[] = " \n";
	char* token;

	FILE* f = fopen("p9.txt", "r");
	if (f) {
		while (fgets(linieBuffer, 100, f)) {
			token = strtok(linieBuffer, sep);
			c.nrCursa = atoi(token);

			token = strtok(NULL, sep);
			c.dataCursa = (char*)malloc(sizeof(char) * strlen(token) + 1);
			strcpy_s(c.dataCursa, sizeof(char) * strlen(token) + 1, token);

			token = strtok(NULL, sep);
			c.nrVagoane = atoi(token);


			token = strtok(NULL, sep);
			c.nrBilete = atoi(token);

			c.pretBilete = (float*)malloc(sizeof(float) * c.nrBilete);
			for (int i = 0; i < c.nrBilete; i++) {
				token = strtok(NULL, sep);
				if (!token) break;
				c.pretBilete[i] = atof(token);
			}

			rad = initializareArb(rad, c);
			free(rad->info.dataCursa);
			free(rad->info.pretBilete);
		}
	}
	else {
		printf("Fisier INVALID");
	}

	fclose(f);
	inordine(rad);
}