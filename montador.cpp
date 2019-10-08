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
	Token token, operando, label;
	char buffer[100];
	int posicao = 0, posicaoAuxiliar, valorToken, i;
	int cursorExecutavel = 0;
	int programa[TAMANHO_MAXIMO_ARQUIVO];
	bool naTabela = false;


	Simbolo tabelaDeSimbolos[TAMANHO_MAX_TABELA_DE_SIMBOLOS];
	int tamanhoTabela = 0;

	printf("AAAAAAAAAAA   %d    AAAAAAAAAA\n",(int)texto[58]);
	for (i=55;i<60;i++)printf("%d ",(int)texto[i]);
	printf("\n\n");

	while(texto[posicao]!='\0'){
		posicaoAuxiliar = posicao;	//grava posicao
		posicao += token.leUmToken(texto, posicao);
		//remove espacos desnecessarios.
		while(token.tipo == QUEBRA_DE_LINHA || token.tipo == ESPACO || token.tipo == TABULACAO) {
			posicaoAuxiliar = posicao;
			posicao += token.leUmToken(texto,posicao);
		}
		if (token.tipo == PALAVRA){	//nesse caso acho que so pode ser declaracao de label
			label = token;
			posicao += token.leUmToken(texto, posicao);
			while(token.tipo == QUEBRA_DE_LINHA || token.tipo == ESPACO || token.tipo == TABULACAO) {
				posicaoAuxiliar = posicao;
				posicao += token.leUmToken(texto,posicao);
			}
			if(token.tipo == DOIS_PONTOS){
				//label declarada corretamente.
				//vai na tabela de simbolos e procura, se nao tiver cria e define.
				//se tiver entao define e varre a lista preenchendo o seu valor
				//se tiver na lista e tiver definida é erro
				
			}
		}
		if (token.tipo >= ADD && token.tipo <= OUTPUT && token.tipo != COPY){	// sao todas as instruncoes com um operando
			operando = token;
			posicao += token.leUmToken(texto,posicao);
			while(token.tipo == ESPACO || token.tipo == TABULACAO) {	//ignora tabs e espacos entre instrucao e operando
				posicao += token.leUmToken(texto,posicao);
			}
			if (token.tipo == NUMERO){
				//insere instrucao e operando no programa executavel.
				programa[cursorExecutavel] = operando.tipo;	//o tipo eh o proprio codigo pq eu fui esperto ou eu fui burro nao sei ainda
				cursorExecutavel++;
				token.copiaTokenParaString(buffer);
				programa[cursorExecutavel] = atoi(buffer);	//atoi eh de uma biblioteca incluida, serve pra converter de string pra int
				cursorExecutavel++;				
				//prigrama[cursorExecutavel] = 
			}else if (token.tipo == PALAVRA){
				printf("eh palavra ok\n");
				//verifica se simbolo existe na tabela de simbolo,
				//se existir e tiver definida insere o valor no executavel
				//se existir e nao tiver definida insere valor da posicao do programa atual na tabela de simbolos
				// e insere o valor anterior na tabela de simbolos
				//se nao existir, insere na tabela de simbolo com -1
				token.copiaTokenParaString(buffer);
				//verifica se esta na tabela
				naTabela = false;
				for(i=0; i<tamanhoTabela;i++){
					if(comparaTokens(token,tabelaDeSimbolos[i].token)){
						naTabela = true;
						break;
					}
				}
				if(naTabela){
					if (tabelaDeSimbolos[i].isDef){
						programa[cursorExecutavel] = operando.tipo;	//o tipo eh o proprio codigo pq eu fui esperto ou eu fui burro nao sei ainda
						cursorExecutavel++;
						programa[cursorExecutavel] = tabelaDeSimbolos[i].valor; // insere valor do label no executavel
						cursorExecutavel++;	
					}
					else{	//se simbolo esta na tabela mas nao esta definido 
						programa[cursorExecutavel] = operando.tipo;	//o tipo eh o proprio codigo pq eu fui esperto ou eu fui burro nao sei ainda
						cursorExecutavel++;
						// insere na lista
						programa[cursorExecutavel] = tabelaDeSimbolos[i].lista;
						tabelaDeSimbolos[i].lista = cursorExecutavel; // insere valor do label no executavel
						cursorExecutavel++;	
					}
				}
				else{	//se nao esta na tabela de simbolos
					//insere na tabela de simbolos
					programa[cursorExecutavel] = operando.tipo;
					cursorExecutavel++;
					tabelaDeSimbolos[tamanhoTabela].token = token;
					tabelaDeSimbolos[tamanhoTabela].isDef = false;
					tabelaDeSimbolos[tamanhoTabela].lista = cursorExecutavel;	//indica que eh o primeiro elemento da lista
					programa[cursorExecutavel] = -1;
					cursorExecutavel++;
					tamanhoTabela++;
				}
			}
		}
		if (token.tipo == STOP){
			programa[cursorExecutavel] = token.tipo;
			cursorExecutavel++;
			printf("!!!!!!!!!  %d  !!!!!!!!!!!!!!\n\n", token.tipo);
		}
	}

	printf("\n\nTABELA DE SIMBOLOS:\n");
	for(i=0;i<tamanhoTabela;i++){
		tabelaDeSimbolos[i].token.copiaTokenParaString(buffer);
		printf("token: %s isDef: %d lista: %d\n",buffer,tabelaDeSimbolos[i].isDef,tabelaDeSimbolos[i].lista);
	}
	printf("\n\n"); 
	for(int j=0;j<cursorExecutavel; j++) printf("%d ",programa[j]);
	printf("\n");
}













