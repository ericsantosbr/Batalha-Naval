#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> /* close() */
#include <string.h> /* memset() */

typedef int bool;
#define true 1
#define false 0
#define LOCAL_SERVER_PORT 8000
#define REMOTE_SERVER_PORT 8000
#define MAX_MSG 100

#define TSIZE 10

typedef struct sCasa{

	int id;
	char c;

} casa;

enum Print_Opc{
	CLARO = 0,
	ESCURO,
	IDS
};

void inicializa(casa T[TSIZE][TSIZE]);
void imprimeCampo(casa T[TSIZE][TSIZE], int opt);
bool confere(casa T[TSIZE][TSIZE], int i, int j);
void insereBarcos(casa T[TSIZE][TSIZE]);
int tiro(casa T[TSIZE][TSIZE], char i, char j);

int main(){
    int kind;
    int Acertos = 0;
    int servSock, sd, rc, n, cliLen, i, Linha, Coluna, shot=-1;
  	struct sockaddr_in cliAddr, servAddr, remoteServAddr;
  	char msg[MAX_MSG];
  	struct hostent *h;

  	casa Tabuleiro[TSIZE][TSIZE], Tabuleiro2[TSIZE][TSIZE];

    printf("Selecione a opcao de funcionamento:\n \t 1 - Server \n\t 2 - Client\n\n_");
    scanf("%d", &kind);

	inicializa(Tabuleiro);
  	inicializa(Tabuleiro2);
	imprimeCampo(Tabuleiro, 0);
	insereBarcos(Tabuleiro);
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

		    if(msg[0] == 's'){
		    	printf("Verificando o tiro\n");
		    	shot = tiro(Tabuleiro, msg[1], msg[2]);
		    }

		    if(strlen(msg)>3 && msg[3] == 'r'){
			    if(msg[4] == '1'){ //Caso a linha e a coluna batam com a # no tabuleiro verdadeiro do jogador 2, sera efetuado e mostrado o X em ambos os tabuleiros
					Tabuleiro2[Linha][Coluna].c = 'X';
					printf("\nVoce acertou!\n");
					Acertos++; //o numero de acertos sobe
				}
				else if(msg[4] == '2'){
					printf("\nVoce ja atirou nessa Posicao anteriormente.\n");
				}
				else{ //Caso contrario sera mostrado um "." no tabuleiro de visao do jogador 1 e um ! no tabuleiro verdadeiro do jogador 2
					Tabuleiro2[Linha][Coluna].c = '.';
					printf("\nVoce errou!");
				}
		    }

		    printf("\nInsira a posição onde os misseis serão lançados!");

			printf("Insira a coluna da posição desejada: ");
			scanf("%d", &Coluna);
			while(Coluna < 0 || Coluna > 9){
				printf("\nColuna fora dos limites do tabuleiro, favor inserir uma entre 0 e 9.");
				scanf("%d", &Coluna);
			}

			printf("\nInsira a linha da posição desejada: ");
			scanf("%d", &Linha);
			while(Linha < 0 || Linha > 9){
				printf("\nLinha fora dos limites do tabuleiro, favor inserir uma entre 0 e 9.");
				scanf("%d", &Linha);
			}

			if(shot>=0) sprintf(msg, "s%d%dr%d",Coluna, Linha, shot);
			else sprintf(msg, "s%d%d",Coluna, Linha);

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
		  	printf("Situação atual de sua frota:\n");
			imprimeCampo(Tabuleiro, 0);
			printf("Situação atual da frota de seu inimigo:\n");
			imprimeCampo(Tabuleiro2, 0);

			printf("Insira a coluna da posição desejada: ");
			scanf("%d", &Coluna);
			while(Coluna < 0 || Coluna > 9){
				printf("\nColuna fora dos limites do tabuleiro, favor inserir uma entre 0 e 9.");
				scanf("%d", &Coluna);
			}

			printf("\nInsira a linha da posição desejada: ");
			scanf("%d", &Linha);
			while(Linha < 0 || Linha > 9){
				printf("\nLinha fora dos limites do tabuleiro, favor inserir uma entre 0 e 9.");
				scanf("%d", &Linha);
			}

			if(shot>=0) sprintf(msg, "s%d%dr%d",Coluna, Linha, shot);
			else sprintf(msg, "s%d%d",Coluna, Linha);

		    rc = sendto(sd, msg, strlen(msg)+1, 0,
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

		    if(msg[0] == 's'){
		    	shot = tiro(Tabuleiro, msg[1], msg[2]);
		    }

		  if(strlen(msg)>3 && msg[3] == 'r'){
			    if(msg[4] == '1'){ //Caso a linha e a coluna batam com a # no tabuleiro verdadeiro do jogador 2, sera efetuado e mostrado o X em ambos os tabuleiros
					Tabuleiro2[Linha][Coluna].c = 'X';
					printf("\nVoce acertou!\n");
					Acertos++; //o numero de acertos sobe
				}
				else if(msg[4] == '2'){
					printf("\nVoce ja atirou nessa Posicao anteriormente.\n");
				}
				else{ //Caso contrario sera mostrado um "." no tabuleiro de visao do jogador 1 e um ! no tabuleiro verdadeiro do jogador 2
					Tabuleiro2[Linha][Coluna].c = '.';
					printf("\nVoce errou!");
				}
		    }

		    printf("%s: from %s:UDP%u : %s size: %d\n",
			   "argv[0]",inet_ntoa(remoteServAddr.sin_addr),
			   ntohs(remoteServAddr.sin_port),msg, rc);

		  }

    }
    else{
        printf("Opção inválida!");
    }
}


/**
 * IMPLEMENTACAO DAS FUNCOES
 */

void inicializa(casa T[TSIZE][TSIZE]){
	int i, j;
	for(i=0; i<TSIZE; i++){
		for(j=0;j<TSIZE; j++){
			T[i][j].id = 0;
			T[i][j].c = '.';
		}
	}
}

void imprimeCampo(casa T[TSIZE][TSIZE], int opt){
	int i, j;
	printf("\n\t");
	for(i = 0; i<10; i++) printf("%d ", i);
	printf("\n");
	for(i = 0; i<10; i++){
		printf("%d\t", i);
		for(j = 0; j< TSIZE; j++){
			if(opt == CLARO) printf("%c ", T[i][j].c);
			else if(opt == ESCURO){
				printf("%c ", '?');
			}
			else printf("%d ", T[i][j].id);
		}
		printf("\n");
	}
}

bool confere(casa T[TSIZE][TSIZE], int i, int j){

		if(T[i][j].c == '#') return false;
		if(i-1>=0)
			if(T[i-1][j].c == '#') return false;
		if(i+1<TSIZE)
			if(T[i+1][j].c == '#') return false;
		if(j-1>=0)
			if(T[i][j-1].c == '#') return false;
		if(j+1<TSIZE)
			if(T[i][j+1].c == '#') return false;
		if(i-1>=0 && j-1>=0)
			if(T[i-1][j-1].c == '#') return false;
		if(i-1>=0 && j+1<TSIZE)
			if(T[i-1][j+1].c == '#') return false;
		if(i+1>TSIZE && j-1>=0)
			if(T[i+1][j-1].c == '#') return false;
		if(i+1<TSIZE && j+1>TSIZE)
			if(T[i+1][j+1].c == '#') return false;

		return true;
}

void insereBarcos(casa T[TSIZE][TSIZE]){
	int Coluna, Linha, i, Escolha;

	//Insire submarinos
	i=0;
	while(i<4){
		printf("Insira a posição dos submarinos (um por vez)\n");

		printf("\nInsira a coluna da posição desejada: ");
		scanf("%d", &Coluna); //Input da coluna desejada
		while(Coluna < 0 || Coluna > 9){
			printf("\nColuna fora dos limites do tabuleiro, favor inserir uma entre 0 e 9: ");
			scanf("%d", &Coluna);
		}

		printf("\nInsira a linha da posição desejada: ");
		scanf("%d", &Linha); //Input da linha desejada
		while(Linha < 0 || Linha > 9){
			printf("\nLinha fora dos limites do tabuleiro, favor inserir uma entre 0 e 9: ");
			scanf("%d", &Linha);
		}
		if(confere(T, Coluna, Linha)){
			T[Linha][Coluna].c = '#';
			T[Linha][Coluna].id = 1;
			i++;
		}else{
			printf("\nPosição já ocupada ou proxima demais de outro objeto.\nManter ao menos uma casa de distância de qualquer outro objeto ja inserido.\n");
		}
		imprimeCampo(T, 0);
	}

	//Insere cruzadores
	i=3;
	while(i<3){
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

		if(confere(T, Linha, Coluna)){
			T[Linha][Coluna].c = '#';
			T[Linha][Coluna].id = 2;
			i++;
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
					else if(T[Linha - 2][Coluna].c != '#' && T[Linha - 1][Coluna - 1].c != '#' &&
					T[Linha - 1][Coluna + 1].c != '#' && T[Linha - 2][Coluna - 1].c != '#' &&
					T[Linha - 2][Coluna + 1].c != '#'){ //Sera verifica se nao ha objetos em volta ou na posicao desejada
						T[Linha - 1][Coluna].c = '#'; //Sera efetuada as casas desejadas
						T[Linha - 1][Coluna].id = 2; //Sera efetuada as casas desejadas
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
					else if(T[Linha + 2][Coluna].c != '#' && T[Linha + 1][Coluna + 1].c != '#' &&
					T[Linha + 1][Coluna - 1].c != '#' && T[Linha + 2][Coluna + 1].c != '#' &&
					T[Linha + 2][Coluna - 1].c != '#'){
						T[Linha + 1][Coluna].c = '#';
						T[Linha + 1][Coluna].id = 2;
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
					else if(T[Linha][Coluna - 2].c != '#' && T[Linha + 1][Coluna - 1].c != '#' &&
					T[Linha - 1][Coluna - 1].c != '#' && T[Linha - 1][Coluna - 2].c != '#' &&
					T[Linha + 1][Coluna - 2].c != '#'){
						T[Linha][Coluna - 1].c = '#';
						T[Linha][Coluna - 1].c = 2;
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
					else if(T[Linha][Coluna + 2].c != '#' && T[Linha + 1][Coluna + 1].c != '#' &&
					T[Linha - 1][Coluna + 1].c != '#' && T[Linha + 1][Coluna + 2].c != '#' &&
					T[Linha + 1][Coluna + 2].c != '#'){
						T[Linha][Coluna + 1].c = '#';
						T[Linha][Coluna + 1].id = 2;
					}
					else{
						printf("\nPosicao proxima demais de outro objeto.\nManter ao menos uma casa de distancia de qualquer outro objeto ja inserido: ");
						Escolha = 0;
					}
				}
				//Caso nao tenha sido possivel inseri com as outras escolhas
				//ou o jogador decidiu que nao era a melhor posicao, essa escolha permite o jogador remover o objeto ja efetuado e escolher outra posicao
				if(Escolha == 5){
					T[Linha][Coluna].c = '.';
					T[Linha][Coluna].id = 0;
					i--; //Contador e reduzido devido a remocao da posicao anteriormente inserida
					printf("Objeto removido.");
				}
				imprimeCampo(T, 0);
			}
		}
		else{
			printf("\nPosicao ja ocupada ou proxima demais de outro objeto.\nManter ao menos uma casa de distancia de qualquer outro objeto ja inserido.\n");
			imprimeCampo(T, 0);
		}

		//Escolha e resetada para ser usada novamente mais tarde
		Escolha = 0;

	}

	i = 2;
	while(i < 2){ //Contador utilizado para inserir todos os encouraçados no tabuleiro
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

		if(confere(T, Linha, Coluna)){
			T[Linha][Coluna].c = '#';
			T[Linha][Coluna].id = 3;
			i++;
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
					else if(T[Linha - 2][Coluna].c != '#' && T[Linha - 3][Coluna].c != '#' && T[Linha - 2][Coluna - 1].c != '#' &&
					T[Linha - 2][Coluna + 1].c != '#' && T[Linha - 3][Coluna - 1].c != '#' && T[Linha - 3][Coluna + 1].c != '#'){
						T[Linha - 1][Coluna].c = '#';
						T[Linha - 2][Coluna].c = '#';
						T[Linha - 1][Coluna].id = 3;
						T[Linha - 2][Coluna].id = 3;
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
					else if(T[Linha + 2][Coluna].c != '#' && T[Linha + 3][Coluna].c != '#' && T[Linha + 2][Coluna + 1].c != '#' &&
					T[Linha + 2][Coluna - 1].c != '#' && T[Linha + 3][Coluna + 1].c != '#' && T[Linha + 3][Coluna - 1].c != '#'){
						T[Linha + 1][Coluna].c = '#';
						T[Linha + 2][Coluna].c = '#';
						T[Linha + 1][Coluna].id = 3;
						T[Linha + 2][Coluna].id = 3;
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
					else if(T[Linha][Coluna - 2].c != '#' && T[Linha][Coluna - 3].c != '#' && T[Linha + 1][Coluna - 2].c != '#' &&
					T[Linha - 1][Coluna - 2].c != '#' && T[Linha - 1][Coluna - 3].c != '#' && T[Linha + 1][Coluna - 3].c != '#'){
						T[Linha][Coluna - 1].c = '#';
						T[Linha][Coluna - 2].c = '#';
						T[Linha][Coluna - 1].id = 3;
						T[Linha][Coluna - 2].id = 3;
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
					else if(T[Linha][Coluna + 2].c != '#' && T[Linha][Coluna + 3].c != '#' && T[Linha + 1][Coluna + 2].c != '#' &&
					T[Linha - 1][Coluna + 2].c != '#' && T[Linha + 1][Coluna + 3].c != '#' && T[Linha + 1][Coluna + 3].c != '#'){
						T[Linha][Coluna + 1].c = '#';
						T[Linha][Coluna + 2].c = '#';
						T[Linha][Coluna + 1].id = 3;
						T[Linha][Coluna + 2].id = 3;
					}
					else{
						printf("\nPosicao proxima demais de outro objeto.\nManter ao menos uma casa de distancia de qualquer outro objeto ja inserido.");
						Escolha = 0;
					}
				}

				if(Escolha == 5){
					T[Linha][Coluna].c = '.';
					T[Linha][Coluna].id = 0;
					i--;
					printf("Objeto removido.");
				}
			}
		}
		else{
			printf("\nPosicao ja ocupada ou proxima demais de outro objeto.\nManter ao menos uma casa de distancia de qualquer outro objeto ja inserido.");
		}

		Escolha = 0;

		imprimeCampo(T, 0);
	}
	i = 1;
	while(i < 1){ //Contador utilizado para inserir o porta-avião no tabuleiro
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


		if(confere(T, Linha, Coluna)){
			T[Linha][Coluna].c = '#';
			T[Linha][Coluna].id = 4;
			i++;
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
					else if(T[Linha - 2][Coluna].c != '#' && T[Linha - 3][Coluna].c != '#' && T[Linha - 4][Coluna].c != '#' &&
					T[Linha - 2][Coluna - 1].c != '#' && T[Linha - 2][Coluna + 1].c != '#' && T[Linha - 3][Coluna - 1].c != '#' &&
					T[Linha - 3][Coluna + 1].c != '#' && T[Linha - 4][Coluna - 1].c != '#' && T[Linha - 4][Coluna + 1].c != '#'){
						T[Linha - 1][Coluna].c = '#';
						T[Linha - 2][Coluna].c = '#';
						T[Linha - 3][Coluna].c = '#';
						T[Linha - 1][Coluna].id = 4;
						T[Linha - 2][Coluna].id = 4;
						T[Linha - 3][Coluna].id = 4;
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
					else if(T[Linha + 2][Coluna].c != '#' && T[Linha + 3][Coluna].c != '#' && T[Linha + 4][Coluna].c != '#' && T[Linha + 2][Coluna + 1].c != '#' && T[Linha + 2][Coluna - 1].c != '#' && T[Linha + 3][Coluna + 1].c != '#' && T[Linha + 3][Coluna - 1].c != '#' && T[Linha + 4][Coluna - 1].c != '#' && T[Linha + 4][Coluna + 1].c != '#'){
						T[Linha + 1][Coluna].c = '#';
						T[Linha + 2][Coluna].c = '#';
						T[Linha + 3][Coluna].c = '#';
						T[Linha + 1][Coluna].id = 4;
						T[Linha + 2][Coluna].id = 4;
						T[Linha + 3][Coluna].id = 4;
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
					else if(T[Linha][Coluna - 2].c != '#' && T[Linha][Coluna - 3].c != '#' && T[Linha][Coluna - 4].c != '#' && T[Linha + 1][Coluna - 2].c != '#' && T[Linha - 1][Coluna - 2].c != '#' && T[Linha - 1][Coluna - 3].c != '#' && T[Linha + 1][Coluna - 3].c != '#' && T[Linha - 1][Coluna - 4].c != '#' && T[Linha + 1][Coluna - 4].c != '#'){
						T[Linha][Coluna - 1].c = '#';
						T[Linha][Coluna - 2].c = '#';
						T[Linha][Coluna - 3].c = '#';
						T[Linha][Coluna - 1].id = 4;
						T[Linha][Coluna - 2].id = 4;
						T[Linha][Coluna - 3].id = 4;
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
					else if(T[Linha][Coluna + 2].c != '#' && T[Linha][Coluna + 3].c != '#' && T[Linha][Coluna + 4].c != '#' && T[Linha + 1][Coluna + 2].c != '#' && T[Linha - 1][Coluna + 2].c != '#' && T[Linha + 1][Coluna + 3].c != '#' && T[Linha + 1][Coluna + 3].c != '#' && T[Linha - 1][Coluna + 4].c != '#' && T[Linha + 1][Coluna + 4].c != '#'){
					T[Linha][Coluna + 1].c = '#';
					T[Linha][Coluna + 2].c = '#';
					T[Linha][Coluna + 3].c = '#';
					T[Linha][Coluna + 1].id = 4;
					T[Linha][Coluna + 2].id = 4;
					T[Linha][Coluna + 3].id = 4;
					}
					else{
						printf("\nPosicao proxima demais de outro objeto.\nManter ao menos uma casa de distancia de qualquer outro objeto ja inserido.");
						Escolha = 0;
					}
				}
				if(Escolha == 5){
					T[Linha][Coluna].c = '.';
					T[Linha][Coluna].id = 0;
					i--;
					printf("Objeto removido.");
				}
			}
		}
		else{
			printf("\nPosicao ja ocupada ou proxima demais de outro objeto.\nManter ao menos uma casa de distancia de qualquer outro objeto ja inserido.\n");
			imprimeCampo(T, 0);
		}
		Escolha = 0;
	}
}

int tiro(casa T[TSIZE][TSIZE], char i, char j){
	printf("%c\t%c\n", i, i);
	int ii = atoi(&i);
	int ij = atoi(&j);
	printf("%d\t%d\n", ii, ij);
	if(T[ii][ij].c == '#' ){ //Caso a i e a j batam com a # no tabuleiro verdadeiro do jogador 2, sera efetuado e mostrado o X em ambos os tabuleiros
		T[ii][ij].c = 'X';
		printf("\nO Inimigo lhe acertou\n");
		return 1;
	}
	else if(T[ii][ij].c == 'X' || T[ii][ij].c == '!'){
		return 2;
	}
	else{ //Caso contrario sera mostrado um "." no tabuleiro de visao do jogador 1 e um ! no tabuleiro verdadeiro do jogador 2
		T[ii][ij].c = '!';
		return 0;
	}


}

