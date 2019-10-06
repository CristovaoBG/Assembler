#ifndef _tokenH_
#define _tokenH_

#include <stdio.h>
#include <stdlib.h>

typedef struct Lista Lista;	//para uso interno

enum token_enum{
	ESPACO,
	ADD,
	SUB,
	MULT,
	DIV,
	JMP,
	JMPN,
	JMPP,
	JMPZ,
	COPY,
	LOAD,
	STORE,
	INPUT,
	OUTPUT,
	STOP,
	EQU,
	IF,
	TABULACAO,
	QUEBRA_DE_LINHA,
	NUMERO,
	PALAVRA,		
	DOIS_PONTOS,
	INVALIDO,		
	TOTAL_DE_TOKENS,
};

class Token {
	public:
		int tipo, 
			tamanho,	//tamanho da string que compoe o token
			tamanhoInst, //quantos espacos a instrucao ocupa na memoria, se for instrucao
			codigo;		// qual o codigo que representa essa instruncao em assembly

		int posicao;			//posicao do texto relativa ao inicio da string
		int leUmToken(char *string, int posicao); //retorna tamanho textual do token
		char* posicaoAbsoluta;	//posicao do texto referente ao inicio da memoria
		void copiaTokenParaString(char *destino); 
	};

class ListaToken{
	public:
		ListaToken();
		~ListaToken();
		Token* atual();
		Token* prox();
		void add(Token token);
		void liberaLista(Lista *c);
		void reinicia();
		void printaTodos();
	private:
		Lista *cabeca, *cursor, *fim;
};

#endif
