#include <stdio.h>
#include <string.h>
#include "token.h"


#define TAMANHO_MAXIMO_ARQUIVO 1024

void preProcessa(char *);
void monta(char *);


void criaListaDeTokens(char *string, int tamanho){
	//printf(string);
//	char *token, tokenString[50];
//	list<Token> tokenList;
//	Token token;
	char *token;
	ListaToken listaToken;
	
	Token token1, token2;
	strcpy(token1.string,"o homem macaco correu atras de mim");
	strcpy(token2.string,"o homem macaco que nao tem alma e nem coracao");

	listaToken.add(token1);
	listaToken.add(token2);

	printf("%s,,,%s",listaToken.prox()->string,listaToken.prox()->string);
	

	printf("\n\n%s\n",string);

	token = strtok(string, " ");
	while(token!=NULL){
		printf("%s\n", token);
		token = strtok(NULL," ");
	}
	printf("\n\n%s\n",string);
}

int main(int argc, char *argv[]){
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

	criaListaDeTokens(programa, tamanhoArquivo);
	preProcessa(programa);
	monta(programaPreProcessado);
	
	return 0;
}


void preProcessa(char *a){

}

void monta(char *a){

}

