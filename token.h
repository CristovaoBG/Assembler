#ifndef _tokenH_
#define _tokenH_

#include <stdio.h>
#include <stdlib.h>

enum token_enum{
	ROTULO,
	EQU
};

class Token {
	public:
		int tipoDeToken;
		char string[50];
	};

typedef struct Lista{
	Token *token;
	struct Lista *prox;
} Lista;

class ListaToken{
	public:
		ListaToken();
		~ListaToken();
		Token* atual();
		Token* prox();
		void add(Token token);
		void liberaLista(Lista *c);
		void reinicia();
	private:
		Lista *cabeca, *cursor, *fim;
};

#endif
