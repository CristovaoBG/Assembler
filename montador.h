#ifndef __montador__
#define __montador__

#define TAMANHO_MAX_TABELA_DE_SIMBOLOS 1024
#define TAMANHO_MAX_ARQUIVO_EXECUTAVEL 1024
#define TAMANHO_BITMAP_REALOCACAO 2048

int monta(char *texto, int *executavel, char *tabelaDeDefinicoes, char *tabelaDeUso, int* bitmapRealocacao);	//retorna tamanho do executavel
void reestruturaSections(char *texto);
void erroLexico(Token *token, int *posicao);
int dizLinhaOriginal(int linha);
extern int mostrarErroLexico;
extern int numeroDeArquivosFornecidos;

#endif
