#include "token.h"

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




