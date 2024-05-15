/**
 * Programa servidor para exemplificar uma conexao
 * com o protocolo UDP com um cliente.
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> /* close() */
#include <string.h> /* memset() */

#define LOCAL_SERVER_PORT 4321
#define MAX 100

int main(int argc, char *argv[]) 
{
  // Descritor do software
  int sd;
  // Variavel para armazenar o tamanho da estrutura com dados do cliente
  int cliLen;
  int rc, n;

  // Estruturas para armazenar as informacoes do cliente
  // e do servidor;
  struct sockaddr_in cliAddr;
  struct sockaddr_in servAddr;

  // String para armazenar as mensagens
  char msg[MAX];

  // Criacao e verificacao do socket.
  // Parametros:
  //   AF_INET: o Protocolo IPv4
  //   SOCK_DGRAM: Equivalente ao protocolo UDP
  //   0: Informa um protocolo especifico na familia de protocolos. Pode deixar 0 mesmo
  sd=socket(AF_INET, SOCK_DGRAM, 0);
  if(sd<0) {
    printf("%s: Nao foi possivel abrir o socket \n",argv[0]);
    exit(EXIT_FAILURE);
  }

  // Atribui as informacoes especificas na estrutura servaddr
  //  A familia de protocolos e o AF_INET (IPv4)
  servAddr.sin_family = AF_INET;
  //  Endereco do qual aceita conexoes. 
  //  Nesse caso, INADDR_ANY significa qualquer endereco.
  servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
  //  A porta a se conectar esta no LOCAL_SERVER_PORT (nesse caso e a 4321) 
  servAddr.sin_port = htons(LOCAL_SERVER_PORT);

  // Faz a ligacao do socket ao endereco IP e porta.
  // A primitiva bind utiliza tres parametros:
  //   - o descritor do socket
  //   - a estrutura com os dados do servidor preenchida nos comandos anteriores
  //   - o tamanho da estrutura com os dados do servidor
  rc = bind (sd, (struct sockaddr *) &servAddr, sizeof(servAddr));
  if(rc<0) {
    printf("%s: Nao foi possivel estabelecer uma ligacao com a porta %d \n", 
	   argv[0], LOCAL_SERVER_PORT);
    exit(EXIT_FAILURE);
  }

  printf("%s: Esperando por dados na porta UDP %u\n", 
	   argv[0], LOCAL_SERVER_PORT);

  // Laco infinito no servidor
  while(1) {
    
    // Zera buffer 
    bzero(&msg, sizeof(msg));

    // Calcula o tamanho da estrutura que armazenara os dados do cliente
    cliLen = sizeof(cliAddr);

    // Recebe os dados da rede e armazena no buffer.
    // A primitiva recvfrom precisa de seis parametros:
    //   - o descritor do software
    //   - variavel que contera os dados. Nesse caso, o buffer
    //   - tamanho da variavel que contera os dados
    //   - as flags. Nesse caso, mantenha 0 ou de uma olhada no manual do recvfrom
    //   - estrutura com as informacoes do cliente
    //   - tamanho da estrutura com as informacoes do cliente
    n = recvfrom(sd, msg, sizeof(msg), 0, 
		 (struct sockaddr *) &cliAddr, &cliLen);

    if(n<0) {
      printf("%s: Nao foi possivel receber os dados \n",argv[0]);
      continue;
    }
      
    // Imprime as mensagens recebidas
    printf("%s: recebido do %s:UDP %u : %s \n", 
	   argv[0],inet_ntoa(cliAddr.sin_addr),
	   ntohs(cliAddr.sin_port),msg);
    
  }// Fim do laco no servidor

return 0;

}
