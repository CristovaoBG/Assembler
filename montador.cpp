#include <stdio.h>
#include "token.h"
#include "montador.h"
//#include "erros.h"

void erroSemantico(int linha){
	printf("%d erro semantico\n", linha);
}

void erroSintatico(int linha){
	printf("%d erro sintatico\n", linha);
}

void erroLexico(int linha){
	printf("%d erro lexico\n", linha);
}

typedef struct Simbolo{
	//char string[100];		//texto do simbolo
	Token token;
	int valor, isDef, lista;
} TabelaSimbolos;

int monta(char *texto, int *programa){
	Token token, operando, rotulo;
	char buffer[100];
	int posicao = 0, posicaoAuxiliar, valorToken, i, j, contaLinha = 1;
	int cursorExecutavel = 0;
//	int programa[TAMANHO_MAX_ARQUIVO_EXECUTAVEL];
	bool naTabela = false;

	Simbolo tabelaDeSimbolos[TAMANHO_MAX_TABELA_DE_SIMBOLOS];
	int tamanhoTabela = 0;
	// obs: acrescentar nova fileira de inteiros especificando a linha do codigo ocorrido para caso o label nao esteja definido exeibir a posicao
	int adicionaAoEndereco[TAMANHO_MAX_ARQUIVO_EXECUTAVEL];		//tabela que registra o numero NUMERO em LABEL+NUMERO a ser adicionado no final de todo o processo
	for(i=0;i<TAMANHO_MAX_ARQUIVO_EXECUTAVEL; i++) adicionaAoEndereco[i] = 0;

	while(texto[posicao]!='\0'){
		posicaoAuxiliar = posicao;	//grava posicao
		posicao += token.leUmToken(texto, posicao);
		//remove espacos desnecessarios.
		while(token.tipo == QUEBRA_DE_LINHA || token.tipo == ESPACO || token.tipo == TABULACAO) {
			if (token.tipo == QUEBRA_DE_LINHA) contaLinha++;
			posicaoAuxiliar = posicao;
			posicao += token.leUmToken(texto,posicao);
		}
		if (token.tipo == INVALIDO){
			erroLexico(token.leLinhaAtual());
		}
		if (token.tipo == SECTION){
			posicao += token.leUmToken(texto,posicao);
			while(token.tipo == ESPACO || token.tipo == TABULACAO){
				posicao += token.leUmToken(texto,posicao);
			}
			if(token.tipo == TEXT){
				
			}
			else if(token.tipo == DATA){

			}
			else{ //ERRO SECAO NAO RECONHECIDA
				erroLexico(token.leLinhaAtual());	//TA CERTO ISSO?
			}
		}
		if (token.tipo == PALAVRA){	//nesse caso acho que so pode ser declaracao de rotulo(label)
			rotulo = token;
			posicao += token.leUmToken(texto, posicao);
			while(token.tipo == QUEBRA_DE_LINHA || token.tipo == ESPACO || token.tipo == TABULACAO) {
				if (token.tipo == QUEBRA_DE_LINHA) contaLinha++;
				posicaoAuxiliar = posicao;
				posicao += token.leUmToken(texto,posicao);
			}
			if(token.tipo == DOIS_PONTOS){
				//label declarada corretamente.
				//verifica se segue outro rotulo, o que eh proibido.
				//vai na tabela de simbolos e procura, se nao tiver cria e define.
				//se tiver entao define e varre a lista preenchendo o seu valor
				//se tiver na lista e tiver definida é erro

				//verifica se segue outro rotulo, o que eh proibido por lei
				i = token.leLinhaAtual();
				posicaoAuxiliar = posicao;
				posicaoAuxiliar += token.leUmToken(texto,posicaoAuxiliar);
				while(token.tipo == ESPACO) posicaoAuxiliar += token.leUmToken(texto,posicaoAuxiliar);
				if (token.tipo == PALAVRA){
					posicaoAuxiliar += token.leUmToken(texto,posicaoAuxiliar);
					while(token.tipo == ESPACO) posicaoAuxiliar += token.leUmToken(texto,posicaoAuxiliar);
					if(token.tipo == DOIS_PONTOS){ //dois rotulos na mesma linha! eh proibido por lei.
						erroSintatico(token.leLinhaAtual());
						//vai para a proxima linha;
					}			
				}
				token.atribuiContaLinha(i);

				// procura
				naTabela = false;
				for(i = 0; i < tamanhoTabela; i++){
					if(comparaTokens(rotulo,tabelaDeSimbolos[i].token)){
						naTabela = true;
						break;
					}
				}
				//break;
				if(!naTabela){	//se nao tiver na tabela, cria e define
					tabelaDeSimbolos[tamanhoTabela].token = rotulo;
					tabelaDeSimbolos[tamanhoTabela].isDef = true;
					tabelaDeSimbolos[tamanhoTabela].lista = -1;
					tabelaDeSimbolos[tamanhoTabela].valor = cursorExecutavel;
					tamanhoTabela++;
				}
				else if(!tabelaDeSimbolos[i].isDef){	//se tiver na tabela varre a lista e define
					//printf(" E ELA TA NA TABELA DE SIMBOLOS UAU\n");					
					tabelaDeSimbolos[i].isDef = true;
					while(tabelaDeSimbolos[i].lista != -1){ //enquanto a lista nao acabar
						j = tabelaDeSimbolos[i].lista;	// j = item da lista
						tabelaDeSimbolos[i].lista = programa[j];	//tabela de simbolos vira item seguinte
						programa[j] = cursorExecutavel;		//lista dispensada, atribui valor real na memoria
					}
					tabelaDeSimbolos[i].valor = cursorExecutavel;
				}else{
					// ERRO, DUPLA DEFINICAO
					erroSemantico(token.leLinhaAtual());
				}
			}
			else{
				//erro, palavra solta aleatoria
				erroSintatico(token.leLinhaAtual()); ///////////////// TA CERTO ISSO?							
			}
		}
		// ###########	INSTRUCOES DE UM OPERANDO	###########
		if (token.tipo >= ADD && token.tipo <= OUTPUT && token.tipo != COPY){	// sao todas as instruncoes com um operando
			operando = token;
			posicao += token.leUmToken(texto,posicao);
			while(token.tipo == ESPACO || token.tipo == TABULACAO) {	//ignora tabs e espacos entre instrucao e operando
				posicao += token.leUmToken(texto,posicao);
			}
			if (token.tipo == NUMERO){
				//insere instrucao e operando no programa executavel.
				programa[cursorExecutavel] = operando.tipo;	//o tipo eh o proprio codigo pq eu fui esperto ou eu fui burro nao sei ainda
				cursorExecutavel++;
				token.copiaTokenParaString(buffer);
				programa[cursorExecutavel] = atoi(buffer);	//atoi eh de uma biblioteca incluida, serve pra converter de string pra int
				if (operando.tipo == DIV && programa[cursorExecutavel] == 0){
					//ERRO DE DIVISAO POR ZERO
					erroSintatico(token.leLinhaAtual());
				}
				cursorExecutavel++;
			}else if (token.tipo == PALAVRA){
				//printf("eh palavra ok\n");
				//verifica se simbolo existe na tabela de simbolo,
				//se existir e tiver definida insere o valor no executavel
				//se existir e nao tiver definida insere valor da posicao do programa atual na tabela de simbolos
				// e insere o valor anterior na tabela de simbolos
				//se nao existir, insere na tabela de simbolo com -1
				token.copiaTokenParaString(buffer);
				//verifica se esta na tabela
				naTabela = false;
				for(i=0; i<tamanhoTabela;i++){
					if(comparaTokens(token,tabelaDeSimbolos[i].token)){
						naTabela = true;
						break;
					}
				}
				if(naTabela){
					if (tabelaDeSimbolos[i].isDef){
						programa[cursorExecutavel] = operando.tipo;	//o tipo eh o proprio codigo pq eu fui esperto ou eu fui burro nao sei ainda
						cursorExecutavel++;
						programa[cursorExecutavel] = tabelaDeSimbolos[i].valor; // insere valor do label no executavel
						cursorExecutavel++;	
					}
					else{	//se simbolo esta na tabela mas nao esta definido 
						programa[cursorExecutavel] = operando.tipo;	//o tipo eh o proprio codigo pq eu fui esperto ou eu fui burro nao sei ainda
						cursorExecutavel++;
						// insere na lista
						programa[cursorExecutavel] = tabelaDeSimbolos[i].lista;
						tabelaDeSimbolos[i].lista = cursorExecutavel; // insere valor do label no executavel
						cursorExecutavel++;	
					}
				}
				else{	//se nao esta na tabela de simbolos
					//insere na tabela de simbolos
					programa[cursorExecutavel] = operando.tipo;
					cursorExecutavel++;
					tabelaDeSimbolos[tamanhoTabela].token = token;
					tabelaDeSimbolos[tamanhoTabela].isDef = false;
					tabelaDeSimbolos[tamanhoTabela].lista = cursorExecutavel;	//indica que eh o primeiro elemento da lista
					programa[cursorExecutavel] = -1;
					cursorExecutavel++;
					tamanhoTabela++;
				}
				// verifica se eh ...+NUMERO
				posicao += token.leUmToken(texto,posicao);
				if(token.tipo == MAIS){
					//le numero a seguir e copia para a tabela adicionaAoEndereco na posicao do executavel
					posicao += token.leUmToken(texto,posicao);
					if (token.tipo == NUMERO){
						token.copiaTokenParaString(buffer);
						 adicionaAoEndereco[cursorExecutavel-1]= atoi(buffer);
					}
				}
			}
			else if(token.tipo == QUEBRA_DE_LINHA){
				//ERRO
				erroSintatico(token.leLinhaAtual());
			}
		}
		// ###########	FIM INSTRUCOES DE UM OPERANDO	###########
		// ###########	INSTRUCOES DE DOIS OPERANDOS (COPY)	###########
		if (token.tipo == COPY){	// sao todas as instruncoes com um operando
			operando = token;
			posicao += token.leUmToken(texto,posicao);
			while(token.tipo == ESPACO || token.tipo == TABULACAO) {	//ignora tabs e espacos entre instrucao e operando
				posicao += token.leUmToken(texto,posicao);
			}
			if (token.tipo == NUMERO){
				//insere instrucao e operando no programa executavel.
				programa[cursorExecutavel] = operando.tipo;	//o tipo eh o proprio codigo pq eu fui esperto ou eu fui burro nao sei ainda
				cursorExecutavel++;
				token.copiaTokenParaString(buffer);
				programa[cursorExecutavel] = atoi(buffer);	//atoi eh de uma biblioteca incluida, serve pra converter de string pra int
				cursorExecutavel++;				
				//prigrama[cursorExecutavel] = 
			}else if (token.tipo == PALAVRA){
				//printf("eh palavra ok\n");
				//verifica se simbolo existe na tabela de simbolo,
				//se existir e tiver definida insere o valor no executavel
				//se existir e nao tiver definida insere valor da posicao do programa atual na tabela de simbolos
				// e insere o valor anterior na tabela de simbolos
				//se nao existir, insere na tabela de simbolo com -1
				token.copiaTokenParaString(buffer);
				//verifica se esta na tabela
				naTabela = false;
				for(i=0; i<tamanhoTabela;i++){
					if(comparaTokens(token,tabelaDeSimbolos[i].token)){
						naTabela = true;
						break;
					}
				}
				if(naTabela){
					if (tabelaDeSimbolos[i].isDef){
						programa[cursorExecutavel] = operando.tipo;	//o tipo eh o proprio codigo pq eu fui esperto ou eu fui burro nao sei ainda
						cursorExecutavel++;
						programa[cursorExecutavel] = tabelaDeSimbolos[i].valor; // insere valor do label no executavel
						cursorExecutavel++;	
					}
					else{	//se simbolo esta na tabela mas nao esta definido 
						programa[cursorExecutavel] = operando.tipo;	//o tipo eh o proprio codigo pq eu fui esperto ou eu fui burro nao sei ainda
						cursorExecutavel++;
						// insere na lista
						programa[cursorExecutavel] = tabelaDeSimbolos[i].lista;
						tabelaDeSimbolos[i].lista = cursorExecutavel; // insere valor do label no executavel
						cursorExecutavel++;	
					}
				}
				else{	//se nao esta na tabela de simbolos
					//insere na tabela de simbolos
					programa[cursorExecutavel] = operando.tipo;
					cursorExecutavel++;
					tabelaDeSimbolos[tamanhoTabela].token = token;
					tabelaDeSimbolos[tamanhoTabela].isDef = false;
					tabelaDeSimbolos[tamanhoTabela].lista = cursorExecutavel;	//indica que eh o primeiro elemento da lista
					programa[cursorExecutavel] = -1;
					cursorExecutavel++;
					tamanhoTabela++;
				}
				// verifica se eh ...+NUMERO
				posicaoAuxiliar = posicao;
				posicaoAuxiliar += token.leUmToken(texto,posicao);
				if(token.tipo == MAIS){
					//le numero (a seguir de posicaoAuxiliar) e copia para a tabela adicionaAoEndereco na posicao do executavel
					posicaoAuxiliar += token.leUmToken(texto,posicaoAuxiliar);
					posicao = posicaoAuxiliar;
					if (token.tipo == NUMERO){
						token.copiaTokenParaString(buffer);
						adicionaAoEndereco[cursorExecutavel-1]= atoi(buffer);
						posicao = posicaoAuxiliar;
					}
					else{
						//ERRO, somou com uma porcaria que nao eh numero
						if (token.tipo == QUEBRA_DE_LINHA) erroSintatico(token.leLinhaAtual()-1);
						else erroSintatico(token.leLinhaAtual());
						// POSICAO = LE_TOKEN_ATE_QUEBRA_DE_LINHA(); //AQUI
						// CONTINUE;
					}
				}
			}
			else{	//ERRO, NENHUM OPERANDO FORNECIDO
				if (token.tipo == QUEBRA_DE_LINHA) erroSintatico(token.leLinhaAtual()-1);
				else erroSintatico(token.leLinhaAtual());
				continue;
			}
			//le virgula e segundo operando
			posicao += token.leUmToken(texto,posicao);
			if(token.tipo == VIRGULA){
				posicao += token.leUmToken(texto,posicao);
				//le numero ou label ou label+NUMERO
				if (token.tipo == NUMERO){
					//insere instrucao e operando no programa executavel.
					programa[cursorExecutavel] = operando.tipo;	//o tipo eh o proprio codigo pq eu fui esperto ou eu fui burro nao sei ainda
					cursorExecutavel++;
					token.copiaTokenParaString(buffer);
					programa[cursorExecutavel] = atoi(buffer);	//atoi eh de uma biblioteca incluida, serve pra converter de string pra int
					cursorExecutavel++;				
					//prigrama[cursorExecutavel] = 
				}else if (token.tipo == PALAVRA){
					//printf("eh palavra ok\n");
					//verifica se simbolo existe na tabela de simbolo,
					//se existir e tiver definida insere o valor no executavel
					//se existir e nao tiver definida insere valor da posicao do programa atual na tabela de simbolos
					// e insere o valor anterior na tabela de simbolos
					//se nao existir, insere na tabela de simbolo com -1
					token.copiaTokenParaString(buffer);
					//verifica se esta na tabela
					naTabela = false;
					for(i=0; i<tamanhoTabela;i++){
						if(comparaTokens(token,tabelaDeSimbolos[i].token)){
							naTabela = true;
							break;
						}
					}
					if(naTabela){
						if (tabelaDeSimbolos[i].isDef){
							programa[cursorExecutavel] = tabelaDeSimbolos[i].valor; // insere valor do label no executavel
							cursorExecutavel++;	
						}

						else{	//se simbolo esta na tabela mas nao esta definido 
							// insere na lista
							programa[cursorExecutavel] = tabelaDeSimbolos[i].lista;
							tabelaDeSimbolos[i].lista = cursorExecutavel; // insere valor do label no executavel
							cursorExecutavel++;	
						}
					}
					else{	//se nao esta na tabela de simbolos
						//insere na tabela de simbolos
						tabelaDeSimbolos[tamanhoTabela].token = token;
						tabelaDeSimbolos[tamanhoTabela].isDef = false;
						tabelaDeSimbolos[tamanhoTabela].lista = cursorExecutavel;	//indica que eh o primeiro elemento da lista
						programa[cursorExecutavel] = -1;
						cursorExecutavel++;
						tamanhoTabela++;
					}
					// verifica se eh ...+NUMERO
					posicaoAuxiliar = posicao;
					posicaoAuxiliar += token.leUmToken(texto,posicao);
					if(token.tipo == MAIS){
						//le numero (a seguir de posicaoAuxiliar) e copia para a tabela adicionaAoEndereco na posicao do executavel
						posicaoAuxiliar += token.leUmToken(texto,posicaoAuxiliar);
						posicao = posicaoAuxiliar;
						if (token.tipo == NUMERO){
							token.copiaTokenParaString(buffer);
							adicionaAoEndereco[cursorExecutavel-1]= atoi(buffer);
							posicao = posicaoAuxiliar;
						}
						else{
							//ERRO, somou com uma porcaria que nao eh numero
							if (token.tipo == QUEBRA_DE_LINHA) erroSintatico(token.leLinhaAtual()-1);
							else erroSintatico(token.leLinhaAtual());
						}
					}
				}
				else{	//ERRO nao encontrou um segundo argumento valido
					if (token.tipo == QUEBRA_DE_LINHA) erroSintatico(token.leLinhaAtual()-1);
					else erroSintatico(token.leLinhaAtual());
				}
			}
			else{
				//ERRO, nao achou virgula entre os argumentos
				erroSintatico(token.leLinhaAtual());
			}
		}
		// ###########	FIM INSTRUCOES DE DOIS OPERANDOS	###########
		if (token.tipo == STOP){
			programa[cursorExecutavel] = token.tipo;
			cursorExecutavel++;
			//printf("!!!!!!!!!  %d  !!!!!!!!!!!!!!\n\n", token.tipo);
		}
		if (token.tipo == SPACE){
			//se for seguido de um numero n, escreve n zeros na memoria,
			//se nao for seguido de nada, escreve um zero na memoria
			posicao += token.leUmToken(texto, posicao);
			while(token.tipo == ESPACO || token.tipo == TABULACAO) { //aqui ele nao ignora quebra de linha
				posicaoAuxiliar = posicao;
				posicao += token.leUmToken(texto,posicao);
			}
			if(token.tipo == NUMERO){
				token.copiaTokenParaString(buffer);
				i = atoi(buffer);	//atoi eh de uma biblioteca incluida, serve pra converter de string pra int	
				for (j=0; j<i; j++){
					//escreve 00 e incrementa programa
					programa[cursorExecutavel] = 0;
					cursorExecutavel++;
				}
			}
			else{
				programa[cursorExecutavel] = 0;
				cursorExecutavel++;
			}
		}
		if (token.tipo == CONST){
			//le proximo token e escreve no programa
			posicao += token.leUmToken(texto, posicao);
			while(token.tipo == QUEBRA_DE_LINHA || token.tipo == ESPACO || token.tipo == TABULACAO) {
				posicaoAuxiliar = posicao;
				posicao += token.leUmToken(texto,posicao);
			}
			if (token.tipo == NUMERO){
				token.copiaTokenParaString(buffer);
				programa[cursorExecutavel] = atoi(buffer);	//atoi eh de uma biblioteca incluida, serve pra converter de string pra int	
				cursorExecutavel++;
			}
		}
	}
	
	for(i=0;i<TAMANHO_MAX_ARQUIVO_EXECUTAVEL; i++) {	//adiciona NUMERO ao valor de memoria em LABEL+NUMERO
		programa[i]+=adicionaAoEndereco[i];
	}

	printf("\n\nTABELA DE SIMBOLOS:\n");
	for(i=0;i<tamanhoTabela;i++){
		tabelaDeSimbolos[i].token.copiaTokenParaString(buffer);
		printf("token: %s isDef: %d lista: %d\n",buffer,tabelaDeSimbolos[i].isDef,tabelaDeSimbolos[i].lista);
		
	}

	//verifica se todos os simbolos da tabela de simbolos foram definidos

	printf("\n\n"); 
	for(j=0;j<cursorExecutavel; j++) printf("%d ",programa[j]);
	printf("\n");
	return cursorExecutavel;
}













