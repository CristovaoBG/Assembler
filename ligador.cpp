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

int leValorDeTokenNaTabela(Token token, TabelaGlobal *tabela, int tamanhoTabela){
	int i, output = -1;
	for (i=0;i<tamanhoTabela; i++){
		if (token.comparaToken(tabela[i].token)){
			output = tabela[i].valor;
			break;
		}
	}
	return output;
}

void ligaDoisArquivos(char *nomeArquivo1, char *nomeArquivo2){
	TabelaGlobal tabelaGlobal[2048];
	int i, tamanhoStrPrograma1, arquivoDeSaida[2048], tamanhoArquivoFinal=0;
	char strArquivoObjeto1[TAMANHO_MAX_ARQUIVO_OBJETO],strArquivoObjeto2[TAMANHO_MAX_ARQUIVO_OBJETO], strArquivoDeSaida[TAMANHO_MAX_ARQUIVO_OBJETO], strBuffer[256];
	int tamanhoArquivo; 
	int posicaoAuxiliar;
	int tamanhoPrograma1;
	char *nomePrograma, *strTamanhoDoArquivo, *strBitmapRealocacao, *strTabelaDeDefinicao, *strPrograma, *strTabelaDeUso;
	char strTabelaDeUso1[2048];
	int tamanhoTabelaGlobal=0;
	Token token, tokenUsado;
	int posicao=0;
	//abre arquivo
	tamanhoArquivo = copiaArquivoParaString(strArquivoObjeto1,nomeArquivo1);
	//descobre posicao de cada um dos campos
	nomePrograma = strArquivoObjeto1;
	for (i=0;strArquivoObjeto1[i]!='\n'; i++);	//i retorna endereco do prox \n
	strArquivoObjeto1[i] = '\0';
	strTamanhoDoArquivo = strArquivoObjeto1+(++i);
	printf("%d",i);
	for (;strArquivoObjeto1[i]!='\n'; i++);	//i retorna endereco do prox \n
	strArquivoObjeto1[i] = '\0';
	strBitmapRealocacao = strArquivoObjeto1+(++i);
	for (;strArquivoObjeto1[i]!='\n'; i++);	//i retorna endereco do prox \n
	strArquivoObjeto1[i] = '\0';
	strTabelaDeDefinicao = strArquivoObjeto1+(++i);
	for (;strArquivoObjeto1[i]!='\n'; i++);	//i retorna endereco do prox \n
	strArquivoObjeto1[i] = '\0';
	strPrograma = strArquivoObjeto1+(++i);	
	for (;strArquivoObjeto1[i]!='\n' && strArquivoObjeto1[i]!='\0' ; i++);	//i retorna endereco do prox \n
	strArquivoObjeto1[i] = '\0';
	strTabelaDeUso = strArquivoObjeto1+(++i);

	strcpy(strArquivoDeSaida,strPrograma);
	tamanhoStrPrograma1 = strlen(strPrograma);

	strcpy(strTabelaDeUso1,strTabelaDeUso);	//copia para um lugar permanente, pq strTabelaDeUso vai ser usado pelo prox programa
	tamanhoPrograma1 = atoi(strTamanhoDoArquivo);

	printf("\n\n%s\n\n",strArquivoDeSaida);

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
		token.copiaTokenParaString(strBuffer);
		printf("tipo do token: %d, token: %s",token.tipo,strBuffer);
		if (token.tipo != PALAVRA) break;
		//guarda token
		tabelaGlobal[tamanhoTabelaGlobal].token = token;
		posicao++; //salta espaco
		posicao += token.leUmToken(strTabelaDeDefinicao,posicao);	//le numero
		//token.copiaTokenParaString(strBuffer);
		//printf("--%s--",strBuffer);
		if (token.tipo != NUMERO) printf("ERRO NA TABELA\n");
		tabelaGlobal[tamanhoTabelaGlobal].valor = token.leNumero();	//atribui valor a tabela
		tamanhoTabelaGlobal++;
		printf("#\n");
	}

	//abre segundo arquivo e faz a mesma coisa
	//abre arquivo
	tamanhoArquivo = copiaArquivoParaString(strArquivoObjeto2,nomeArquivo2);

	//descobre posicao de cada um dos campos
	nomePrograma = strArquivoObjeto2;
	for (i=0;strArquivoObjeto2[i]!='\n'; i++);	//i retorna endereco do prox \n
	strArquivoObjeto2[i] = '\0';
	strTamanhoDoArquivo = strArquivoObjeto2+(++i);
	for (;strArquivoObjeto2[i]!='\n'; i++);	//i retorna endereco do prox \n
	strArquivoObjeto2[i] = '\0';
	strBitmapRealocacao = strArquivoObjeto2+(++i);
	for (;strArquivoObjeto2[i]!='\n'; i++);	//i retorna endereco do prox \n
	strArquivoObjeto2[i] = '\0';
	strTabelaDeDefinicao = strArquivoObjeto2+(++i);
	for (;strArquivoObjeto2[i]!='\n'; i++);	//i retorna endereco do prox \n
	strArquivoObjeto2[i] = '\0';
	strPrograma = strArquivoObjeto2+(++i);
	for (;strArquivoObjeto2[i]!='\n' && strArquivoObjeto2[i]!='\0' ; i++);	//i retorna endereco do prox \n
	strArquivoObjeto2[i] = '\0';
	strTabelaDeUso = strArquivoObjeto2+(++i);
	
	strcpy(strArquivoDeSaida+tamanhoStrPrograma1,strPrograma);

	//adiciona definicoes na tabela global de definicoes
	posicao = 0;
	while(true){
		posicao += token.leUmToken(strTabelaDeDefinicao,posicao);
		//token.copiaTokenParaString(strBuffer);
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
	}

	//converte executavel de saida para vetor de int
	tamanhoArquivoFinal=0 ;	// i eh o cursor do arquivoDeSaida
	posicao = 0;
	while(true){
		posicao += token.leUmToken(strArquivoDeSaida, posicao);	//le numero
		while (token.tipo == ESPACO) posicao += token.leUmToken(strArquivoDeSaida,posicao); //se for espaco le prox
		if (token.tipo != NUMERO) break;
		arquivoDeSaida[tamanhoArquivoFinal] = token.leNumero();
		tamanhoArquivoFinal++;
	}

	//le tabelas de uso do primeiro arquivo e aplica no executabel usando a tabela de definicao global
	posicao = 0;
	while(true){
		posicao += token.leUmToken(strTabelaDeUso1, posicao);	//le palavra
		while (token.tipo == ESPACO) posicao += token.leUmToken(strTabelaDeUso1,posicao); //se for espaco le prox
		if (token.tipo != PALAVRA) break;	//se nao for palavra lista acabou
		tokenUsado = token; //token usado eh o label a ser substituido
		// le lista de numeros e aplica ate que encontre proxima palavra ou a lista acabe
		while(true){
			posicao++; //ignora espaco
			posicao += token.leUmToken(strTabelaDeUso1,posicao);
			if (token.tipo != NUMERO) break;
			// substitui no arquivo executavel de saida na posicao lida o verdadeiro valor dado pela lista global de definicao
			arquivoDeSaida[token.leNumero()] = leValorDeTokenNaTabela(tokenUsado, tabelaGlobal, tamanhoTabelaGlobal) + arquivoDeSaida[token.leNumero()];
		}
	}	

	//le bitmap e aplica fator de correcao
	for (i=0;strBitmapRealocacao[i]=='0' || strBitmapRealocacao[i]=='1';i++){
		if (strBitmapRealocacao[i] == '1') arquivoDeSaida[i+tamanhoPrograma1] += tamanhoPrograma1;
	}

	//le tabelas de uso do sgundo arquivo e aplica no executabel usando a tabela de definicao global
	posicao = 0;
	printf("---%s---",strTabelaDeUso);	
	while(true){
		posicao += token.leUmToken(strTabelaDeUso, posicao);	//le palavra
		while (token.tipo == ESPACO) posicao += token.leUmToken(strTabelaDeUso,posicao); //se for espaco le prox
		if (token.tipo != PALAVRA) break;	//se nao for palavra lista acabou
		tokenUsado = token; //token usado eh o label a ser substituido
		// le lista de numeros e aplica ate que encontre proxima palavra ou a lista acabe
		while(true){
			posicao++; //ignora espaco
			posicaoAuxiliar = posicao;
			posicao += token.leUmToken(strTabelaDeUso,posicao);
			token.copiaTokenParaString(strBuffer);
			//printf("\n*token lido: %s tipo: %d\n",strBuffer,token.tipo);
			if (token.tipo != NUMERO) {
				posicao = posicaoAuxiliar;
				break;
				}
			//substitui no arquivo executavel de saida na posicao lida o verdadeiro valor dado pela lista global de definicao
			arquivoDeSaida[token.leNumero()+tamanhoPrograma1] -= tamanhoPrograma1;	//desfaz correcao feita acima, pq nao era pra ter feito nesse caso
			arquivoDeSaida[token.leNumero()+tamanhoPrograma1] = leValorDeTokenNaTabela(tokenUsado, tabelaGlobal, tamanhoTabelaGlobal) + arquivoDeSaida[token.leNumero()+tamanhoPrograma1];
			tokenUsado.copiaTokenParaString(strBuffer);
			printf("\nvalor do token: %d token:%s\nposicao a ser escrita:%d\n", leValorDeTokenNaTabela(tokenUsado, tabelaGlobal, tamanhoTabelaGlobal),strBuffer,token.leNumero()+tamanhoPrograma1);
		}
	}	

	printf("%d\n%s\n",tamanhoPrograma1,	strArquivoDeSaida);

	//imprime arquivo de saida
	for(i=0; i<tamanhoArquivoFinal; i++) printf("%d ", arquivoDeSaida[i]);
	printf("\n");

	//imprime executavel;
	int j = 0;
	for(i = 0; i<tamanhoArquivoFinal; i++){
		sprintf(strBuffer,"%d", arquivoDeSaida[i]);
		sprintf(strArquivoDeSaida+j,"%s ",strBuffer);
		j=strlen(strArquivoDeSaida);
	}

	//deprecated
	salvaArquivo(nomeArquivo1,strArquivoDeSaida);

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
