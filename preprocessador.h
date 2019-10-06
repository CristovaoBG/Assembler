#ifndef _preprocessador_
#define _preprocessador_

#include "token.h"

void preProcessa(char *string, int tamanho);
void removeCaracteres(char *string,int nCaracteres); //remove o numero nCaracteres de caracteres na posicao de string 
void insereStringNaPosicao(char *strAlvo, char *str); //insere str na posicao strAlvo

#endif
