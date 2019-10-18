#include "token.h"
#include "montador.h"
#include <string.h>

#define NUM_TOKENS 31

struct Lista{
	Token *token;
	struct Lista *prox;
};

bool comparaDoisTokens(char * t1, char * t2, int tamanho){ //verdadeiro se sao iguais, falso se nao
	int i;
	for (i=0; i < tamanho; i++){
		if (t1[i]!=t2[i]) {
			return false;
		}
//			printf("caractere 1:%c  caractere 2:%c\n",t1[i],t2[i]);
	}
	return true;
}

bool Token::comparaToken(Token token){
	char buffer1[100], buffer2[100];
	copiaTokenParaString(buffer1);
	token.copiaTokenParaString(buffer2);
	//printf(" %d |",!strcmp(buffer1,buffer2));
	return !strcmp(buffer1,buffer2);
	
}

Token::Token(){
	contaLinha = 1;
}

int Token::leLinhaAtual(){
	return contaLinha;
}

void Token::atribuiContaLinha(int valor){
	contaLinha = valor;
}


void Token::copiaTokenParaString(char *destino){
	int i;
	for(i = 0; i<tamanho; i++){
		destino[i] = posicaoAbsoluta[i];
	}
	destino[i] = '\0';
}

bool comparaTokens(Token t1, Token t2){
	char str1[100],str2[100];
	t1.copiaTokenParaString(str1);
	t2.copiaTokenParaString(str2);
	if (strcmp(str1,str2)==0){
		return 1;
	} else{
		return 0;
	}
}

int descobreToken(char *inicioString, int tamanho){
	int i, j;
	//sao 19 tokens textuais conhecidos, o maior deles apresenta 7 caracteres + FIM_STR = 8 caractreres
	char tokenStrings[NUM_TOKENS][8];
	//limpa lixo de tokenStrings;
	for (i = 0; i< 8; i++) for (j=0; j<NUM_TOKENS; j++)tokenStrings[j][i] = '\0';
	
	//define tabela de strings de tokens
	strcpy(&tokenStrings[ADD][0],"ADD");
	strcpy(&tokenStrings[SUB][0],"SUB");
	strcpy(&tokenStrings[MULT][0],"MULT");
	strcpy(&tokenStrings[DIV][0],"DIV");
	strcpy(&tokenStrings[JMP][0],"JMP");
	strcpy(&tokenStrings[JMPN][0],"JMPN");
	strcpy(&tokenStrings[JMPP][0],"JMPP");
	strcpy(&tokenStrings[JMPZ][0],"JMPZ");
	strcpy(&tokenStrings[COPY][0],"COPY");
	strcpy(&tokenStrings[LOAD][0],"LOAD");
	strcpy(&tokenStrings[STORE][0],"STORE");
	strcpy(&tokenStrings[INPUT][0],"INPUT");
	strcpy(&tokenStrings[OUTPUT][0],"OUTPUT");
	strcpy(&tokenStrings[STOP][0],"STOP");
	strcpy(&tokenStrings[SECTION][0],"SECTION");
	strcpy(&tokenStrings[SPACE][0],"SPACE");
	strcpy(&tokenStrings[CONST][0],"CONST");
	strcpy(&tokenStrings[EQU][0],"EQU");
	strcpy(&tokenStrings[IF][0],"IF");
	strcpy(&tokenStrings[DATA][0],"DATA");
	strcpy(&tokenStrings[TEXT][0],"TEXT");

	if (tamanho > 8) return PALAVRA;	// se for maior que oito entao so pode ser uma palavra
	
//	printf("_____________%d__________\n",tamanho);
	for (i=0; i<NUM_TOKENS; i++){
		if (strlen(&tokenStrings[i][0]) == tamanho) {
			if(comparaDoisTokens(inicioString, &tokenStrings[i][0], tamanho) == true) return i;	//retorna o índice do token
		}
	}
	return PALAVRA;	 // se nenhum token foi encontrado
}

int Token::leUmToken(char *stringInput, int inicio){
	posicao = inicio;
	char c = stringInput[posicao];
	tamanho = 0;
	if (c>='A' && c<='Z' || c == '_'){	
		//tipo texto -> podem suceder numeros
		//tamanho++;
		//c = stringInput[posicao];
		while ((c>='A' && c<='Z' )||(c>='0' && c<='9' )|| c == '_'){
			c = stringInput[posicao+(++tamanho)];
		}
		tipo = descobreToken(stringInput+inicio, tamanho);
	}
	else if (c>='0' && c<='9'){
		while (c>='0' && c<='9' ){	//varre os proximos digitos pra saber tamanho
			c = stringInput[posicao+(++tamanho)];
		}
		//c = stringInput[posicao+(++tamanho)];
		tipo = NUMERO;		
		//le numero e salva em valor
	}
	else {
		switch(c){
			case ' ':
				tipo = ESPACO;
				tamanho++;
				break;
			case '\n':
				tipo = QUEBRA_DE_LINHA;
				contaLinha++;
				tamanho++;
				//printf("POSICAO: %d \n", dizLinhaOriginal(posicao+tamanho));
				
				break;
			case ':':
				tipo = DOIS_PONTOS;
				tamanho++;
				break;
			case '\t':
				tipo = TABULACAO;
				tamanho++;
				break;
			case ',':
				tipo = VIRGULA;
				tamanho++;
				break;
			case '+':
				tipo = MAIS;
				tamanho++;
				break;
			case '\0':
				tipo = FIM_DE_STR;
				break;
			default:
				tipo = INVALIDO;
				printf("%d erro lexico\n", dizLinhaOriginal(contaLinha));
				tamanho++;
				//printf("token invalido. ascii: %c, valor: %d\n",c,(int)c);
		}
	}
	printf("ID DO TOKEN:%d POSICAO: %d TAMANHO: %d\n",tipo, posicao, tamanho);
	posicaoAbsoluta = stringInput + posicao;
	return tamanho;
}


ListaToken::ListaToken(){
	cabeca = (Lista *) malloc(sizeof(Lista));
	cabeca->prox = NULL;
	cabeca->token = NULL;
	fim = cabeca;
	cursor = cabeca;	
}

void ListaToken::add(Token token){
	fim->token = new Token(token);
	//ja adiciona o proximo, vazio
	fim->prox = (Lista *) malloc(sizeof(Lista));
	fim = fim->prox;
	fim->prox = NULL;
	fim->token = NULL;	
}

void ListaToken::printaTodos(){
	Lista *c;
	c = cabeca;
	while (c!=NULL){
		//if(c->token!=NULL) printf(c->token->string);
		c = c->prox;
	}
}

void ListaToken::liberaLista(Lista *c){
	if(c != NULL){
		if (c->prox != NULL){
			this->liberaLista(c->prox);
		}
		if (c->token != NULL) delete[] c->token;
		free(c);
	}
}

void ListaToken::reinicia(){
	cursor = cabeca;
}

Token* ListaToken::prox(){	//retorna o token atual
	Token *esta;
	esta = NULL;
	if (cursor != NULL){
		esta = cursor -> token;
		cursor = cursor -> prox;
	}
	return esta;
}

ListaToken::~ListaToken(){
	cursor = cabeca;
	this->liberaLista(cursor);
}




