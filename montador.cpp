#include <stdio.h>

#define TAMANHO_MAXIMO_ARQUIVO 1024

int main(int argc, char *argv[]){
	//abre e carrega arquivo na memoria
	if (argc != 2) {
		printf("uso incorreto.\n");
		return 0;
		}

	int programa[TAMANHO_MAXIMO_ARQUIVO];
	int tamanhoArquivo = 0;
	FILE *file = NULL;
	file = fopen(argv[1],"r");
	if (file == NULL){
		printf("arquivo não encontrado\n");
		return 0;
	}

	fscanf(file,"%d",programa);
	while (!feof(file) && tamanhoArquivo < 1024){
		//printf ("%d", i);
		tamanhoArquivo++;
		fscanf (file, "%d", programa+tamanhoArquivo); 
	}
	fclose(file);

	return 0;
}
