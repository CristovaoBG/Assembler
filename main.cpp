#include <stdio.h>
#include <string.h>
#include <list>
#include "token.h"
#include "preprocessador.h"
#include "montador.h"

#define TAMANHO_MAXIMO_ARQUIVO 1024

int main(int argc, char *argv[]){
	int i, tamanhoExecutavel;
	char nomeBuffer[100];
	//abre e carrega arquivo na memoria
	if (argc != 2) {
		printf("uso incorreto.\n");
		return 0;
		}

	char programa[TAMANHO_MAXIMO_ARQUIVO];
	int	 executavel[TAMANHO_MAXIMO_ARQUIVO];

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
	//gera arquivo preprocessado
	FILE *arquivoPreProcessado = NULL, *arquivoExecutavel = NULL;
	//adiciona extensao ".pre"
	strcpy(nomeBuffer, argv[1]);
	for(i = strlen(nomeBuffer); nomeBuffer[i] != '.' && i>0; i--);	
	if(i==0) i=strlen(nomeBuffer);
	strcpy(nomeBuffer+i, ".pre");
	//salva arquivo.pre
	arquivoPreProcessado = fopen(nomeBuffer,"w");
	fprintf(arquivoPreProcessado,"%s",programa);
	//restrutura na forma SECTION TEXT seguida de SECTION DATA
	reestruturaSections(programa);
	//monta
	tamanhoExecutavel = monta(programa, executavel);
	//adiciona extensao ".obj"
	strcpy(nomeBuffer, argv[1]);
	for(i = strlen(nomeBuffer); nomeBuffer[i] != '.' && i>0; i--);	
	if(i==0) i=strlen(nomeBuffer);
	strcpy(nomeBuffer+i, ".obj");
	//salva arquivo.obj
	arquivoExecutavel = fopen(nomeBuffer,"w");
	for(i = 0; i<tamanhoExecutavel; i++){
		fprintf(arquivoExecutavel,"%d ",executavel[i]);
	}
	return 0;
}





