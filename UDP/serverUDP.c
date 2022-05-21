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
#define MAX_MSG 100

int main(int argc, char *argv[]) {
  
  int sd, rc, n, cliLen;
  struct sockaddr_in cliAddr, servAddr;
  char msg[MAX_MSG];

  // Criacao do socket
  sd=socket(AF_INET, SOCK_DGRAM, 0);
  if(sd<0) {
    printf("%s: Nao foi possivel abrir o socket \n",argv[0]);
    exit(EXIT_FAILURE);
  }

  // Liga-se ao endereco e porta do servidor
  servAddr.sin_family = AF_INET;
  servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servAddr.sin_port = htons(LOCAL_SERVER_PORT);
  rc = bind (sd, (struct sockaddr *) &servAddr,sizeof(servAddr));
  if(rc<0) {
    printf("%s: Nao foi possivel estabelecer uma ligacao com a porta %d \n", 
	   argv[0], LOCAL_SERVER_PORT);
    exit(EXIT_FAILURE);
  }

  printf("%s: Esperando por dados na porta UDP %u\n", 
	   argv[0], LOCAL_SERVER_PORT);

  // Laco infinito no servidor
  while(1) {
    
    // Inicializa buffer 
    memset(msg,0x0,MAX_MSG);

    // Recebe a mensagem
    cliLen = sizeof(cliAddr);
    n = recvfrom(sd, msg, MAX_MSG, 0, 
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
