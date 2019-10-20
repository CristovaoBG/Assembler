#include <stdio.h>
#include <string.h>
#include "token.h"

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

int apagaLinha(char *str){	//retorna tamanho da linha apagada
	int tamanhoL = 0;
	while (str[tamanhoL] != '\n' && str[tamanhoL] != '\0' ) tamanhoL++;
	tamanhoL++;
	removeCaracteres(str,tamanhoL);
	return tamanhoL;
}

void substituiEqu(char *string, char *rotuloStr, char *valorEquStr){ //substitui o valor de rotuloStr por valorEquStr na string

	Token token;
	int posicao = 0, posAnterior;
	char tokenLido[100];
	int tamanhoDoRotulo = strlen(rotuloStr);

	while(string[posicao]!='\0'){
		posAnterior = posicao;
		posicao += token.leUmToken(string, posicao);
		token.copiaTokenParaString(tokenLido);
		if (strcmp(tokenLido,rotuloStr)==0){	//significa que sao iguais, enta substitui
			removeCaracteres(string + posAnterior, tamanhoDoRotulo);
			insereStringNaPosicao(string + posAnterior, valorEquStr);
			posicao = posAnterior;
		}
	}
}

void preProcessa(char *string, int tamanho){
	//printf(string);
//	char *token, tokenString[50];
//	list<Token> tokenList;
//	Token token;
	int posicao = 0, posicaoAuxiliar = 0, i;

	//ListaToken listaDeEqus;
	Token token, rotulo, valorEqu;
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
	

	//bota tudo em maiusculo
	for(i=0; string[i] != '\0'; i++){
		if(string[i]>='a' && string[i]<='z'){
			string[i]+= 'A' - 'a';
		}
	}

	//remove comentarios
	for(i=0; string[i] != '\0'; i++){
		if (string[i] == ';'){
			string[i] = '\n';
			apagaLinha(string+i+1);
		}
	}

	//remove enters tabs e espacos desnecessarios
	i=0;
	while(string[i] == '\n' || string[i] == '\t' || string[i] == ' '){
		removeCaracteres(string,1);	//remove \n a frente
	}
	for(i=0; i<tamanho; i++){
		if (string[i] == '\n'){
			while(string[i+1] == '\n' || string[i+1] == '\t' || string[i+1] == ' '){
				removeCaracteres(string+i+1,1);	//remove \n a frente
			}
		}
		else if(string[i] == '\t'){
			if(string[i+1] == '\n'){
				removeCaracteres(string+i,1);	//remove \t
				i--;
			}
			else while(string[i+1] == ' ' || string[i+1] == '\t'){
				removeCaracteres(string+i+1,1);	//remove caractere à frente
			}
		}
		else if(string[i] == ' '){
			if(string[i+1] == '\n'){
				removeCaracteres(string+i,1);	//remove ' '
				i--;
			}
			else while(string[i+1] == ' ' || string[i+1] == '\t'){
				removeCaracteres(string+i+1,1);	//remove caractere à frente
			}
		}
	}


	printf("\n\n%s\n",string);
	
	//Processa os EQUs
	while(string[posicao]!='\0'){
		posicaoAuxiliar = posicao;	//grava posicao antes de verificar se eh equ
		posicao += token.leUmToken(string, posicao);
		//remove espacos desnecessarios.
		while(token.tipo == QUEBRA_DE_LINHA || token.tipo == ESPACO || token.tipo == TABULACAO) {
			posicaoAuxiliar = posicao;
			posicao += token.leUmToken(string,posicao);	
			}
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
						printf("estrutura de EQU encontrada. Rotulo: %s Valor: %s\n",rotuloStr,valorEquStr);		
						//varre o texto substituindo os caracteres da string à frente
						substituiEqu(string + posicao,rotuloStr,valorEquStr);
						apagaLinha(string+posicaoAuxiliar);
						posicao = posicaoAuxiliar;
					}
				}
			}
		}
	}
	//processa IFs
	posicao = 0;
	while(string[posicao]!='\0'){
		posicaoAuxiliar = posicao;	//inicio do comando
		posicao += token.leUmToken(string, posicao);
		//tira espacos desnecessarios
		while(token.tipo == QUEBRA_DE_LINHA || token.tipo == ESPACO || token.tipo == TABULACAO) {
			posicaoAuxiliar = posicao;
			posicao += token.leUmToken(string,posicao);
			}
		if (token.tipo == IF){
			posicao += token.leUmToken(string,posicao);
			while(token.tipo == QUEBRA_DE_LINHA || token.tipo == ESPACO || token.tipo == TABULACAO){
				posicao += token.leUmToken(string,posicao);
			}
			if (token.tipo == NUMERO){
				//estrutura IF usada corretamente
				char ifValor[100];
				token.copiaTokenParaString(ifValor);
				//printf("tamanho: %d, valor = %d", strlen(ifValor),ifValor[0]);
				if(strlen(ifValor)== 1 && ifValor[0] == '1'){ // se eh verdadeiro apaga so a linha do if
					apagaLinha(string+posicaoAuxiliar);
					posicao = posicaoAuxiliar;
				}else{ //apaga linha atual e a seguinte
					apagaLinha(string+posicaoAuxiliar);
					posicao = posicaoAuxiliar;
					apagaLinha(string+posicao);
				}
			}
		}
	}
	printf("\nsegue:\n%s\n",string);

}

