#include <string.h>
#include <stdio.h>
#include "token.h"

#define TAMANHO_MAX_ARQUIVO_OBJETO 2048
void salvaArquivo(char *nomeDoAqruivo,char * string);
void adicionaExtensao(char *nome, char *extensao);
void ligaUmArquivo(char* nomeArquivo);
void ligaDoisArquivos(char* nomeArquivo1,char* nomeArquivo2);
int copiaArquivoParaString(char* outPut,char *nomeDoArquivo);

int main(int argc, char *argv[]){

	if (argc == 2){
		ligaUmArquivo(argv[1]);
	}
	else if (argc == 3){
		ligaDoisArquivos(argv[1], argv[2]);;
	}
	else{
		printf("uso incorreto.\n");
		return 0;
	}

	return 0;
}

typedef struct Simbolo{
	//char string[100];		//texto do simbolo
	Token token;
	int valor;
} TabelaGlobal;

void ligaDoisArquivos(char *nomeArquivo1, char *nomeArquivo2){
	int i, tamanhoStrPrograma1;
	char strArquivoObjeto[TAMANHO_MAX_ARQUIVO_OBJETO], ArquivoDeSaida[TAMANHO_MAX_ARQUIVO_OBJETO], strBuffer[256];
	int tamanhoArquivo; 
	int tamanhoPrograma1;
	char *nomePrograma, *strTamanhoDoArquivo, *strBitmapRealocacao, *strTabelaDeDefinicao, *strPrograma, *strTabelaDeUso;
	char strTabelaDeUso1[2048];
	TabelaGlobal tabelaGlobal[2048];
	int tamanhoTabelaGlobal=0;
	Token token;
	int posicao=0;
	//abre arquivo
	tamanhoArquivo = copiaArquivoParaString(strArquivoObjeto,nomeArquivo1);
	//descobre posicao de cada um dos campos
	nomePrograma = strArquivoObjeto;
	for (i=0;strArquivoObjeto[i]!='\n'; i++);	//i retorna endereco do prox \n
	strArquivoObjeto[i] = '\0';
	strTamanhoDoArquivo = strArquivoObjeto+(++i);
	printf("%d",i);
	for (;strArquivoObjeto[i]!='\n'; i++);	//i retorna endereco do prox \n
	strArquivoObjeto[i] = '\0';
	strBitmapRealocacao = strArquivoObjeto+(++i);
	for (;strArquivoObjeto[i]!='\n'; i++);	//i retorna endereco do prox \n
	strArquivoObjeto[i] = '\0';
	strTabelaDeDefinicao = strArquivoObjeto+(++i);
	for (;strArquivoObjeto[i]!='\n'; i++);	//i retorna endereco do prox \n
	strArquivoObjeto[i] = '\0';
	strPrograma = strArquivoObjeto+(++i);	
	for (;strArquivoObjeto[i]!='\n' && strArquivoObjeto[i]!='\0' ; i++);	//i retorna endereco do prox \n
	strArquivoObjeto[i] = '\0';
	strTabelaDeUso = strArquivoObjeto+(++i);

	strcpy(ArquivoDeSaida,strPrograma);
	tamanhoStrPrograma1 = strlen(strPrograma);

	strcpy(strTabelaDeUso1,strTabelaDeUso);	//copia para um lugar permanente, pq strTabelaDeUso vai ser usado pelo prox programa
	tamanhoPrograma1 = atoi(strTamanhoDoArquivo);

	printf("\n\n%s\n\n",ArquivoDeSaida);

	//printf("\n%d---\n",tamanhoPrograma1);

	//adiciona definicoes na tabela global de definicoes
	posicao = 0;
	while(true){
		posicao += token.leUmToken(strTabelaDeDefinicao,posicao);
		token.copiaTokenParaString(strBuffer);
		//printf("token: %s tipo: %d",strBuffer,token.tipo);
		//while (token.tipo == TABULACAO ||token.tipo == ESPACO) posicao += token.leUmToken(strTabelaDeDefinicao,posicao);
		//if (token.tipo == FIM_DE_STR) break;
		while (token.tipo == ESPACO) posicao += token.leUmToken(strTabelaDeDefinicao,posicao);
		if (token.tipo != PALAVRA) break;
		//guarda token
		tabelaGlobal[tamanhoTabelaGlobal].token = token;
		posicao++; //salta espaco
		posicao += token.leUmToken(strTabelaDeDefinicao,posicao);	//le numero
		tabelaGlobal[tamanhoTabelaGlobal].valor = token.leNumero();	//atribui valor a tabela
		tamanhoTabelaGlobal++;
	}

	//abre segundo arquivo e faz a mesma coisa
	//abre arquivo
	tamanhoArquivo = copiaArquivoParaString(strArquivoObjeto,nomeArquivo2);
	//descobre posicao de cada um dos campos
	nomePrograma = strArquivoObjeto;
	for (i=0;strArquivoObjeto[i]!='\n'; i++);	//i retorna endereco do prox \n
	strArquivoObjeto[i] = '\0';
	strTamanhoDoArquivo = strArquivoObjeto+(++i);
	for (;strArquivoObjeto[i]!='\n'; i++);	//i retorna endereco do prox \n
	strArquivoObjeto[i] = '\0';
	strBitmapRealocacao = strArquivoObjeto+(++i);
	for (;strArquivoObjeto[i]!='\n'; i++);	//i retorna endereco do prox \n
	strArquivoObjeto[i] = '\0';
	strTabelaDeDefinicao = strArquivoObjeto+(++i);
	for (;strArquivoObjeto[i]!='\n'; i++);	//i retorna endereco do prox \n
	strArquivoObjeto[i] = '\0';
	strPrograma = strArquivoObjeto+(++i);
	for (;strArquivoObjeto[i]!='\n' && strArquivoObjeto[i]!='\0' ; i++);	//i retorna endereco do prox \n
	strArquivoObjeto[i] = '\0';
	strTabelaDeUso = strArquivoObjeto+(++i);
	
	strcpy(ArquivoDeSaida+tamanhoStrPrograma1,strPrograma);

	//adiciona definicoes na tabela global de definicoes
	posicao = 0;
	while(true){
		posicao += token.leUmToken(strTabelaDeDefinicao,posicao);
		token.copiaTokenParaString(strBuffer);
		//printf("token: %s tipo: %d",strBuffer,token.tipo);
		//while (token.tipo == TABULACAO ||token.tipo == ESPACO) posicao += token.leUmToken(strTabelaDeDefinicao,posicao);
		//if (token.tipo == FIM_DE_STR) break;
		while (token.tipo == ESPACO) posicao += token.leUmToken(strTabelaDeDefinicao,posicao);
		if (token.tipo != PALAVRA) break;
		//guarda token
		tabelaGlobal[tamanhoTabelaGlobal].token = token;
		posicao++; //salta espaco
		posicao += token.leUmToken(strTabelaDeDefinicao,posicao);	//le numero
		tabelaGlobal[tamanhoTabelaGlobal].valor = token.leNumero() + tamanhoPrograma1;	//atribui valor CORRIGIDO a tabela
		tamanhoTabelaGlobal++;
		//printf("#\n");
	}

	salvaArquivo(nomePrograma,ArquivoDeSaida);

	printf("TABELA GLOBAL:\n");
	for (i=0; i<tamanhoTabelaGlobal;i++){
		tabelaGlobal[i].token.copiaTokenParaString(strBuffer);
		printf("%s %d\n",strBuffer, tabelaGlobal[i].valor);
	}
}

void ligaUmArquivo(char* nomeArquivo){
	int i;
	char strArquivoObjeto[TAMANHO_MAX_ARQUIVO_OBJETO];
	int tamanhoArquivo;
	char *nomePrograma, *strTamanhoDoArquivo, *strBitmapRealocacao, *strTabelaDeDefinicao, *strPrograma, *strTabelaDeUso;
	//abre arquivo
	tamanhoArquivo = copiaArquivoParaString(strArquivoObjeto,nomeArquivo);
	//descobre posicao de cada um dos campos
	nomePrograma = strArquivoObjeto;
	for (i=0;strArquivoObjeto[i]!='\n'; i++);	//i retorna endereco do prox \n
	strArquivoObjeto[i] = '\0';
	strTamanhoDoArquivo = strArquivoObjeto+(++i);
	for (i=0;strArquivoObjeto[i]!='\n'; i++);	//i retorna endereco do prox \n
	strArquivoObjeto[i] = '\0';
	strBitmapRealocacao = strArquivoObjeto+(++i);
	for (i=0;strArquivoObjeto[i]!='\n'; i++);	//i retorna endereco do prox \n
	strArquivoObjeto[i] = '\0';
	strTabelaDeDefinicao = strArquivoObjeto+(++i);
	for (i=0;strArquivoObjeto[i]!='\n'; i++);	//i retorna endereco do prox \n
	strArquivoObjeto[i] = '\0';
	strPrograma = strArquivoObjeto+(++i);
	for (i=0;strArquivoObjeto[i]!='\n'; i++);	//i retorna endereco do prox \n
	strArquivoObjeto[i] = '\0';
	strTabelaDeUso = strArquivoObjeto+(++i);

	salvaArquivo(nomePrograma,strPrograma);
	
}

void salvaArquivo(char *nomeDoArquivo,char * string){
	char nomeBuffer[256];
	FILE *arquivo;
	strcpy(nomeBuffer, nomeDoArquivo);
	adicionaExtensao(nomeBuffer,".o");
	arquivo = fopen(nomeBuffer,"w");
	fprintf(arquivo,"%s",string);
	fclose(arquivo);
}

int copiaArquivoParaString(char* outPut,char *nomeDoArquivo){
	int tamanhoArquivo = 0;
	
	FILE *file = NULL;

	file = fopen(nomeDoArquivo,"r");
	if (file == NULL){
		printf("arquivo %s não encontrado\n", nomeDoArquivo);
		return 0;
	}

	while (!feof(file) && tamanhoArquivo < TAMANHO_MAX_ARQUIVO_OBJETO){
		fread(outPut+tamanhoArquivo, sizeof(char), 1, file); 
		tamanhoArquivo++;		
	}
	outPut[--tamanhoArquivo] = 0;

	fclose(file);

	return tamanhoArquivo;
}

void adicionaExtensao(char *nome, char *extensao){
	int i;
	for(i = strlen(nome); nome[i] != '.' && i>0; i--);	
	if(i==0) i=strlen(nome);
	strcpy(nome+i, extensao);
}
