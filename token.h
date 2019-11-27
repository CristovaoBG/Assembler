#ifndef _tokenH_
#define _tokenH_

#include <stdio.h>
#include <stdlib.h>
//#define MOSTRA_ERROS
typedef struct Lista Lista;	//para uso interno

enum token_enum{	// a posicao das instrucoes equivale ao codgo delas, portanto nao mudar a ordem das que tem codigo
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
	SECTION,
	SPACE,
	CONST,
	EQU,
	IF,
	DATA,
	TEXT,
	BEGIN,
	END,
	PUBLIC,
	EXTERN,
	VIRGULA,
	MAIS,
	TABULACAO,
	QUEBRA_DE_LINHA,
	NUMERO,
	PALAVRA,		
	DOIS_PONTOS,
	FIM_DE_STR,
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
		int leNumero(); //retorna numero se for numero
		bool comparaToken(Token token);
		void copiaTokenParaString(char *destino); 
		Token();
		int leLinhaAtual();
		void atribuiContaLinha(int valor);
	private:
		int contaLinha;
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

bool comparaTokens(Token t1, Token t2);

#endif
