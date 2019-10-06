#include <stdio.h>
#include <string.h>
#include "token.h"
#include "preprocessador.h"

#define TAMANHO_MAXIMO_ARQUIVO 1024
#define TAMANHO_MAX_TABELA_DE_SIMBOLOS 1024
void monta(char *, char *);

int main(int argc, char *argv[]){
	int i;
	//abre e carrega arquivo na memoria
	if (argc != 2) {
		printf("uso incorreto.\n");
		return 0;
		}

	char programa[TAMANHO_MAXIMO_ARQUIVO],
		 executavel[TAMANHO_MAXIMO_ARQUIVO];

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
	//gerar arquivo
	monta(programa, executavel);
	//gerar arquivo
	return 0;
}

typedef struct Simbolo{
	//char string[100];		//texto do simbolo
	Token token;
	int valor, isDef, lista;
} TabelaSimbolos;



void monta(char *texto, char *executavel){
	Token token;
	int posicao = 0, posicaoAuxiliar;
	Simbolo tabelaDeSimbolos[TAMANHO_MAX_TABELA_DE_SIMBOLOS];
	int cursorTabela = 0, cursorExecutavel = 0;

	while(texto[posicao]!='\0'){
	posicaoAuxiliar = posicao;	//grava posicao
	posicao += token.leUmToken(texto, posicao);
		//remove espacos desnecessarios.
		while(token.tipo == QUEBRA_DE_LINHA || token.tipo == ESPACO || token.tipo == TABULACAO) {
			posicaoAuxiliar = posicao;
			posicao += token.leUmToken(texto,posicao);
		}
	}
}













