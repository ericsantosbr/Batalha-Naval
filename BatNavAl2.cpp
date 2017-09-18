/*
 * Projeto desenvolvido por Bruno Rocha e Eric Santos como parte de avaliacao de uma materia da Fatec Sao Caetano
 *
 * Bruno Rocha - bru2014@gmail.com
 * Eric Santos - ericsantosbr2@gmail.com
 *
 * FIXME:
 * 1 - Barcos que nao aceitam ser inseridos em pontos MUITO especificos
 *
 * TODO:
 * 1 - Corrigir a possibilidade de inseir outro objeto na coluna 9 caso tenha um objeto na coluna 0 da mesma linha (Erros ocasionados pelas condicionais das diagonais)
 * 2 - Identificar qual objeto e ao afundar (Ja funcional, no entanto em alguns casos ainda nao e identificado)
 * 3 - Optimizar o codigo com funcoes
 * 4 - Nao fazer a geracao do navio se perder caso o jogador insira uma casa proxima de uma ja ocupada e sim repetir o processo (Verificar se esse erro ainda ocorre)
*/

#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>

/*Variaveis para as linhas e colunas do vetor, contador (dos dois jogadores) em relação ao registro as frotas, escolha para opções que serão dadas
aos jogadores, contagem de acertos e vez dos jogadores, e dois tabuleiros para identificação do tipo de nave da frota */
int TabuleiroId[10][10], TabuleiroId2[10][10], Linha, Coluna, Contador, Contador2, Escolha, Quant, Quant1, Acertos, Acertos2, Vez;
//Tabuleiros dos jogadores 1 e 2, o tabuleiro criado e o tabuleiro que deve ser desvendado
char Tabuleiro[10][10], Tabuleiro2[10][10], TabuleiroEsc[10][10], TabuleiroEsc2[10][10], Prox;


// Imprime o tabuleiro de exemplo
// True para vertical, false para horizontal
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
void imprimeCampo(){
	int i, j, k;
	printf("\n  ");
	for(k = 0; k < 10; k++){
		printf("%d ", k);
	}
	printf("\n");
	for(i = 0; i < 10; i++) {
		printf("%d ", i);
			for(j = 0; j < 10; j++) {
					printf("%c ", Tabuleiro[i][j]);
			}
		printf("\n");
	}
}
bool confere(int Linha, int Coluna, char tab[10][10]){
	if(tab[Linha][Coluna] != '#' && (tab[Linha + 1][Coluna] != '#' && tab[Linha][Coluna + 1] != '#' &&
	tab[Linha - 1][Coluna] != '#' && tab[Linha][Coluna - 1] != '#' && tab[Linha + 1][Coluna + 1] != '#' &&
	tab[Linha - 1][Coluna - 1] != '#' && tab[Linha + 1][Coluna - 1] != '#' && tab[Linha - 1][Coluna + 1] != '#')){
		return true;
	}
	else
		return false;
}

int main(){
	system("clear");

	//Definindo valor de algumas variaveis abaixo
	Contador = 0;
	Contador2 = 0;
	Escolha = 0;
	Acertos = 0;
	Acertos2 = 0;
	Vez = 1;

	// Limpeza dos Tabuleiros
	for(int i = 0; i < 10; i++) {
    	for(int j = 0; j < 10; j++) {
        	Tabuleiro[i][j] = '.';
		      Tabuleiro2[i][j] = '.';
					TabuleiroId[i][j] = 0;
					TabuleiroId2[i][j] = 0;
    	}
	}

	//Inicio do código utilizado para as ações do jogador 1 em relação a inserir a frota em seu tabuleiro
	printf("Vez do jogador 1\n");
	imprimeCampo();

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
		imprimeCampo();

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
						printf("\nPosicao proxima de mais de outro objeto.\nManter ao menos uma casa de distancia de qualquer outro objeto ja inserido. Favor escolher outra opcao: ");
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
						printf("\nPosicao proxima de mais de outro objeto.\nManter ao menos uma casa de distancia de qualquer outro objeto ja inserido: ");
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
				imprimeCampo();
			}
		}
		else{
			printf("\nPosicao ja ocupada ou proxima de mais de outro objeto.\nManter ao menos uma casa de distancia de qualquer outro objeto ja inserido.\n");
			imprimeCampo();
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
						printf("\nPosicao proxima de mais de outro objeto. Manter ao menos uma casa de dist�ncia de qualquer outro objeto ja inserido.\
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
						printf("\nPosicao proxima de mais de outro objeto. Manter ao menos uma casa de distancia de qualquer outro objeto ja inserido.");
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
						printf("\nPosicao proxima de mais de outro objeto. Manter ao menos uma casa de distancia de qualquer outro objeto ja inserido.");
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
						printf("\nPosicao proxima de mais de outro objeto. Manter ao menos uma casa de distancia de qualquer outro objeto ja inserido.");
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
			printf("\nPosicao ja ocupada ou proxima de mais de outro objeto. Manter ao menos uma casa de distancia de qualquer outro objeto ja inserido.");
		}

		Escolha = 0;

		imprimeCampo();
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
				printf("\nTenha em mente que teclar 5 REMOVERA o objeto incial de sua posicao.");
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
						printf("\nPosicao proxima de mais de outro objeto.\nManter ao menos uma casa de distancia de qualquer outro objeto ja inserido.");
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
			imprimeCampo();
		}
		Escolha = 0;

	}

	//Aqui encerra a parte do código de inserir a frota do jogador 1

	//Inicio do código utilizado para as ações do jogador 2 em relação a inserir a frota em seu tabuleiro
	//Basicamente a mesma coisa com diferenca de tabuleiro e contador diferente
	printf("Vez do jogador 2");

	while(Contador2 < 4){
		printf("\nInsira a posicao dos submarinos (um por vez)");

		printf("Insira a coluna da posicao desejada: ");
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

		if(confere(Linha, Coluna, Tabuleiro2)){
			Tabuleiro2[Linha][Coluna] = '#';
			TabuleiroId2[Linha][Coluna] = 1;
			Contador2++;
		}
		else{
			printf("\nPosicao ja ocupada ou proxima demais de outro objeto.\nManter ao menos uma casa de distancia de qualquer outro objeto ja inserido.\n");
		}

		imprimeCampo();
	}

	while(Contador2 < 7){
		printf("\nInsira a posicao dos cruzadores (um por vez)");

		printf("Insira a coluna da posica desejada: ");
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

		if(confere(Linha, Coluna, Tabuleiro2)){
			Tabuleiro2[Linha][Coluna] = '#';
			TabuleiroId2[Linha][Coluna] = 2;
			Contador2++;
			while(Escolha < 1 || Escolha > 5){
				printf("\nPara completar seu cruzador voce tere a escolha de qual lado posiciona-lo. 1 - Cima, 2 - Baixo, 3 - Esquerda, 4 - Direita.");
				printf("\nCaso nao seja possivel posicionar o objeto com as opcoes anteriores ou queira cancelar a acao tecle 5.");
				printf("\nTenha em mente que teclar 5 REMOVERA o objeto incial de sua posicao: ");
				scanf("%d", &Escolha);
				if(Escolha == 1){
					if(Linha - 2 < 0){
						printf("\nPosicao fora do tabuleiro. Por favor escolher outra opcao.");
						Escolha = 0;
					}
					else if(Tabuleiro2[Linha - 2][Coluna] != '#' && Tabuleiro2[Linha - 1][Coluna - 1] != '#' && Tabuleiro2[Linha - 1][Coluna + 1] != '#' && Tabuleiro2[Linha - 2][Coluna - 1] != '#' && Tabuleiro2[Linha - 2][Coluna + 1] != '#'){
						Tabuleiro2[Linha - 1][Coluna] = '#';
						TabuleiroId2[Linha - 1][Coluna] = 2;
					}
					else{
						printf("\nPosicao proxima demais de outro objeto.\nManter ao menos uma casa de distancia de qualquer outro objeto ja inserido.\nFavor escolher outra opcao.");
						Escolha = 0;
					}
				}

				if(Escolha == 2){
					if(Linha + 2 > 9){
						printf("\nPosicao fora do tabuleiro. Por favor escolher outra opcao.");
						Escolha = 0;
					}
					else if(Tabuleiro2[Linha + 2][Coluna] != '#' && Tabuleiro2[Linha + 1][Coluna + 1] != '#' && Tabuleiro2[Linha + 1][Coluna - 1] != '#' && Tabuleiro2[Linha + 2][Coluna + 1] != '#' && Tabuleiro2[Linha + 2][Coluna - 1] != '#'){
						Tabuleiro2[Linha + 1][Coluna] = '#';
						TabuleiroId2[Linha + 1][Coluna] = 2;
					}
					else{
						printf("\nPosicao proxima demais de outro objeto.\nManter ao menos uma casa de distancia de qualquer outro objeto ja inserido.");
						Escolha = 0;
					}
				}

				if(Escolha == 3){
					if(Coluna - 2 < 0){
						printf("\nPosicao fora do tabuleiro. Por favor escolher outra opcao.");
						Escolha = 0;
					}
					else if(Tabuleiro2[Linha][Coluna - 2] != '#' && Tabuleiro2[Linha + 1][Coluna - 1] != '#' && Tabuleiro2[Linha - 1][Coluna - 1] != '#' && Tabuleiro2[Linha - 1][Coluna - 2] != '#' && Tabuleiro2[Linha + 1][Coluna - 2] != '#'){
						Tabuleiro2[Linha][Coluna - 1] = '#';
						TabuleiroId2[Linha][Coluna - 1] = 2;
					}
					else{
						printf("\nPosicao proxima de mais de outro objeto.\nManter ao menos uma casa de distancia de qualquer outro objeto ja inserido.");
						Escolha = 0;
					}
				}

				if(Escolha == 4){
					if(Coluna + 2 > 9){
						printf("\nPosicao fora do tabuleiro. Por favor escolher outra opcao.");
						Escolha = 0;
					}
					if(Tabuleiro2[Linha][Coluna + 2] != '#' && Tabuleiro2[Linha + 1][Coluna + 1] != '#' && Tabuleiro2[Linha - 1][Coluna + 1] != '#' && Tabuleiro2[Linha + 1][Coluna + 2] != '#' && Tabuleiro2[Linha + 1][Coluna + 2] != '#'){
						Tabuleiro2[Linha][Coluna + 1] = '#';
						TabuleiroId2[Linha][Coluna + 1] = 2;
					}
					else{
						printf("\nPosicao proxima de mais de outro objeto. Manter ao menos uma casa de distancia de qualquer outro objeto ja inserido.");
						Escolha = 0;
					}
				}

				if(Escolha == 5){
					Tabuleiro2[Linha][Coluna] = '.';
					TabuleiroId2[Linha][Coluna] = 0;
					Contador2--;
					printf("Objeto removido.");
				}
			}
		}
		else{
			printf("\nPosicao ja ocupada ou proxima de mais de outro objeto.\nManter ao menos uma casa de distancia de qualquer outro objeto ja inserido.\n");
		}

		Escolha = 0;
		imprimeCampo();
	}

	while(Contador2 < 9){
		printf("\nInsira a posicao dos encouracado (um por vez)");

		printf("Insira a coluna da posicao desejada: ");
		scanf("%d", &Coluna);
		while(Coluna < 0 || Coluna > 9){
			printf("\nColuna fora dos limites do tabuleiro, favor inserir uma entre 0 e 9: ");
			scanf("%d", &Coluna);
		}

		printf("\nInsira a linha da posicao desejada");
		scanf("%d", &Linha);
		while(Linha < 0 || Linha > 9){
			printf("\nLinha fora dos limites do tabuleiro, favor inserir uma entre 0 e 9: ");
			scanf("%d", &Linha);
		}

		if(Tabuleiro2[Linha][Coluna] != '#' && (Tabuleiro2[Linha + 1][Coluna] != '#' && Tabuleiro2[Linha][Coluna + 1] != '#' &&
		Tabuleiro2[Linha - 1][Coluna] != '#' && Tabuleiro2[Linha][Coluna - 1] != '#' && Tabuleiro2[Linha + 1][Coluna + 1] != '#' &&
		Tabuleiro2[Linha - 1][Coluna - 1] != '#' && Tabuleiro2[Linha + 1][Coluna - 1] != '#' && Tabuleiro2[Linha + 1][Coluna - 1] != '#')){
			Tabuleiro2[Linha][Coluna] = '#';
			TabuleiroId[Linha][Coluna] = 3;
			Contador2++;
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
					else if(Tabuleiro2[Linha - 2][Coluna] != '#' && Tabuleiro2[Linha - 3][Coluna] != '#' &&
					Tabuleiro2[Linha - 2][Coluna - 1] != '#' && Tabuleiro2[Linha - 2][Coluna + 1] != '#' &&
					Tabuleiro2[Linha - 3][Coluna - 1] != '#' && Tabuleiro2[Linha - 3][Coluna + 1] != '#'){
						Tabuleiro2[Linha - 1][Coluna] = '#';
						Tabuleiro2[Linha - 2][Coluna] = '#';
						TabuleiroId2[Linha - 1][Coluna] = 3;
						TabuleiroId2[Linha - 2][Coluna] = 3;
					}
					else{
						printf("\nPosicao proxima de mais de outro objeto.\nManter ao menos uma casa de distancia de qualquer outro objeto ja inserido. Favor escolher outra opcao: ");
						Escolha = 0;
					}
				}

				if(Escolha == 2){
					if(Linha + 3 > 9){
						printf("\nPosicao fora do tabuleiro. Por favor escolher outra opcao.");
						Escolha = 0;
					}
					else if(Tabuleiro2[Linha + 2][Coluna] != '#' && Tabuleiro2[Linha + 3][Coluna] != '#' && Tabuleiro2[Linha + 2][Coluna + 1] != '#' && Tabuleiro2[Linha + 2][Coluna - 1] != '#' && Tabuleiro2[Linha + 3][Coluna + 1] != '#' && Tabuleiro2[Linha + 3][Coluna - 1] != '#'){
						Tabuleiro2[Linha + 1][Coluna] = '#';
						Tabuleiro2[Linha + 2][Coluna] = '#';
						TabuleiroId2[Linha + 1][Coluna] = 3;
						TabuleiroId2[Linha + 2][Coluna] = 3;
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
					else if(Tabuleiro2[Linha][Coluna - 2] != '#' && Tabuleiro2[Linha][Coluna - 3] != '#' && Tabuleiro2[Linha + 1][Coluna - 2] != '#' && Tabuleiro2[Linha - 1][Coluna - 2] != '#' && Tabuleiro2[Linha - 1][Coluna - 3] != '#' && Tabuleiro2[Linha + 1][Coluna - 3] != '#'){
						Tabuleiro2[Linha][Coluna - 1] = '#';
						Tabuleiro2[Linha][Coluna - 2] = '#';
						TabuleiroId2[Linha][Coluna - 1] = 3;
						TabuleiroId2[Linha][Coluna - 2] = 3;
					}
					else{
						printf("\nPosicao proxima de mais de outro objeto.\nManter ao menos uma casa de distancia de qualquer outro objeto ja inserido.");
						Escolha = 0;
					}
				}

				if(Escolha == 4){
					if(Coluna + 3 > 9){
						printf("\nPosicao fora do tabuleiro. Por favor escolher outra opcao.");
						Escolha = 0;
					}
					else if(Tabuleiro2[Linha][Coluna + 2] != '#' && Tabuleiro2[Linha][Coluna + 3] != '#' && Tabuleiro2[Linha + 1][Coluna + 2] != '#' && Tabuleiro2[Linha - 1][Coluna + 2] != '#' && Tabuleiro2[Linha + 1][Coluna + 3] != '#' && Tabuleiro2[Linha + 1][Coluna + 3] != '#'){
						Tabuleiro2[Linha][Coluna + 1] = '#';
						Tabuleiro2[Linha][Coluna + 2] = '#';
						TabuleiroId2[Linha][Coluna + 1] = 3;
						TabuleiroId2[Linha][Coluna + 2] = 3;
					}
					else{
						printf("\nPosicao proxima de mais de outro objeto.\nManter ao menos uma casa de distancia de qualquer outro objeto ja inserido.");
						Escolha = 0;
					}
				}
				if(Escolha == 5){
					Tabuleiro2[Linha][Coluna] = '.';
					TabuleiroId2[Linha][Coluna - 1] = 0;
					Contador2--;
					printf("Objeto removido.");
				}
			}
		}
		else{
			printf("\nPosicao ja ocupada ou proxima de mais de outro objeto.\nManter ao menos uma casa de distancia de qualquer outro objeto ja inserido.");
		}

		Escolha = 0;

		imprimeCampo();

	}

	while(Contador2 < 10){
		printf("\nInsira a posicao do porta-aviao");
		printf("\nInsira a linha da posicao desejada: ");
		scanf("%d", &Linha);
		while(Linha < 0 || Linha > 9){
			printf("\nLinha fora dos limites do tabuleiro, favor inserir uma entre 0 e 9: ");
			scanf("%d", &Linha);
		}
		printf("Insira a coluna da posicao desejada: ");
		scanf("%d", &Coluna);
		while(Coluna < 0 || Coluna > 9){
			printf("\nColuna fora dos limites do tabuleiro, favor inserir uma entre 0 e 9: ");
			scanf("%d", &Coluna);
		}

		if(Tabuleiro2[Linha][Coluna] != '#' && (Tabuleiro2[Linha + 1][Coluna] != '#' && Tabuleiro2[Linha][Coluna + 1] != '#' &&
		Tabuleiro2[Linha - 1][Coluna] != '#' && Tabuleiro2[Linha][Coluna - 1] != '#' && Tabuleiro2[Linha + 1][Coluna + 1] != '#' &&
		Tabuleiro2[Linha - 1][Coluna - 1] != '#' && Tabuleiro2[Linha + 1][Coluna - 1] != '#' && Tabuleiro2[Linha + 1][Coluna - 1] != '#')){
			Tabuleiro2[Linha][Coluna] = '#';
			TabuleiroId2[Linha][Coluna] = 4;
			Contador2++;
			while(Escolha < 1 || Escolha > 5){
				printf("\nPara completar seu cruzador voc� ter� a escolha de qual lado posiciona-lo. 1 - Cima, 2 - Baixo, 3 - Esquerda, 4 - Direita.");
				printf("\nCaso n�o seja possivel posicionar o objeto com as op��es anteriores ou queira cancelar a a��o tecle 5.");
				printf("\nTenha em mente que teclar 5 REMOVERA o objeto incial de sua posi��o.");
				scanf("%d", &Escolha);
				if(Escolha == 1){
					if(Linha - 4 < 0){
						printf("\nPosi��o fora do tabuleiro. Por favor escolher outra op��o.");
						Escolha = 0;
					}
					else if(Tabuleiro2[Linha - 2][Coluna] != '#' && Tabuleiro2[Linha - 3][Coluna] != '#' && Tabuleiro2[Linha - 4][Coluna] != '#' && Tabuleiro2[Linha - 2][Coluna - 1] != '#' && Tabuleiro2[Linha - 2][Coluna + 1] != '#' && Tabuleiro2[Linha - 3][Coluna - 1] != '#' && Tabuleiro2[Linha - 3][Coluna + 1] != '#' && Tabuleiro2[Linha - 4][Coluna - 1] != '#' && Tabuleiro2[Linha - 4][Coluna + 1] != '#'){
						Tabuleiro2[Linha - 1][Coluna] = '#';
						Tabuleiro2[Linha - 2][Coluna] = '#';
						Tabuleiro2[Linha - 3][Coluna] = '#';
						TabuleiroId2[Linha - 1][Coluna] = 4;
						TabuleiroId2[Linha - 2][Coluna] = 4;
						TabuleiroId2[Linha - 3][Coluna] = 4;
					}
					else{
						printf("\nPosi��o proxima de mais de outro objeto. Manter ao menos uma casa de dist�ncia de qualquer outro objeto ja inserido. Favor escolher outra op��o.");
						Escolha = 0;
					}
				}

				if(Escolha == 2){
					if(Linha + 4 > 9){
						printf("\nPosi��o fora do tabuleiro. Por favor escolher outra op��o.");
						Escolha = 0;
					}
					else if(Tabuleiro2[Linha + 2][Coluna] != '#' && Tabuleiro2[Linha + 3][Coluna] != '#' && Tabuleiro2[Linha + 4][Coluna] != '#' && Tabuleiro2[Linha + 2][Coluna + 1] != '#' && Tabuleiro2[Linha + 2][Coluna - 1] != '#' && Tabuleiro2[Linha + 3][Coluna + 1] != '#' && Tabuleiro2[Linha + 3][Coluna - 1] != '#' && Tabuleiro2[Linha + 4][Coluna - 1] != '#' && Tabuleiro2[Linha + 4][Coluna + 1] != '#'){
						Tabuleiro2[Linha + 1][Coluna] = '#';
						Tabuleiro2[Linha + 2][Coluna] = '#';
						Tabuleiro2[Linha + 3][Coluna] = '#';
						TabuleiroId2[Linha + 1][Coluna] = 4;
						TabuleiroId2[Linha + 2][Coluna] = 4;
						TabuleiroId2[Linha + 3][Coluna] = 4;
					}
					else{
						printf("\nPosi��o proxima de mais de outro objeto. Manter ao menos uma casa de dist�ncia de qualquer outro objeto ja inserido.");
						Escolha = 0;
					}
				}

				if(Escolha == 3){
					if(Coluna - 4 < 0){
						printf("\nPosi��o fora do tabuleiro. Por favor escolher outra op��o.");
						Escolha = 0;
					}
					else if(Tabuleiro2[Linha][Coluna - 2] != '#' && Tabuleiro2[Linha][Coluna - 3] != '#' && Tabuleiro2[Linha][Coluna - 4] != '#' && Tabuleiro2[Linha + 1][Coluna - 2] != '#' && Tabuleiro2[Linha - 1][Coluna - 2] != '#' && Tabuleiro2[Linha - 1][Coluna - 3] != '#' && Tabuleiro2[Linha + 1][Coluna - 3] != '#' && Tabuleiro2[Linha - 1][Coluna - 4] != '#' && Tabuleiro2[Linha + 1][Coluna - 4] != '#'){
						Tabuleiro2[Linha][Coluna - 1] = '#';
						Tabuleiro2[Linha][Coluna - 2] = '#';
						Tabuleiro2[Linha][Coluna - 3] = '#';
						TabuleiroId2[Linha][Coluna - 1] = 4;
						TabuleiroId2[Linha][Coluna - 2] = 4;
						TabuleiroId2[Linha][Coluna - 3] = 4;
					}
					else{
						printf("\nPosi��o proxima de mais de outro objeto. Manter ao menos uma casa de dist�ncia de qualquer outro objeto ja inserido.");
						Escolha = 0;
					}
				}

				if(Escolha == 4){
					if(Coluna + 4 > 9){
						printf("\nPosi��o fora do tabuleiro. Por favor escolher outra op��o.");
						Escolha = 0;
					}
					else if(Tabuleiro2[Linha][Coluna + 2] != '#' && Tabuleiro2[Linha][Coluna + 3] != '#' && Tabuleiro2[Linha][Coluna + 4] != '#' && Tabuleiro2[Linha + 1][Coluna + 2] != '#' && Tabuleiro2[Linha - 1][Coluna + 2] != '#' && Tabuleiro2[Linha + 1][Coluna + 3] != '#' && Tabuleiro2[Linha + 1][Coluna + 3] != '#' && Tabuleiro2[Linha - 1][Coluna + 4] != '#' && Tabuleiro2[Linha + 1][Coluna + 4] != '#'){
					Tabuleiro2[Linha][Coluna + 1] = '#';
					Tabuleiro2[Linha][Coluna + 2] = '#';
					Tabuleiro2[Linha][Coluna + 3] = '#';
					TabuleiroId2[Linha][Coluna + 1] = 4;
					TabuleiroId2[Linha][Coluna + 2] = 4;
					TabuleiroId2[Linha][Coluna + 3] = 4;
					}
					else{
						printf("\nPosi��o proxima de mais de outro objeto. Manter ao menos uma casa de dist�ncia de qualquer outro objeto ja inserido.");
						Escolha = 0;
					}
				}
				if(Escolha == 5){
					Tabuleiro2[Linha][Coluna] = '.';
					TabuleiroId2[Linha][Coluna] = 0;
					Contador2--;
					printf("Objeto removido.");
				}
			}
		}
		else{
			printf("\nPosi��o j� ocupada ou proxima de mais de outro objeto. Manter ao menos uma casa de dist�ncia de qualquer outro objeto ja inserido.\n");
		}

		Escolha = 0;

		for(int i = 0; i < 10; i++) {
    		for(int j = 0; j < 10; j++) {
        		printf("%c ", Tabuleiro2[i][j]);
    		}
    	printf("\n");
		}

	}

	//Aqui encerra a parte do código de inserir a frota  do jogador 2

	//inicio do c�digo na parte combate
	//Valores iniciais dos tabuleros dos jogador rival na vis�o do jogador combatente
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

	printf("\nHora do combate!");

	while(Acertos < 20 && Acertos2 < 20){//Enquanto nenhum dos dois jogadores totalizar 20 acertos no jogo, o jogo continua
		//inicio do código na parte de combate do jogador 1
		if(Vez == 1){ //Vez do jogador 1
			printf("\nVez do jogador 1");
			printf("Situação atual de sua frota:\n"); //Auto-explicativo
			for(int i = 0; i < 10; i++) {
	    		for(int j = 0; j < 10; j++) {
	        		printf("%c ", Tabuleiro[i][j]);
	    		}
	    		printf("\n");
			}

			printf("Situação atual da frota de seu inimigo:\n"); //Tabuleiro do jogador 2 na vis�o do jogador 1
			for(int i = 0; i < 10; i++) {
	    		for(int j = 0; j < 10; j++) {
	        		printf("%c ", TabuleiroEsc2[i][j]);
	    		}
	    		printf("\n");
			}

			printf("\nInsira a posição onde os misseis serão lançados.");
			printf("\nInsira a linha da posição desejada");
			scanf("%d", &Linha);
			while(Linha < 0 || Linha > 9){
				printf("\nLinha fora dos limites do tabuleiro, favor inserir uma entre 0 e 9.");
				scanf("%d", &Linha);
			}
			printf("Insira a coluna da posição desejada");
			scanf("%d", &Coluna);
			while(Coluna < 0 || Coluna > 9){
				printf("\nColuna fora dos limites do tabuleiro, favor inserir uma entre 0 e 9.");
				scanf("%d", &Coluna);
			}

			if(Tabuleiro2[Linha][Coluna] == '#' ){ //Caso a linha e a coluna batam com a # no tabuleiro verdadeiro do jogador 2, ser� efetuado e mostrado o X em ambos os tabuleiros
				TabuleiroEsc2[Linha][Coluna] = 'X';
				Tabuleiro2[Linha][Coluna] = 'X';
				printf("\nVoc� acertou!\n");
				Acertos++; //o n�mero de acertos sobe
			}
			else if(Tabuleiro2[Linha][Coluna] == 'X' || Tabuleiro2[Linha][Coluna] == '!'){
				printf("\nVoc� j� atirou nessa posi��o anteriormente.\n");
			}
			else{ //Caso contr�rio ser� mostrado um . no tabuleiro de vis�o do jogador 1 e um ! no tabuleiro verdadeiro do jogador 2
				TabuleiroEsc2[Linha][Coluna] = '.';
				Tabuleiro2[Linha][Coluna] = '!';
				printf("\nVoc� errou!");
			}

			//Embaixo mostra o c�digo para identificar o objeto ao jogador ao ter afundado o objeto do jogador rival
			if(TabuleiroId2[Linha][Coluna] == 1){
				printf("Voc� afundou um submarino do inimigo\n");
			}
			else if(TabuleiroId2[Linha][Coluna] == 2){
				if((Tabuleiro2[Linha + 1][Coluna] == 'X' && TabuleiroId2[Linha + 1][Coluna] == 2) || (Tabuleiro2[Linha][Coluna + 1] == 'X' && TabuleiroId2[Linha][Coluna + 1] == 2) || (Tabuleiro2[Linha - 1][Coluna] == 'X' && TabuleiroId2[Linha - 1][Coluna] == 2) || (Tabuleiro2[Linha][Coluna - 1] == 'X' && TabuleiroId2[Linha][Coluna - 1] == 2)){
					printf("Voc� afundou um cruzador\n");
				}
			}
			else if(TabuleiroId2[Linha][Coluna] == 3){
				if((Tabuleiro2[Linha + 1][Coluna] == 'X' && TabuleiroId2[Linha + 1][Coluna] == 3) || (Tabuleiro2[Linha][Coluna + 1] == 'X' && TabuleiroId2[Linha][Coluna + 1] == 3) || (Tabuleiro2[Linha - 1][Coluna] == 'X' && TabuleiroId2[Linha - 1][Coluna] == 3) || (Tabuleiro2[Linha][Coluna - 1] == 'X' && TabuleiroId2[Linha][Coluna - 1] == 3)){
					if((Tabuleiro2[Linha + 2][Coluna] == 'X' && TabuleiroId2[Linha + 2][Coluna] == 3) || (Tabuleiro2[Linha][Coluna + 2] == 'X' && TabuleiroId2[Linha][Coluna + 2] == 3) || (Tabuleiro2[Linha - 2][Coluna] == 'X' && TabuleiroId2[Linha - 2][Coluna] == 3) || (Tabuleiro2[Linha][Coluna - 2] == 'X' && TabuleiroId2[Linha][Coluna - 2] == 3)){
						printf("Voc� afundou um encoura�ado\n");
					}
				}
			}
			else if(TabuleiroId2[Linha][Coluna] == 4){
				if((Tabuleiro2[Linha + 1][Coluna] == 'X' && TabuleiroId2[Linha + 1][Coluna] == 4) || (Tabuleiro2[Linha][Coluna + 1] == 'X' && TabuleiroId2[Linha][Coluna + 1] == 4) || (Tabuleiro2[Linha - 1][Coluna] == 'X' && TabuleiroId2[Linha - 1][Coluna] == 4) || (Tabuleiro2[Linha][Coluna - 1] == 'X' && TabuleiroId2[Linha][Coluna - 1] == 4)){
					if((Tabuleiro2[Linha + 2][Coluna] == 'X' && TabuleiroId2[Linha + 2][Coluna] == 4) || (Tabuleiro2[Linha][Coluna + 2] == 'X' && TabuleiroId2[Linha][Coluna + 2] == 4) || (Tabuleiro2[Linha - 2][Coluna] == 'X' && TabuleiroId2[Linha - 2][Coluna] == 4) || (Tabuleiro2[Linha][Coluna - 2] == 'X' && TabuleiroId2[Linha][Coluna - 2] == 4)){
						if((Tabuleiro2[Linha + 3][Coluna] == 'X' && TabuleiroId2[Linha + 3][Coluna] == 4) || (Tabuleiro2[Linha][Coluna + 3] == 'X' && TabuleiroId2[Linha][Coluna + 3] == 4) || (Tabuleiro2[Linha - 3][Coluna] == 'X' && TabuleiroId2[Linha - 3][Coluna] == 4) || (Tabuleiro2[Linha][Coluna - 3] == 'X' && TabuleiroId2[Linha][Coluna - 3] == 4)){
							printf("Voc� afundou um porta-avi�o.\n");
						}
					}
				}
			}

			//Tabuleiro foi atualizado
			for(int i = 0; i < 10; i++) {
		    	for(int j = 0; j < 10; j++) {
		        	printf("%c ", TabuleiroEsc2[i][j]);
		    	}
		    printf("\n");
			}

			if(Acertos == 20){ //Caso de 20 acertos o jogo � encerrado
				printf("\n\nJogador 1 ganhou! Fim de jogo.");
			}
			else{ //Caso contrario, passa a vez
				Vez++;
			}

		}
		//Fim do c�digo na parte de combate do jogador 1 e inicio do jogador 2
		//Basicamente a mesma coisa do jogador 1, substituindo apenas as variaveis dos tabuleiros e de acerto
		if(Vez == 2){
			printf("\nVez do jogador 2");
			printf("Situa��o atual da sua frota:\n");
			for(int i = 0; i < 10; i++) {
	    		for(int j = 0; j < 10; j++) {
	        		printf("%c ", Tabuleiro2[i][j]);
	    		}
	    		printf("\n");
			}

			printf("\nSitua��o atual da frota de seu inimigo:\n");
			for(int i = 0; i < 10; i++) {
	    		for(int j = 0; j < 10; j++) {
	        		printf("%c ", TabuleiroEsc[i][j]);
	    		}
	    		printf("\n");
			}

			printf("\nInsira a posi��o onde os misseis ser�o lan�ados.");
			printf("\nInsira a linha da posi��o desejada");
			scanf("%d", &Linha);
			while(Linha < 0 || Linha > 9){
				printf("\nLinha fora dos limites do tabuleiro, favor inserir uma entre 0 e 9.");
				scanf("%d", &Linha);
			}
			printf("Insira a coluna da posi��o desejada");
			scanf("%d", &Coluna);
			while(Coluna < 0 || Coluna > 9){
				printf("\nColuna fora dos limites do tabuleiro, favor inserir uma entre 0 e 9.");
				scanf("%d", &Coluna);
			}

			if(Tabuleiro[Linha][Coluna] == '#' ){
				TabuleiroEsc[Linha][Coluna] = 'X';
				Tabuleiro[Linha][Coluna] = 'X';
				printf("\nVoc� acertou.\n");
				Acertos2++;
			}
			else if(Tabuleiro[Linha][Coluna] == 'X' || Tabuleiro[Linha][Coluna] == '!'){
				printf("\nVoc� j� atirou nessa posi��o anteriormente.\n");
			}
			else{
				TabuleiroEsc[Linha][Coluna] = '.';
				Tabuleiro[Linha][Coluna] = '!';
				printf("\nVoc� errou.\n");

			}

			if(TabuleiroId[Linha][Coluna] == 1){
				printf("Voc� afundou um submarino do inimigo\n");
			}
			else if(TabuleiroId[Linha][Coluna] == 2){
				if((Tabuleiro[Linha + 1][Coluna] == 'X' && TabuleiroId[Linha + 1][Coluna] == 2) || (Tabuleiro[Linha][Coluna + 1] == 'X' && TabuleiroId[Linha][Coluna + 1] == 2) || (Tabuleiro[Linha - 1][Coluna] == 'X' && TabuleiroId[Linha - 1][Coluna] == 2) || (Tabuleiro[Linha][Coluna - 1] == 'X' && TabuleiroId[Linha][Coluna - 1] == 2)){
					printf("Voc� afundou um cruzador\n");
				}
			}
			else if(TabuleiroId[Linha][Coluna] == 3){
				if((Tabuleiro[Linha + 1][Coluna] == 'X' && TabuleiroId[Linha + 1][Coluna] == 3) || (Tabuleiro[Linha][Coluna + 1] == 'X' && TabuleiroId[Linha][Coluna + 1] == 3) || (Tabuleiro[Linha - 1][Coluna] == 'X' && TabuleiroId[Linha - 1][Coluna] == 3) || (Tabuleiro[Linha][Coluna - 1] == 'X' && TabuleiroId[Linha][Coluna - 1] == 3)){
					if((Tabuleiro[Linha + 2][Coluna] == 'X' && TabuleiroId[Linha + 2][Coluna] == 3) || (Tabuleiro[Linha][Coluna + 2] == 'X' && TabuleiroId[Linha][Coluna + 2] == 3) || (Tabuleiro[Linha - 2][Coluna] == 'X' && TabuleiroId[Linha - 2][Coluna] == 3) || (Tabuleiro[Linha][Coluna - 2] == 'X' && TabuleiroId[Linha][Coluna - 2] == 3)){
						printf("Voc� afundo um encoura�ado");
					}
				}
			}
			else if(TabuleiroId[Linha][Coluna] == 4){
				if((Tabuleiro[Linha + 1][Coluna] == 'X' && TabuleiroId[Linha + 1][Coluna] == 4) || (Tabuleiro[Linha][Coluna + 1] == 'X' && TabuleiroId[Linha][Coluna + 1] == 4) || (Tabuleiro[Linha - 1][Coluna] == 'X' && TabuleiroId[Linha - 1][Coluna] == 4) || (Tabuleiro[Linha][Coluna - 1] == 'X' && TabuleiroId[Linha][Coluna - 1] == 4)){
					if((Tabuleiro[Linha + 2][Coluna] == 'X' && TabuleiroId[Linha + 2][Coluna] == 4) || (Tabuleiro[Linha][Coluna + 2] == 'X' && TabuleiroId[Linha][Coluna + 2] == 4) || (Tabuleiro[Linha - 2][Coluna] == 'X' && TabuleiroId[Linha - 2][Coluna] == 4) || (Tabuleiro[Linha][Coluna - 2] == 'X' && TabuleiroId[Linha][Coluna - 2] == 4)){
						if((Tabuleiro[Linha + 3][Coluna] == 'X' && TabuleiroId[Linha + 3][Coluna] == 4) || (Tabuleiro[Linha][Coluna + 3] == 'X' && TabuleiroId[Linha][Coluna + 3] == 4) || (Tabuleiro[Linha - 3][Coluna] == 'X' && TabuleiroId[Linha - 3][Coluna] == 4) || (Tabuleiro[Linha][Coluna - 3] == 'X' && TabuleiroId[Linha][Coluna - 3] == 4)){
							printf("Voc� afundou um porta-avi�o.\n");
						}
					}
				}
			}

			for(int i = 0; i < 10; i++) {
		    	for(int j = 0; j < 10; j++) {
		        	printf("%c ", TabuleiroEsc[i][j]);
		    	}
		    printf("\n");
			}

			if(Acertos2 == 20){
				printf("\n\nJogador 2 ganhou! Fim de jogo.");
			}
			else{ //Caso contrario, passa a vez
				Vez--;
			}

		}
	}
	return(0);
}
