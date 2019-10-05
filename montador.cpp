#include <stdio.h>
#include <string.h>
#include "token.h"


#define TAMANHO_MAXIMO_ARQUIVO 1024

void preProcessa(char *);
void monta(char *);

void removeCaracteres(char *string,int nCaracteres){ //remove o numero nCaracteres de caracteres na posicao de string 
	int cursor = 0;
	while (string[cursor + nCaracteres]!='\0'){
		string[cursor] = string[cursor + nCaracteres];
		cursor++;
	}
	string[cursor] = '\0';
}

void insereStringNaPosicao(char *strAlvo, char *str){ //insere str na posicao strAlvo
	int tamanhoAlvo = strlen(strAlvo);
	int tamanhoStr = strlen(str);
	int cursor = tamanhoAlvo;
	//libera espaco
	while(cursor >=0){
		strAlvo[cursor+tamanhoStr] = strAlvo[cursor];
		cursor--;
	}
	//insere str
	cursor = 0;
	while(str[cursor] != '\0'){
		strAlvo[cursor] = str[cursor];
		cursor++;
	}
}

void criaListaDeTokens(char *string, int tamanho){
	//printf(string);
//	char *token, tokenString[50];
//	list<Token> tokenList;
//	Token token;
	int posicao = 0, posicaoAuxiliar = 0;

	insereStringNaPosicao(string+4, "meu pau juvenal");

	ListaToken listaDeEqus;
	Token token;
	/*
//	Token token, token1, token2, token3, token4;
	strcpy(token1.string,"A ");
	strcpy(token2.string,"B ");

	listaToken.add(token1);
	strcpy(token3.string,"C ");
	strcpy(token4.string,"D ");
	listaToken.add(token2);
	listaToken.add(token3);
	listaToken.add(token4);

	listaToken.reinicia();

	listaToken.printaTodos();
	*/
	printf("\n\n%s\n",string);
	
	while(string[posicao]!='\0'){
//		posicao += token.leUmToken(string, posicao);
//		continue;
		//printf("c = CARACTERE:%c\n",string[posicao]);
		//verifica se EQU
		Token rotulo, valorEqu;
		posicaoAuxiliar = posicao;	//grava posicao antes de verificar se eh equ
		posicao += token.leUmToken(string, posicao);
		//printf("\n\n");
		while(token.tipo == QUEBRA_DE_LINHA || token.tipo == ESPACO || token.tipo == TABULACAO) posicao += token.leUmToken(string,posicao);	
		//add na lista		
		if (token.tipo == PALAVRA){
			rotulo = token; //so faz sentido se houver dois pontos a seguir. mas ja assume que vai ter
			posicao += token.leUmToken(string,posicao);		
			if (token.tipo == DOIS_PONTOS){	
				posicao += token.leUmToken(string,posicao);
				while(token.tipo == QUEBRA_DE_LINHA || token.tipo == ESPACO || token.tipo == TABULACAO){
					posicao += token.leUmToken(string,posicao);
				}
				//add na lista
				if(token.tipo == EQU){
					posicao += token.leUmToken(string,posicao);
					while(token.tipo == QUEBRA_DE_LINHA || token.tipo == ESPACO || token.tipo == TABULACAO){
						posicao += token.leUmToken(string,posicao);
					}
					
					if(token.tipo == NUMERO){
						valorEqu = token;
						char rotuloStr[100],valorEquStr[100];
						rotulo.copiaTokenParaString(rotuloStr);
						valorEqu.copiaTokenParaString(valorEquStr);
						printf("estrutura de EQ correta. Rotulo: %s Valor: %s\n",rotuloStr,valorEquStr);		
						//varre o texto substituindo
					}
				}
			}
		}
		//posicao++;
		//getchar();
		//break;
	}
	printf("\n\n%s\n",string);

}

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
	
	//bota tudo em maiusculo
	for(i=0; i<tamanhoArquivo; i++){
		if(programa[i]>='a' && programa[i]<='z'){
			programa[i]+= 'A' - 'a';
		}
	}


	criaListaDeTokens(programa, tamanhoArquivo);
	preProcessa(programa);
	monta(programaPreProcessado);
	
	return 0;
}


void preProcessa(char *a){

}

void monta(char *a){

}

