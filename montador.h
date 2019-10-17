#ifndef __montador__
#define __montador__

#define TAMANHO_MAX_TABELA_DE_SIMBOLOS 1024
#define TAMANHO_MAX_ARQUIVO_EXECUTAVEL 1024

int monta(char *texto, int *executavel);	//retorna tamanho do executavel
void reestruturaSections(char *texto);

#endif
