#include <stdio.h>
#define TAMANHO_MAXIMO_ARQUIVO 1024

void rodaPrograma(int , int *);

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
		printf("arquivo inválido\n");
		return 0;
	}
	
	//tamanhoArquivo = fread(programa,sizeof(char),TAMANHO_MAXIMO_ARQUIVO,file);
	fscanf(file,"%d",programa);
	while (!feof(file) && tamanhoArquivo < 1024){
		//printf ("%d", i);
		tamanhoArquivo++;
		fscanf (file, "%d", programa+tamanhoArquivo); 
	}
	fclose(file);
//	for (int i=0; i<tamanhoArquivo; i++) printf("%d ",programa[i]);
	rodaPrograma(tamanhoArquivo, programa);

	return 0;
}

void rodaPrograma(int tamanho, int *programa){
	int i;
	int pc = 0, //contador de programa
		enderecoModificado, // -1 caso nenhum
		acc = 0; //acumulador

	while (programa[pc]!=14){
		enderecoModificado = -1;
		switch (programa[pc]){
			case 1:		//add
				acc += programa[programa[pc+1]];
				pc += 2;
				break;
			case 2:		//sub
				acc -= programa[programa[pc+1]];
				pc += 2;
				break;
			case 3:		//mult
				acc *= programa[programa[pc+1]];
				pc += 2;
				break;
			case 4:		//div
				acc /= programa[programa[pc+1]];
				pc += 2;
				break;
			case 5:		//jmp
				pc = programa[pc+1];
				//pc += 2;
				break;
			case 6:		//jmpn
				if (acc < 0) pc = programa[pc+1];
				else pc +=2;
				break;
			case 7:		//jmpp
				if (acc > 0) pc = programa[pc+1];
				else pc += 2;
				break;
			case 8:		//jmpz
				if (acc == 0) pc = programa[pc+1];
				else pc += 2;
				break;
			case 9:		//copy
				programa[programa[pc+2]] = programa[programa[pc+1]];
				enderecoModificado = programa[pc+2];
				pc += 3;
				break;
			case 10:	//load
				acc = programa[programa[pc+1]];
				pc += 2;	
				break;
			case 11:	//store
				programa[programa[pc+1]] = acc;
				enderecoModificado = programa[pc+1];
				pc += 2;
				break;
			case 12:	//input
				printf("Favor, inserir um valor numérico:\n");
				scanf("%d",&programa[programa[pc+1]]);
				enderecoModificado = programa[pc+1];
				pc += 2;
				break;
			case 13:	//output
				printf("O valor de saída é: %d\n", programa[programa[pc+1]]);
				pc += 2;
				break;
			case 14:	//stop (desnecessário acho)
				//pc += 1;
				break;
		}
		//printf("%d %d %d\n",acc, pc, programa[pc+1]);
		printf("%d ",acc);
		if (enderecoModificado>=0) printf("%d %d", enderecoModificado, programa[enderecoModificado]);
		//printf("    pc:%d",pc);
		putchar('\n');
		//getchar();
	}

	
}

















