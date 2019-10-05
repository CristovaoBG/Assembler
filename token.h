#ifndef _tokenH_
#define _tokenH_

#include <stdio.h>
#include <stdlib.h>

typedef struct Lista Lista;	//para uso interno

enum token_enum{
	EQU = 1,
	IF,
	ESPACO,			//3
	TABULACAO,
	QUEBRA_DE_LINHA, //5
	NUMERO,
	PALAVRA,		//7
	DOIS_PONTOS,
	INVALIDO,		//9
	TOTAL_DE_TOKENS,
};

class Token {
	public:
		int tipo, 
			tamanho, 
			valor;	//caso seja numero

		int posicao;			//posicao relativa ao inicio da string
		int leUmToken(char *string, int posicao); //retorna tamanho do token
		char* posicaoAbsoluta;	//posicao de inicio na memoria
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
