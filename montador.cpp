#include <stdio.h>
#include "token.h"
#include "montador.h"
#include <string.h>
//#include "erros.h"

#define TAMANHO_BUFFER_SECTION 1024

int nLinhasText = 0, nLinhasData = 0;
bool ordemCorreta = false;

int dizLinhaOriginal(int linha){
	int  linhaOriginal;
	//linha = token.leLinhaAtual();
	if (!ordemCorreta){
		if (linha>nLinhasText){	//significa que esta na secao data
			linhaOriginal = linha - nLinhasText;
		}
		else{					//significa que esta na secao text
			linhaOriginal = linha + nLinhasData;
		}
	}
	else{
		linhaOriginal = linha;
	}
	//printf("opa\n");
	return linhaOriginal;
}

bool verificaSeEhSecaoText(int linha){	//se nao for, significa que eh data
	if (linha>nLinhasText){
	}
}

void erroLexico(Token *token, int *posicao){
	printf("%d erro lexico\n", dizLinhaOriginal(token->leLinhaAtual()), *posicao);
}

void erroSemantico(Token *token,int *posicao){
	printf("%d erro semantico\n", dizLinhaOriginal(token->leLinhaAtual()), *posicao);
}

void erroSintatico(Token *token,int *posicao){
	//printf("%d erro sintatico, posicao %d\n", token->leLinhaAtual(), *posicao);
	printf("%d erro sintatico\n", dizLinhaOriginal(token->leLinhaAtual()), *posicao);
}

void reestruturaSections(char *texto){
	char 	bufferText[TAMANHO_BUFFER_SECTION],
			bufferData[TAMANHO_BUFFER_SECTION];
	Token token, prevToken;
	int posicao = 0, posicaoText = 0, posicaoData = 0, tamanhoData = 0;

	printf("\n INICIO\n");

/*	posicao += token.leUmToken(texto, posicao);
	while(token.tipo != FIM_DE_STR){
		posicao += token.leUmToken(texto, posicao);
		printf("%d ", posicao);
	}

	printf("\n FIM\n");
	posicao = 0;
	token.atribuiContaLinha(0);
*/
	// remove \ns que estao no fim do arquivo por algum motivo
	int i=0;
	while(texto[i]!='\0') i++;
	i--;
	while(texto[i]=='\n'){
		texto[i] = '\0';
		i--;
	}
	//texto[++i] = '\n';
	//texto[++i] = '\0';
	printf("REESTRUTURANDO:\n\n");

	posicao += token.leUmToken(texto, posicao);
	while(token.tipo == QUEBRA_DE_LINHA || token.tipo == ESPACO || token.tipo == TABULACAO){
		posicao += token.leUmToken(texto, posicao);
	}
	if(token.tipo!=SECTION){
		//ERRO SECAO INVALIDA
		
	}
	else{
		posicao += token.leUmToken(texto, posicao);
		while(token.tipo == QUEBRA_DE_LINHA || token.tipo == ESPACO || token.tipo == TABULACAO){
			posicao += token.leUmToken(texto, posicao);
		}
		if (token.tipo == TEXT){
			ordemCorreta = true;
			while(token.tipo != SECTION && token.tipo != FIM_DE_STR){
				//tamanhoText = posicao;	//quando sair deste loop vai conter o valor correto se tiver tudo certo.
				nLinhasText = token.leLinhaAtual();
				posicao += token.leUmToken(texto, posicao);
			}
		}
		else if(token.tipo == DATA){
			//le secao de dados ate encontrar sessao text
			while(token.tipo != SECTION && token.tipo != FIM_DE_STR){
				tamanhoData = posicao;	//quando sair deste loop vai conter o valor correto se tiver tudo certo.
				nLinhasData = token.leLinhaAtual();
				posicao += token.leUmToken(texto, posicao);
			}
			if(token.tipo == SECTION){
				nLinhasData--;	// nao tenho certeza pq
				posicao += token.leUmToken(texto, posicao);
				while(token.tipo == ESPACO || token.tipo == TABULACAO){
					posicao += token.leUmToken(texto, posicao);
				}
				if(token.tipo == TEXT){
					while(token.tipo != FIM_DE_STR){	// conta o total de linhas ate o fim do programa
						prevToken = token;
						posicao += token.leUmToken(texto, posicao);
					}
					strncpy(bufferData,texto,tamanhoData);
					bufferData[tamanhoData] = '\0';
					strcpy(bufferText,texto+tamanhoData);
					strcpy(texto,bufferText);
					while(texto[posicaoText]!='\0') posicaoText++;
					texto[posicaoText++]='\n';
					nLinhasText = token.leLinhaAtual() - nLinhasData;
					strcpy(texto+posicaoText,bufferData);
				}	
				else{
					//ERRO SECAO DESCONHECIDA (NAO EH TEXT MAS PODE SER DATA, QUE JA FOI)
					erroSintatico(&token,&posicao);
				}			
			}
			else{	//ERRO SECAO DE TEXT AUSENTE
				erroSemantico(&token,&posicao);
			}
		}
		else {
			//ERRO SECAO NAO EH TEXT NEM DATA
			erroSemantico(&token,&posicao);
		}
	}
	printf("aiosdjdaoisjdoasijd\n%s\nnLinhasData: %d, nLinhasText: %d, total de linhas: %d, prevToken: %d, posicao: %d\n",texto,nLinhasData, nLinhasText, token.leLinhaAtual(),prevToken.tipo,posicao);
	
	
}

typedef struct Simbolo{
	//char string[100];		//texto do simbolo
	Token token;
	int valor, isDef, lista, definidoNaLinha, isConst, isConst0;
} TabelaSimbolos;

int monta(char *texto, int *programa){
	Token token, operando, rotulo, tokenAuxiliar;
	char buffer[100], buffer2[100];
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
		if (token.tipo == SECTION){
			posicao += token.leUmToken(texto,posicao);
			while(token.tipo == ESPACO || token.tipo == TABULACAO){
				posicao += token.leUmToken(texto,posicao);
			}
			if(token.tipo == TEXT){
				
			}
			else if(token.tipo == DATA){

			}
			else{ //ERRO SECAO NAO RECONHECIDA (JA FOI DADO ESSE ERRO NO DE CIMA)
				//erroSintatico(&token,&posicao);	//TA CERTO ISSO?
				// vai para prox linmha e volta o loop
				//while (texto[posicao]!= '\n' && texto[posicao]!= '\0') posicao ++;
				//continue;
			}
		}
		if (token.tipo == PALAVRA){	//nesse caso acho que so pode ser declaracao de rotulo(label)

			rotulo = token;
			posicao += token.leUmToken(texto, posicao);
			while(token.tipo == ESPACO || token.tipo == TABULACAO) {
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
				tokenAuxiliar = token;
				posicaoAuxiliar = posicao;
				posicaoAuxiliar += token.leUmToken(texto,posicaoAuxiliar);
				while(token.tipo == ESPACO) posicaoAuxiliar += token.leUmToken(texto,posicaoAuxiliar);
				if (token.tipo == PALAVRA){
					posicaoAuxiliar += token.leUmToken(texto,posicaoAuxiliar);
					while(token.tipo == ESPACO) posicaoAuxiliar += token.leUmToken(texto,posicaoAuxiliar);
					if(token.tipo == DOIS_PONTOS){ //dois rotulos na mesma linha! eh proibido por lei.
						//ERRO, DOIS ROTULOS NA MESMA LINHA
						erroSintatico(&token,&posicao);
						//vai para a proxima linha;
						//while (texto[posicao]!= '\n' && texto[posicao]!= '\0') posicao ++;
						//continue;
					}			
				}
				token = tokenAuxiliar;

				// procura
				naTabela = false;
				for(i = 0; i < tamanhoTabela; i++){
					if(comparaTokens(rotulo,tabelaDeSimbolos[i].token)){
						naTabela = true;
						break;
					}
				}
				if(!naTabela){	//se nao tiver na tabela, cria e define
					tabelaDeSimbolos[tamanhoTabela].token = rotulo;
					tabelaDeSimbolos[tamanhoTabela].isDef = true;
					tabelaDeSimbolos[tamanhoTabela].lista = -1;
					tabelaDeSimbolos[tamanhoTabela].valor = cursorExecutavel;
					tabelaDeSimbolos[tamanhoTabela].definidoNaLinha = token.leLinhaAtual();
					//verifica se eh const
					tokenAuxiliar = token;
					posicaoAuxiliar = posicao;
					do{
						posicaoAuxiliar += tokenAuxiliar.leUmToken(texto,posicaoAuxiliar);
					}while(tokenAuxiliar.tipo == ESPACO || tokenAuxiliar.tipo == QUEBRA_DE_LINHA);
					if (tokenAuxiliar.tipo == CONST){
						tabelaDeSimbolos[tamanhoTabela].isConst = true;
						// verifica se eh 0
						do{
							posicaoAuxiliar += tokenAuxiliar.leUmToken(texto,posicaoAuxiliar);
						}while(tokenAuxiliar.tipo == ESPACO || tokenAuxiliar.tipo == QUEBRA_DE_LINHA);	
						tokenAuxiliar.copiaTokenParaString(buffer);
						if (atoi(buffer) == 0) tabelaDeSimbolos[tamanhoTabela].isConst0 = true;
						else tabelaDeSimbolos[tamanhoTabela].isConst0 = false;
					}
					else{
						tabelaDeSimbolos[tamanhoTabela].isConst = false;
					}
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
					// a partir daqui i eh o indice do simbolo em questao
					tabelaDeSimbolos[i].valor = cursorExecutavel;
					tabelaDeSimbolos[i].definidoNaLinha = token.leLinhaAtual();
					//verifica se eh const
					tokenAuxiliar = token;
					posicaoAuxiliar = posicao;
					do{
						posicaoAuxiliar += tokenAuxiliar.leUmToken(texto,posicaoAuxiliar);
					}while(tokenAuxiliar.tipo == ESPACO || tokenAuxiliar.tipo == QUEBRA_DE_LINHA);
					//tabelaDeSimbolos[i].isConst = token.tipo == CONST? true : false;
					if (tokenAuxiliar.tipo == CONST){
						tabelaDeSimbolos[i].isConst = true;
						do{
							posicaoAuxiliar += tokenAuxiliar.leUmToken(texto,posicaoAuxiliar);
						}while(tokenAuxiliar.tipo == ESPACO || tokenAuxiliar.tipo == QUEBRA_DE_LINHA);	
						tokenAuxiliar.copiaTokenParaString(buffer);
						if (atoi(buffer) == 0) tabelaDeSimbolos[i].isConst0 = true;
						else tabelaDeSimbolos[i].isConst0 = false;
					}
					else{
						tabelaDeSimbolos[i].isConst = false;
					}
					
				}else{
					// ERRO, DUPLA DEFINICAO
					erroSemantico(&token,&posicao);
					// vai para prox linmha e volta o loop
					//while (texto[posicao]!= '\n' && texto[posicao]!= '\0') posicao ++;
					//continue;
				}
			}
			else{
				//erro, palavra solta aleatoria
				erroSintatico(&token,&posicao); ///////////////// TA CERTO ISSO?	
				// vai para prox linmha e volta o loop
				//while (texto[posicao]!= '\n' && texto[posicao]!= '\0') posicao ++;
				//continue;						
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
				//if (operando.tipo == DIV && programa[cursorExecutavel] == 0){
					//ERRO DE DIVISAO POR ZERO (TA ERRADO, EH SE FOR CTT)
					//erroSintatico(&token,&posicao);
					//continue;
				//}
				cursorExecutavel++;
			}else if (token.tipo == PALAVRA){
				//printf("comando de um arg. tipo: %d\n", operando.tipo);
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
			else{
				//ERRO
				erroSintatico(&token,&posicao);
				//printf("---token: %d---", token.tipo);
				while (texto[posicao] != '\n') posicao++;
				
				//continue;
				// vai para prox linmha e volta o loop
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
				while(token.tipo == ESPACO || token.tipo == TABULACAO) {	//ignora tabs e espacos entre instrucao e operando
					posicaoAuxiliar += token.leUmToken(texto,posicaoAuxiliar);
				}
				if(token.tipo == QUEBRA_DE_LINHA){
					posicao = posicaoAuxiliar;
				}
				else if(token.tipo == MAIS){
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
						//if (token.tipo == QUEBRA_DE_LINHA) erroSintatico(token.leLinhaAtual()-1);
						//erroSintatico(&token,&posicao);
						//continue;
					}
				}
			}
			else{	//ERRO, NENHUM OPERANDO FORNECIDO
				//erroSintatico(&token,&posicao);
				//continue;
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
					while(token.tipo == ESPACO || token.tipo == TABULACAO) {	//ignora tabs e espacos entre instrucao e operando
						posicaoAuxiliar += token.leUmToken(texto,posicaoAuxiliar);
					}
					if(token.tipo == QUEBRA_DE_LINHA){
						posicao = posicaoAuxiliar;
					}
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
							//erroSintatico(&token,&posicao);
							//continue;
						}
					}
				}
				else{	//ERRO nao encontrou um segundo argumento valido
					//erroSintatico(&token,&posicao);
					//continue;
				}
			}
			else{
				//ERRO, nao achou virgula entre os argumentos
				//erroSintatico(&token,&posicao);
				//continue;
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
		printf("token: %s isDef: %d lista: %d defNaLinha: %d isConst:%d isCt0 %d\n",buffer,tabelaDeSimbolos[i].isDef,tabelaDeSimbolos[i].lista,tabelaDeSimbolos[i].definidoNaLinha,tabelaDeSimbolos[i].isConst,tabelaDeSimbolos[i].isConst0);
		
	}

	//verifica se todos os simbolos da tabela de simbolos foram definidos

	for(i=0; i< tamanhoTabela; i++){
		if (!tabelaDeSimbolos[i].isDef){
			token.atribuiContaLinha(1);
			posicao = 0;
			//varre arquivo de texto imprimindo as posicoes onde o rotulo nao definido foi usado
			do {				
				posicao+=token.leUmToken(texto,posicao);
				if(tabelaDeSimbolos[i].token.comparaToken(token)){
					erroSintatico(&token,&posicao);
				}
			}while(token.tipo != FIM_DE_STR);
		}
	}

	//verifica se ha saltos para secoes invalidas
	token.atribuiContaLinha(1);
	posicao = 0;
	do {				
		posicao+=token.leUmToken(texto,posicao);
		if (token.tipo >= JMP && token.tipo <= JMPZ){
			do {				
				posicao+=token.leUmToken(texto,posicao);
			} while(token.tipo == ESPACO || token.tipo == TABULACAO);
			//varre toda a tabela, compara e ve se ta saltando pra secao errada
			for(i=0;i<tamanhoTabela;i++){
				if(tabelaDeSimbolos[i].token.comparaToken(token)){
					//ve se ta saltando pra secao boa
					if (tabelaDeSimbolos[i].definidoNaLinha > nLinhasText){
						//ERRO, SALTO PARA SECAO INVALIDA
						erroSemantico(&token,&posicao);
					}
				}
			}
		}
		// verifica se ha modificacao de constantes
		else if (token.tipo == STORE){
			do {			
				posicao+=token.leUmToken(texto,posicao);
			} while(token.tipo == ESPACO || token.tipo == TABULACAO);
			//varre toda a tabela, compara e ve se ta alterando valor de ctt
			for(i=0;i<tamanhoTabela;i++){
				if(tabelaDeSimbolos[i].token.comparaToken(token)){
					//ve se ta saltando pra secao boa
					if (tabelaDeSimbolos[i].isConst){
						//ERRO, SALTO PARA SECAO INVALIDA
						erroSemantico(&token,&posicao);
					}
				}
			}
		}
		else if (token.tipo == COPY){
			do {			
				posicao+=token.leUmToken(texto,posicao);
			} while(token.tipo == ESPACO || token.tipo == TABULACAO);
			posicao+=token.leUmToken(texto,posicao); //le virgula
			//varre toda a tabela, compara e ve se ta alterando valor de ctt
			if(token.tipo == VIRGULA){
				posicao+=token.leUmToken(texto,posicao);			
				for(i=0;i<tamanhoTabela;i++){
					if(tabelaDeSimbolos[i].token.comparaToken(token)){
						if (tabelaDeSimbolos[i].isConst){
							//ta maluco? ta tentando alterar const o que eh proibido por lei
							erroSemantico(&token,&posicao);
						}
					}
				}
			}
		}
		else if (token.tipo == DIV){
			do {			
				posicao+=token.leUmToken(texto,posicao);
			} while(token.tipo == ESPACO || token.tipo == TABULACAO);
			for(i=0;i<tamanhoTabela;i++){
				if(tabelaDeSimbolos[i].token.comparaToken(token)){
					if (tabelaDeSimbolos[i].isConst0){
						//ERRO, Divisao por zero aqui nao
						erroSemantico(&token,&posicao);
					}
				}
			}
			
		}
	}while(token.tipo != FIM_DE_STR);
	

	printf("\n\n"); 
	for(j=0;j<cursorExecutavel; j++) printf("%d ",programa[j]);
	printf("\n");


	return cursorExecutavel;
}













