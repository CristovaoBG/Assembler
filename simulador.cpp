#include <stdio.h>
#define TAMANHO_MAXIMO_ARQUIVO 1024

void rodaPrograma(int , char *);

int main(int argc, char *argv[]){
	//abre e carrega arquivo na memoria
	if (argc != 2) {
		printf("uso incorreto.\n");
		return 0;
		}

	char programa[TAMANHO_MAXIMO_ARQUIVO];
	int tamanhoArquivo;
	FILE *file = NULL;
	file = fopen(argv[1],"r");
	if (file == NULL){
		printf("arquivo inválido\n");
		return 0;
	}
	
	tamanhoArquivo = fread(programa,sizeof(char),TAMANHO_MAXIMO_ARQUIVO,file);
	fclose(file);
	
	rodaPrograma(tamanhoArquivo, programa);

	return 0;
}

void rodaPrograma(int tamanho, char *programa){
	int i;
	int pc = 0; //contador de programa
	int acc = 0; //acumulador

	//for(i=0; i<tamanho; i++){
	//	putchar(programa[i]);
	//}

	
}

















