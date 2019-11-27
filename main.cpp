#include <stdio.h>
#include <string.h>
#include <list>
#include "token.h"
#include "preprocessador.h"
#include "montador.h"

#define TAMANHO_MAXIMO_ARQUIVO 1024

int numeroDeArquivosFornecidos = 0;

void adicionaExtensao(char *nome, char *extensao);
int montaArquivo(char *nomeDoArquivo);

int main(int argc, char *argv[]){

	if (argc == 2){
		numeroDeArquivosFornecidos = 1;
	}
	else if (argc == 3){
		numeroDeArquivosFornecidos = 2;
	}
	else{
		printf("uso incorreto.\n");
		return 0;
	}

	montaArquivo(argv[1]);
	if (numeroDeArquivosFornecidos == 2){
		montaArquivo(argv[2]);
	}

	return 0;
}

int montaArquivo(char *nomeDoArquivo){
	
	int i, tamanhoExecutavel;
	char nomeBuffer[100];
	char programa[TAMANHO_MAXIMO_ARQUIVO];
	int	 executavel[TAMANHO_MAXIMO_ARQUIVO];
	char tabelaDeUso[2048], tabelaDeDefinicoes[2048];
	int bitmapRealocacao[TAMANHO_BITMAP_REALOCACAO];

	int tamanhoArquivo = 0;
	FILE *file = NULL;

	file = fopen(nomeDoArquivo,"r");
	if (file == NULL){
		printf("arquivo %s não encontrado\n", nomeDoArquivo);
		return 0;
	}

	//copia texto para 'programa'
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
	//salva arquivo.pre
	strcpy(nomeBuffer, nomeDoArquivo);
	adicionaExtensao(nomeBuffer,".pre");
	arquivoPreProcessado = fopen(nomeBuffer,"w");
	fprintf(arquivoPreProcessado,"%s",programa);
	//restrutura na forma SECTION TEXT seguida de SECTION DATA
	reestruturaSections(programa);
	//monta
	tamanhoExecutavel = monta(programa, executavel,tabelaDeUso, tabelaDeDefinicoes, bitmapRealocacao);
	//salva arquivo.obj
	strcpy(nomeBuffer, nomeDoArquivo);
	adicionaExtensao(nomeBuffer,".obj");
	arquivoExecutavel = fopen(nomeBuffer,"w");
	//imprime nome do arquivo
	strcpy(nomeBuffer, nomeDoArquivo);
	adicionaExtensao(nomeBuffer,"");
	fprintf(arquivoExecutavel,"%s\n",nomeBuffer);
	//imprime tamanho do executavel
	fprintf(arquivoExecutavel,"%d\n",tamanhoExecutavel);
	//imprime bitmap de realocacao;
	for(i=0; i<tamanhoExecutavel;i++){
		fprintf(arquivoExecutavel,"%d",bitmapRealocacao[i]);
	}
	//imprime tabela de uso
	fprintf(arquivoExecutavel,"\n%s\n",tabelaDeUso);
	//imprime executavel;
	for(i = 0; i<tamanhoExecutavel; i++){
		fprintf(arquivoExecutavel,"%d ",executavel[i]);
	}
	//imprime tabela de definicoes
	fprintf(arquivoExecutavel,"\n%s",tabelaDeDefinicoes);

	return 1;
}

void adicionaExtensao(char *nome, char *extensao){
	int i;
	for(i = strlen(nome); nome[i] != '.' && i>0; i--);	
	if(i==0) i=strlen(nome);
	strcpy(nome+i, extensao);
}
