#include <stdio.h>
#include <string.h>
#include "token.h"
#include "preprocessador.h"

#define TAMANHO_MAXIMO_ARQUIVO 1024

void monta(char *);

int main(int argc, char *argv[]){
	int i;
	//abre e carrega arquivo na memoria
	if (argc != 2) {
		printf("uso incorreto.\n");
		return 0;
		}

	char programa[TAMANHO_MAXIMO_ARQUIVO],
		 programaPreProcessado[TAMANHO_MAXIMO_ARQUIVO];

	int tamanhoArquivo = 0;
	FILE *file = NULL;

	file = fopen(argv[1],"r");
	if (file == NULL){
		printf("arquivo não encontrado\n");
		return 0;
	}

	while (!feof(file) && tamanhoArquivo < TAMANHO_MAXIMO_ARQUIVO){
		//printf ("%d", i);
		fread(programa+tamanhoArquivo, sizeof(char), 1, file); 
		tamanhoArquivo++;		
	}
	programa[--tamanhoArquivo] = 0;

	fclose(file);

	preProcessa(programa, tamanhoArquivo);
	monta(programa);
	
	return 0;
}

void monta(char *a){
	
}

