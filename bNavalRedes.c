#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h> /* close() */
#include <string.h> /* memset() */

typedef int bool;
#define true 1
#define false 0
#define LOCAL_SERVER_PORT 8000
#define REMOTE_SERVER_PORT 8000
#define MAX_MSG 100

/**
 * Variaveis para as linhas e colunas do vetor, contador (dos dois jogadores)
 * em relação ao registro as frotas, escolha para opções que serão dadas
 * aos jogadores, contagem de acertos e vez dos jogadores, e dois tabuleiros
 * para identificação do tipo de nave da frota
 **/
int TabuleiroId[10][10], TabuleiroId2[10][10], Linha, Coluna, Contador, Contador2, Escolha, Quant, Quant1, Acertos, Acertos2, Vez;

/**
 * Tabuleiros dos jogadores 1 e 2, o tabuleiro criado e o tabuleiro que deve
 * ser desvendado
 */
char Tabuleiro[10][10], Tabuleiro2[10][10], TabuleiroEsc[10][10], TabuleiroEsc2[10][10], Prox;

/**
 * Imprime um tabuleiro exemplo
 * @param sentido true - vertical, false - horizontal
 */
void exemplo(bool sentido){
	int i, j;
	if (sentido == false){
		for(i = 0; i < 10; i++){
			for(j = 0; j < 10; j++){
				if(j != 9){
					printf("- ");
				}
				else{
					printf("- >\n");
				}
			}
		}
	}
	else{
		for(i = 0; i < 10; i++){
			for(j = 0; j < 10; j++){
				if(i != 9){
					printf("| ");
				}
				else{
					printf("V ");
				}
				if(j == 9){
					printf("\n");
				}
			}
		}
	}

}

/**
 * Insere o campo na tela
 * @param id do tabuleiro a ser impresso
 */
void imprimeCampo(int id){
	int i, j, k;
	printf("\n  ");
	for(k = 0; k < 10; k++){
		printf("%d ", k);
	}
	printf("\n");
	for(i = 0; i < 10; i++) {
		printf("%d ", i);
			for(j = 0; j < 10; j++) {
					if(id == 1) printf("%c ", Tabuleiro[i][j]);
					else if(id == 2) printf("%c ", Tabuleiro2[i][j]);
			}
		printf("\n");
	}
}

/**
 * confere se a posição inserida é valida
 * @param Linha
 * @param Coluna
 * @param tab
 * @return true caso posição válida
 */
bool confere(int Linha, int Coluna, char tab[10][10]){

	// Caso a posicao seja alguma fora das extremidades
	if((Coluna > 0 && Coluna < 9) && (Linha > 0 && Linha < 9)){
		if(tab[Linha][Coluna] != '#' && (tab[Linha + 1][Coluna] != '#' && tab[Linha][Coluna + 1] != '#' &&
		tab[Linha - 1][Coluna] != '#' && tab[Linha][Coluna - 1] != '#' && tab[Linha + 1][Coluna + 1] != '#' &&
		tab[Linha - 1][Coluna - 1] != '#' && tab[Linha + 1][Coluna - 1] != '#' && tab[Linha - 1][Coluna + 1] != '#')){
			return true;
		}
		else
			return false;
	}

	// Caso a posicao seja alguma nos cantos
	else if(Coluna == 0 && Linha == 0){
		if(tab[Linha][Coluna] != '#' && tab[Linha + 1][Coluna] != '#' &&
		   tab[Linha + 1][Coluna + 1] != '#' && tab[Linha][Coluna + 1] != '#'){
			return true;
		}
		else
			return false;
	}

	else if((Coluna == 9 && Linha == 9)){
		if(tab[Linha][Coluna] != '#' && tab[Linha - 1][Coluna] != '#' &&
		   tab[Linha - 1][Coluna - 1] != '#' && tab[Linha][Coluna - 1] != '#'){
			return true;
		   }
		else
			return false;
	}

	// Caso Linha esteja em um dos extremos (0 ou 9)
	else if(Linha == 0){
		if(tab[Linha][Coluna] != '#' && tab[Linha][Coluna - 1] != '#' && tab[Linha][Coluna + 1] != '#' &&
		   tab[Linha + 1][Coluna + 1] != '#' && tab[Linha + 1][Coluna] != '#' && tab[Linha + 1][Coluna - 1] != '#'){
			return true;
		}
		else
			return false;
	}

	else if(Linha == 9){
		if(tab[Linha][Coluna] != '#' && tab[Linha][Coluna - 1] != '#' && tab[Linha][Coluna + 1] != '#' &&
		   tab[Linha - 1][Coluna - 1] != '#'&& tab[Linha - 1][Coluna] != '#' && tab[Linha - 1][Coluna + 1] != '#'){
			return true;
		}
		else
			return false;
	}

	// Caso Coluna esteja em um dos extremos (0 ou 9)
	else if(Coluna == 0){
		if(tab[Linha][Coluna] != '#' && tab[Linha - 1][Coluna] != '#' && tab[Linha + 1][Coluna] != '#' &&
		   tab[Linha - 1][Coluna + 1] != '#' && tab[Linha][Coluna + 1] != '#' && tab[Linha + 1][Coluna + 1] != '#'){
			return true;
		}
		else
			return false;
	}

	else if(Coluna == 9){
		if(tab[Linha][Coluna] != '#' && tab[Linha - 1][Coluna] != '#' && tab[Linha + 1][Coluna] != '#' &&
		   tab[Linha - 1][Coluna - 1] != '#' && tab[Linha][Coluna - 1] != '#' && tab[Linha + 1][Coluna - 1] != '#'){
			return true;
		}
		else
			return false;
	}

	else
		return false;
}

/**
 * limpa ambos os tabuleiros
 */
void limpaTabuleiros(){
	for(int i = 0; i < 10; i++) {
    	for(int j = 0; j < 10; j++) {
        	Tabuleiro[i][j] = '.';
		      Tabuleiro2[i][j] = '.';
					TabuleiroId[i][j] = 0;
					TabuleiroId2[i][j] = 0;
    	}
	}
	for(int i = 0; i < 10; i++) {
	    for(int j = 0; j < 10; j++) {
	       	TabuleiroEsc2[i][j] = '?';
	    }
	}

	for(int i = 0; i < 10; i++) {
    	for(int j = 0; j < 10; j++) {
        	TabuleiroEsc[i][j] = '?';
    	}
	}
}

void inicializa(){
	Contador = 0;
	Contador2 = 0;
	Escolha = 0;
	Acertos = 0;
	Acertos2 = 0;
	Vez = 1;
}

void populaTabuleiro(){
	while(Contador < 4){ //Contador utilizado para inserir todos os submarinos no tabuleiro
		printf("Insira a posição dos submarinos (um por vez)\n");

		printf("\nInsira a coluna da posição desejada: ");
		scanf("%d", &Coluna); //Input da coluna desejada
		while(Coluna < 0 || Coluna > 9){
			//Evitar que jogador ultrapasse os valores permitidos pela matriz definida (10x10), fazendo-o inserir novamente ate ter um valor permitido
			printf("\nColuna fora dos limites do tabuleiro, favor inserir uma entre 0 e 9: ");
			scanf("%d", &Coluna);
		}

		printf("\nInsira a linha da posição desejada: ");
		scanf("%d", &Linha); //Input da linha desejada
		while(Linha < 0 || Linha > 9){
			//Evitar que jogador ultrapasse os valores permitidos pela matriz definida (10x10), fazendo-o inserir novamente ate ter um valor permitido
			printf("\nLinha fora dos limites do tabuleiro, favor inserir uma entre 0 e 9: ");
			scanf("%d", &Linha);
		}

		//Verificar se as posicoes em volta e na posicao desejada pelo jogador ja nao tenha outro objeto inserido pelo mesmo.
		if(confere(Linha, Coluna, Tabuleiro)){
			Tabuleiro[Linha][Coluna] = '#'; //modificar o valor da posicao permitida com uma hashtag (#)
			TabuleiroId[Linha][Coluna] = 1; //modifica o valor da posicao com um valor que identificara esse tipo de nave, no caso um submarino
			Contador++;
		}
		else{
			printf("\nPosição já ocupada ou proxima demais de outro objeto.\nManter ao menos uma casa de distância de qualquer outro objeto ja inserido.\n");
		}

		//Tabuleiro é atualizado com os novos objetos
		imprimeCampo(1);

	}

	while(Contador < 7){ //Contador utilizado para inserir todos os cruzadores no tabuleiro
		printf("\nInsira a posicao dos cruzadores (uma por vez)");

		printf("\nInsira a coluna da posicao desejada: ");
		scanf("%d", &Coluna);
		while(Coluna < 0 || Coluna > 9){
			printf("\nColuna fora dos limites do tabuleiro, favor inserir uma entre 0 e 9.");
			scanf("%d", &Coluna);
		}

		printf("\nInsira a linha da posicao desejada: ");
		scanf("%d", &Linha);
		while(Linha < 0 || Linha > 9){
			printf("\nLinha fora dos limites do tabuleiro, favor inserir uma entre 0 e 9.");
			scanf("%d", &Linha);
		}

		if(confere(Linha, Coluna, Tabuleiro)){
			Tabuleiro[Linha][Coluna] = '#';
			TabuleiroId[Linha][Coluna] = 2;
			Contador++;
			while(Escolha < 1 || Escolha > 5){ //Sistem de escolhas em qual direcao o jogador quer inserir o resto do objeto em questao ao inves
				//de inserir casa por casa.
				printf("\n\nPara completar seu cruzador voce tera a escolha de qual lado posiciona-lo. 1 - Cima, 2 - Baixo, 3 - Esquerda, 4 - Direita.");
				printf("\nCaso nao seja possivel posicionar o objeto com as opcoes anteriores ou queira cancelar a acao tecle 5.");
				printf("\nTenha em mente que teclar 5 REMOVERA o objeto incial de sua posicao: ");
				scanf("%d", &Escolha);
				if(Escolha == 1){ //Sera verificado se e possivel inserir as outras casas para cima.
					if(Linha - 1 < 0){
						printf("\nPosicao fora do tabuleiro. Por favor escolher outra opcao: ");
						Escolha = 0;
					}
					else if(Tabuleiro[Linha - 2][Coluna] != '#' && Tabuleiro[Linha - 1][Coluna - 1] != '#' &&
					Tabuleiro[Linha - 1][Coluna + 1] != '#' && Tabuleiro[Linha - 2][Coluna - 1] != '#' &&
					Tabuleiro[Linha - 2][Coluna + 1] != '#'){ //Sera verifica se nao ha objetos em volta ou na posicao desejada
						Tabuleiro[Linha - 1][Coluna] = '#'; //Sera efetuada as casas desejadas
						TabuleiroId[Linha - 1][Coluna] = 2; //Sera efetuada as casas desejadas
					}
					else{
						printf("\nPosicao proxima demais de outro objeto.\nManter ao menos uma casa de distancia de qualquer outro objeto ja inserido. Favor escolher outra opcao: ");
						Escolha = 0; //Escolha nao possivel devido ao objeto proximo, fara com que o jogador escolha outro posicionamento
					}
				}

				if(Escolha == 2){
					if(Linha + 1 > 9){//Sera verificado se e possivel inserir as outras casas para baixo.
						printf("\nPosicao fora do tabuleiro. Por favor escolher outra opcao.");
						Escolha = 0;
					}
					else if(Tabuleiro[Linha + 2][Coluna] != '#' && Tabuleiro[Linha + 1][Coluna + 1] != '#' &&
					Tabuleiro[Linha + 1][Coluna - 1] != '#' && Tabuleiro[Linha + 2][Coluna + 1] != '#' &&
					Tabuleiro[Linha + 2][Coluna - 1] != '#'){
						Tabuleiro[Linha + 1][Coluna] = '#';
						TabuleiroId[Linha + 1][Coluna] = 2;
					}
					else{
						printf("\nPosicao proxima demais de outro objeto.\nManter ao menos uma casa de distancia de qualquer outro objeto ja inserido: ");
						Escolha = 0;
					}
				}

				if(Escolha == 3){
					if(Coluna - 1 < 0){//Sera verificado se e possivel inserir as outras casas para esquerda.
						printf("\nPosicao fora do tabuleiro. Por favor escolher outra opcao.");
						Escolha = 0;
					}
					else if(Tabuleiro[Linha][Coluna - 2] != '#' && Tabuleiro[Linha + 1][Coluna - 1] != '#' &&
					Tabuleiro[Linha - 1][Coluna - 1] != '#' && Tabuleiro[Linha - 1][Coluna - 2] != '#' &&
					Tabuleiro[Linha + 1][Coluna - 2] != '#'){
						Tabuleiro[Linha][Coluna - 1] = '#';
						TabuleiroId[Linha][Coluna - 1] = 2;
					}
					else{
						printf("\nPosicao proxima demais de outro objeto.\nManter ao menos uma casa de distancia de qualquer outro objeto ja inserido: ");
						Escolha = 0;
					}
				}

				if(Escolha == 4){
					if(Coluna + 1 > 9){ //Sera verificado se e possivel inserir as outras casas para direita.
						printf("\nPosicao fora do tabuleiro. Por favor escolher outra opcao: ");
						Escolha = 0;
					}
					else if(Tabuleiro[Linha][Coluna + 2] != '#' && Tabuleiro[Linha + 1][Coluna + 1] != '#' &&
					Tabuleiro[Linha - 1][Coluna + 1] != '#' && Tabuleiro[Linha + 1][Coluna + 2] != '#' &&
					Tabuleiro[Linha + 1][Coluna + 2] != '#'){
						Tabuleiro[Linha][Coluna + 1] = '#';
						TabuleiroId[Linha][Coluna + 1] = 2;
					}
					else{
						printf("\nPosicao proxima demais de outro objeto.\nManter ao menos uma casa de distancia de qualquer outro objeto ja inserido: ");
						Escolha = 0;
					}
				}
				//Caso nao tenha sido possivel inseri com as outras escolhas
				//ou o jogador decidiu que nao era a melhor posicao, essa escolha permite o jogador remover o objeto ja efetuado e escolher outra posicao
				if(Escolha == 5){
					Tabuleiro[Linha][Coluna] = '.';
					TabuleiroId[Linha][Coluna] = 0;
					Contador--; //Contador e reduzido devido a remocao da posicao anteriormente inserida
					printf("Objeto removido.");
				}
				imprimeCampo(1);
			}
		}
		else{
			printf("\nPosicao ja ocupada ou proxima demais de outro objeto.\nManter ao menos uma casa de distancia de qualquer outro objeto ja inserido.\n");
			imprimeCampo(1);
		}

		//Escolha e resetada para ser usada novamente mais tarde
		Escolha = 0;

	}

	while(Contador < 9){ //Contador utilizado para inserir todos os encouraçados no tabuleiro
		printf("\nInsira a posicao dos encouracado (um por vez)");

		printf("\nInsira a coluna da posicao desejada: ");
		scanf("%d", &Coluna);
		while(Coluna < 0 || Coluna > 9){
			printf("\nColuna fora dos limites do tabuleiro, favor inserir uma entre 0 e 9: ");
			scanf("%d", &Coluna);
		}

		printf("\nInsira a linha da posicao desejada: ");
		scanf("%d", &Linha);
		while(Linha < 0 || Linha > 9){
			printf("\nLinha fora dos limites do tabuleiro, favor inserir uma entre 0 e 9: ");
			scanf("%d", &Linha);
		}

		if(confere(Linha, Coluna, Tabuleiro)){
			Tabuleiro[Linha][Coluna] = '#';
			TabuleiroId[Linha][Coluna] = 3;
			Contador++;
			while(Escolha < 1 || Escolha > 5){
				printf("\nPara completar seu cruzador voce tera a escolha de qual lado posiciona-lo. 1 - Cima, 2 - Baixo, 3 - Esquerda, 4 - Direita.");
				printf("\nCaso nao seja possivel posicionar o objeto com as opcoes anteriores ou queira cancelar a acao tecle 5.");
				printf("\nTenha em mente que teclar 5 REMOVERA o objeto incial de sua posicao: ");
				scanf("%d", &Escolha);
				if(Escolha == 1){
					if(Linha - 3 < 0){
						printf("\nPosicao fora do tabuleiro. Por favor escolher outra opcao.");
						Escolha = 0;
					}
					else if(Tabuleiro[Linha - 2][Coluna] != '#' && Tabuleiro[Linha - 3][Coluna] != '#' && Tabuleiro[Linha - 2][Coluna - 1] != '#' &&
					Tabuleiro[Linha - 2][Coluna + 1] != '#' && Tabuleiro[Linha - 3][Coluna - 1] != '#' && Tabuleiro[Linha - 3][Coluna + 1] != '#'){
						Tabuleiro[Linha - 1][Coluna] = '#';
						Tabuleiro[Linha - 2][Coluna] = '#';
						TabuleiroId[Linha - 1][Coluna] = 3;
						TabuleiroId[Linha - 2][Coluna] = 3;
					}
					else{
						printf("\nPosicao proxima demais de outro objeto.\nManter ao menos uma casa de distancia de qualquer outro objeto ja inserido.\
						Favor escolher outra opcao.");
						Escolha = 0;
					}
				}

				if(Escolha == 2){
					if(Linha + 3 > 9){
						printf("\nPosicao fora do tabuleiro. Por favor escolher outra opcao.");
						Escolha = 0;
					}
					else if(Tabuleiro[Linha + 2][Coluna] != '#' && Tabuleiro[Linha + 3][Coluna] != '#' && Tabuleiro[Linha + 2][Coluna + 1] != '#' &&
					Tabuleiro[Linha + 2][Coluna - 1] != '#' && Tabuleiro[Linha + 3][Coluna + 1] != '#' && Tabuleiro[Linha + 3][Coluna - 1] != '#'){
						Tabuleiro[Linha + 1][Coluna] = '#';
						Tabuleiro[Linha + 2][Coluna] = '#';
						TabuleiroId[Linha + 1][Coluna] = 3;
						TabuleiroId[Linha + 2][Coluna] = 3;
					}
					else{
						printf("\nPosicao proxima demais de outro objeto.\nManter ao menos uma casa de distancia de qualquer outro objeto ja inserido.");
						Escolha = 0;
					}
				}

				if(Escolha == 3){
					if(Coluna - 3 < 0){
						printf("\nPosicao fora do tabuleiro. Por favor escolher outra opcao.");
						Escolha = 0;
					}
					else if(Tabuleiro[Linha][Coluna - 2] != '#' && Tabuleiro[Linha][Coluna - 3] != '#' && Tabuleiro[Linha + 1][Coluna - 2] != '#' &&
					Tabuleiro[Linha - 1][Coluna - 2] != '#' && Tabuleiro[Linha - 1][Coluna - 3] != '#' && Tabuleiro[Linha + 1][Coluna - 3] != '#'){
						Tabuleiro[Linha][Coluna - 1] = '#';
						Tabuleiro[Linha][Coluna - 2] = '#';
						TabuleiroId[Linha][Coluna - 1] = 3;
						TabuleiroId[Linha][Coluna - 2] = 3;
					}
					else{
						printf("\nPosicao proxima demais de outro objeto.\nManter ao menos uma casa de distancia de qualquer outro objeto ja inserido.");
						Escolha = 0;
					}
				}

				if(Escolha == 4){
					if(Coluna + 3 > 9){
						printf("\nPosicao fora do tabuleiro. Por favor escolher outra opcao.");
						Escolha = 0;
					}
					else if(Tabuleiro[Linha][Coluna + 2] != '#' && Tabuleiro[Linha][Coluna + 3] != '#' && Tabuleiro[Linha + 1][Coluna + 2] != '#' &&
					Tabuleiro[Linha - 1][Coluna + 2] != '#' && Tabuleiro[Linha + 1][Coluna + 3] != '#' && Tabuleiro[Linha + 1][Coluna + 3] != '#'){
						Tabuleiro[Linha][Coluna + 1] = '#';
						Tabuleiro[Linha][Coluna + 2] = '#';
						TabuleiroId[Linha][Coluna + 1] = 3;
						TabuleiroId[Linha][Coluna + 2] = 3;
					}
					else{
						printf("\nPosicao proxima demais de outro objeto.\nManter ao menos uma casa de distancia de qualquer outro objeto ja inserido.");
						Escolha = 0;
					}
				}

				if(Escolha == 5){
					Tabuleiro[Linha][Coluna] = '.';
					TabuleiroId[Linha][Coluna] = 0;
					Contador--;
					printf("Objeto removido.");
				}
			}
		}
		else{
			printf("\nPosicao ja ocupada ou proxima demais de outro objeto.\nManter ao menos uma casa de distancia de qualquer outro objeto ja inserido.");
		}

		Escolha = 0;

		imprimeCampo(1);
	}

	while(Contador < 10){ //Contador utilizado para inserir o porta-avião no tabuleiro
		printf("\nInsira a posicao do porta-aviao");

		printf("\nInsira a coluna da posicao desejada: ");
		scanf("%d", &Coluna);
		while(Coluna < 0 || Coluna > 9){
			printf("\nColuna fora dos limites do tabuleiro, favor inserir uma entre 0 e 9.");
			scanf("%d", &Coluna);
		}

		printf("\nInsira a linha da posicao desejada: ");
		scanf("%d", &Linha);
		while(Linha < 0 || Linha > 9){
			printf("\nLinha fora dos limites do tabuleiro, favor inserir uma entre 0 e 9.");
			scanf("%d", &Linha);
		}


		if(confere(Linha, Coluna, Tabuleiro)){
			Tabuleiro[Linha][Coluna] = '#';
			TabuleiroId[Linha][Coluna] = 4;
			Contador++;
			while(Escolha < 1 || Escolha > 5){
				printf("\nPara completar seu cruzador voce tera a escolha de qual lado posiciona-lo. 1 - Cima, 2 - Baixo, 3 - Esquerda, 4 - Direita.");
				printf("\nCaso nao seja possivel posicionar o objeto com as opcoes anteriores ou queira cancelar a acao tecle 5.");
				printf("\nTenha em mente que teclar 5 REMOVERA o objeto incial de sua posicao: ");
				scanf("%d", &Escolha);
				if(Escolha == 1){
					if(Linha - 4 < 0){
						printf("\nPosicao fora do tabuleiro. Por favor escolher outra opcao.");
						Escolha = 0;
					}
					else if(Tabuleiro[Linha - 2][Coluna] != '#' && Tabuleiro[Linha - 3][Coluna] != '#' && Tabuleiro[Linha - 4][Coluna] != '#' &&
					Tabuleiro[Linha - 2][Coluna - 1] != '#' && Tabuleiro[Linha - 2][Coluna + 1] != '#' && Tabuleiro[Linha - 3][Coluna - 1] != '#' &&
					Tabuleiro[Linha - 3][Coluna + 1] != '#' && Tabuleiro[Linha - 4][Coluna - 1] != '#' && Tabuleiro[Linha - 4][Coluna + 1] != '#'){
						Tabuleiro[Linha - 1][Coluna] = '#';
						Tabuleiro[Linha - 2][Coluna] = '#';
						Tabuleiro[Linha - 3][Coluna] = '#';
						TabuleiroId[Linha - 1][Coluna] = 4;
						TabuleiroId[Linha - 2][Coluna] = 4;
						TabuleiroId[Linha - 3][Coluna] = 4;
					}
					else{
						printf("\nPosicao proxima demais de outro objeto.\nManter ao menos uma casa de distancia de qualquer outro objeto ja inserido. Favor escolher outra opcao.");
						Escolha = 0;
					}
				}

				if(Escolha == 2){
					if(Linha + 4 > 9){
						printf("\nPosicao fora do tabuleiro. Por favor escolher outra opcao.");
						Escolha = 0;
					}
					else if(Tabuleiro[Linha + 2][Coluna] != '#' && Tabuleiro[Linha + 3][Coluna] != '#' && Tabuleiro[Linha + 4][Coluna] != '#' && Tabuleiro[Linha + 2][Coluna + 1] != '#' && Tabuleiro[Linha + 2][Coluna - 1] != '#' && Tabuleiro[Linha + 3][Coluna + 1] != '#' && Tabuleiro[Linha + 3][Coluna - 1] != '#' && Tabuleiro[Linha + 4][Coluna - 1] != '#' && Tabuleiro[Linha + 4][Coluna + 1] != '#'){
						Tabuleiro[Linha + 1][Coluna] = '#';
						Tabuleiro[Linha + 2][Coluna] = '#';
						Tabuleiro[Linha + 3][Coluna] = '#';
						TabuleiroId[Linha + 1][Coluna] = 4;
						TabuleiroId[Linha + 2][Coluna] = 4;
						TabuleiroId[Linha + 3][Coluna] = 4;
					}
					else{
						printf("\nPosicao proxima demais de outro objeto.\nManter ao menos uma casa de distancia de qualquer outro objeto ja inserido.");
						Escolha = 0;
					}
				}

				if(Escolha == 3){
					if(Coluna - 4 < 0){
						printf("\nPosicao fora do tabuleiro. Por favor escolher outra opcao.");
						Escolha = 0;
					}
					else if(Tabuleiro[Linha][Coluna - 2] != '#' && Tabuleiro[Linha][Coluna - 3] != '#' && Tabuleiro[Linha][Coluna - 4] != '#' && Tabuleiro[Linha + 1][Coluna - 2] != '#' && Tabuleiro[Linha - 1][Coluna - 2] != '#' && Tabuleiro[Linha - 1][Coluna - 3] != '#' && Tabuleiro[Linha + 1][Coluna - 3] != '#' && Tabuleiro[Linha - 1][Coluna - 4] != '#' && Tabuleiro[Linha + 1][Coluna - 4] != '#'){
						Tabuleiro[Linha][Coluna - 1] = '#';
						Tabuleiro[Linha][Coluna - 2] = '#';
						Tabuleiro[Linha][Coluna - 3] = '#';
						TabuleiroId[Linha][Coluna - 1] = 4;
						TabuleiroId[Linha][Coluna - 2] = 4;
						TabuleiroId[Linha][Coluna - 3] = 4;
					}
					else{
						printf("\nPosicao proxima demais de outro objeto.\nManter ao menos uma casa de distancia de qualquer outro objeto ja inserido.");
						Escolha = 0;
					}
				}

				if(Escolha == 4){
					if(Coluna + 4 > 9){
						printf("\nPosicao fora do tabuleiro. Por favor escolher outra opcao.");
						Escolha = 0;
					}
					else if(Tabuleiro[Linha][Coluna + 2] != '#' && Tabuleiro[Linha][Coluna + 3] != '#' && Tabuleiro[Linha][Coluna + 4] != '#' && Tabuleiro[Linha + 1][Coluna + 2] != '#' && Tabuleiro[Linha - 1][Coluna + 2] != '#' && Tabuleiro[Linha + 1][Coluna + 3] != '#' && Tabuleiro[Linha + 1][Coluna + 3] != '#' && Tabuleiro[Linha - 1][Coluna + 4] != '#' && Tabuleiro[Linha + 1][Coluna + 4] != '#'){
					Tabuleiro[Linha][Coluna + 1] = '#';
					Tabuleiro[Linha][Coluna + 2] = '#';
					Tabuleiro[Linha][Coluna + 3] = '#';
					TabuleiroId[Linha][Coluna + 1] = 4;
					TabuleiroId[Linha][Coluna + 2] = 4;
					TabuleiroId[Linha][Coluna + 3] = 4;
					}
					else{
						printf("\nPosicao proxima demais de outro objeto.\nManter ao menos uma casa de distancia de qualquer outro objeto ja inserido.");
						Escolha = 0;
					}
				}
				if(Escolha == 5){
					Tabuleiro[Linha][Coluna] = '.';
					TabuleiroId[Linha][Coluna] = 0;
					Contador--;
					printf("Objeto removido.");
				}
			}
		}
		else{
			printf("\nPosicao ja ocupada ou proxima demais de outro objeto.\nManter ao menos uma casa de distancia de qualquer outro objeto ja inserido.\n");
			imprimeCampo(1);
		}
		Escolha = 0;
	}
}

int main(){
    int kind;
    int servSock, sd, rc, n, cliLen, i;
  	struct sockaddr_in cliAddr, servAddr, remoteServAddr;
  	char msg[MAX_MSG];
  	struct hostent *h;

  	inicializa();
  	limpaTabuleiros();



    printf("Selecione a opcao de funcionamento:\n \t 1 - Server \n\t 2 - Client\n\n_");
    scanf("%d", &kind);
    if(kind == 1){//server stuff

		  servSock=socket(AF_INET, SOCK_DGRAM, 0);
		  if(servSock<0) {
		    printf("%s: cannot open socket \n","argv[0]");
		  //  exit(0);
		  }

		  /* bind local server port */
		  servAddr.sin_family = AF_INET;
		  servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
		  servAddr.sin_port = htons(LOCAL_SERVER_PORT);
		  rc = bind (servSock, (struct sockaddr *) &servAddr, sizeof(servAddr));
		  if(rc<0) {
		    printf("%s: cannot bind port number %d \n",
			   "argv[0]", LOCAL_SERVER_PORT);
		//    exit(0);
		  }

		  printf("%s: waiting for data on port UDP %u\n",
			   "argv[0]",LOCAL_SERVER_PORT);

		 // populaTabuleiro();

		  /* server infinite loop */
		  while(1) {

		    /* init buffer */
		    memset(msg,0x0,MAX_MSG);


		    /* receive message */
		    cliLen = sizeof(cliAddr);
		    n = recvfrom(servSock, msg, MAX_MSG, 0,
				 (struct sockaddr *) &cliAddr, &cliLen);

		    if(n<0) {
		      printf("%s: cannot receive data \n","argv[0]");
		      continue;
		    }

		    /* print received message */
		    printf("%s: from %s:UDP%u : %s \n",
			   "argv[0]",inet_ntoa(cliAddr.sin_addr),
			   ntohs(cliAddr.sin_port),msg);

		    /**
		     * AQUI EU RETORNO A BUCETUDA DA MENSAGEM PRO CLIENTE
		     * PRECISA TRABALHAR AGORA PRA ESSA MENSAGEM TER CONTEXTO
		     * VOU PENSAR EM UM FORMATO E UM PARSER.....
		     *
		     */
		    n = sendto(servSock, msg, strlen(msg), 0, (struct sockaddr *)&cliAddr, sizeof(cliAddr));

		    if(n<0){
		    	printf("Could't send back data");
		    	continue;
		    }



		  }/* end of server infinite loop */

    }
    else if(kind == 2){//client stuff

    	h = gethostbyname("127.0.0.1");
		  if(h==NULL) {
		    printf("%s: unknown host '%s' \n", "argv[0]", "argv[1]");
		//    exit(1);
		  }

		  printf("%s: sending data to '%s' (IP : %s) \n", "argv[0]", h->h_name,
			 inet_ntoa(*(struct in_addr *)h->h_addr_list[0]));

		  remoteServAddr.sin_family = h->h_addrtype;
		  memcpy((char *) &remoteServAddr.sin_addr.s_addr,
			 h->h_addr_list[0], h->h_length);
		  remoteServAddr.sin_port = htons(REMOTE_SERVER_PORT);

		  /* socket creation */
		  sd = socket(AF_INET,SOCK_DGRAM,0);
		  if(sd<0) {
		    printf("%s: cannot open socket \n","argv[0]");
		//    exit(1);
		  }

		  /* bind any port */
		  cliAddr.sin_family = AF_INET;
		  cliAddr.sin_addr.s_addr = htonl(INADDR_ANY);
		  cliAddr.sin_port = htons(0);

		  rc = bind(sd, (struct sockaddr *) &cliAddr, sizeof(cliAddr));
		  if(rc<0) {
		    printf("%s: cannot bind port\n", "argv[0]");
		  //  exit(1);
		  }


		  /**
		   * OLHA AQUI SEUS FILHO DE QUENGA
		   * COMO ESSE Ë A PORRA DO CLIENTE ELE ENVIA E DEPOIS RECEBE
		   * NÃO SEI SE ESSA ORDEM INFLUENCIA MAS PRA MIM FAZ SENTIDO ASSSIM
		   */

		  /* send data */
		  for(i = 0;;i++) {
		    rc = sendto(sd, "Wololo", strlen("Wololo")+1, 0,
				(struct sockaddr *) &remoteServAddr,
				sizeof(remoteServAddr));

		    if(rc<0) {
		      printf("%s: cannot send data %d \n","argv[0]",i-1);
		      close(sd);
		  //    exit(1);
		    }

		    /**
		     * TOU USANDO A CARALHA DO cliLen MAS Ë O TAMANHO DO ENDERECO
		     * DO FILHADAPUTA DO SERVIDOR COMO TA NO MESMO ARQUIVO ACHO
		     * QUE DARIA PRA USAR A MESMA VARIÁVEL QUE O SERVADDR
		     * MAS NAO SE MECHE EM TIME QUE TA GANHANDO
		     */
		    cliLen = sizeof(remoteServAddr);
		    rc = recvfrom(sd, msg, MAX_MSG, 0, (struct sockaddr *) &remoteServAddr, &cliLen);

		    printf("%s: from %s:UDP%u : %s size: %d\n",
			   "argv[0]",inet_ntoa(remoteServAddr.sin_addr),
			   ntohs(remoteServAddr.sin_port),msg, rc);

		  }

    }
    else{
        printf("Opção inválida!");
    }
}
