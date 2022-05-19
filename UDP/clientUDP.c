/**
 * Programa cliente para exemplificar uma conexao 
 * com o protocolo UDP com um servidor.
 */  
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h> /* memset() */
#include <sys/time.h> /* select() */ 

#define REMOTE_SERVER_PORT 1500
#define MAX_MSG 100


int main(int argc, char *argv[]) {
  
  int sd, rc, i;
  struct sockaddr_in cliAddr, remoteServAddr;
  struct hostent *host;

  // Verificar argumentos da linha de comando
  if(argc<3) {
    printf("uso : %s <IP Servidor> <dado1> ... <dadoN> \n", argv[0]);
    exit(EXIT_FAILURE);
  }

  // Obtem o endereco IP ou host como argumento da entrada.
  host = gethostbyname(argv[1]);

  if (host == NULL) 
  {
    printf("%s: host desconhecido '%s' \n", argv[0], argv[1]);
    exit(EXIT_FAILURE);
  }

  printf("%s: Enviando dados para '%s' (IP : %s) \n", argv[0], host->h_name,
	 inet_ntoa(*(struct in_addr *) host->h_addr_list[0]));

  // Copiando informacoes do servidor para a estrutura
  remoteServAddr.sin_family = host->h_addrtype;
  memcpy((char *) &remoteServAddr.sin_addr.s_addr, 
	 host->h_addr_list[0], host->h_length);
  remoteServAddr.sin_port = htons(REMOTE_SERVER_PORT);

  // Criacao do socket
  sd = socket(AF_INET,SOCK_DGRAM,0);
  if(sd<0) {
    printf("%s: nao e possivel abrir o socket \n",argv[0]);
    exit(EXIT_FAILURE);
  }
  
  // Conecta-se a qualquer endereco/porta disponivel
  cliAddr.sin_family = AF_INET;
  cliAddr.sin_addr.s_addr = htonl(INADDR_ANY);
  cliAddr.sin_port = htons(0);
  
  // Liga-se ao endereco do cliente
  rc = bind(sd, (struct sockaddr *) &cliAddr, sizeof(cliAddr));
  if(rc<0) {
    printf("%s: Nao pode se ligar a porta\n", argv[0]);
    exit(EXIT_FAILURE);
  }


  // Enviar os dados
  for(i=2;i<argc;i++) {
    rc = sendto(sd, argv[i], strlen(argv[i])+1, 0, 
		(struct sockaddr *) &remoteServAddr, 
		sizeof(remoteServAddr));

    if(rc<0) {
      printf("%s: Nao foi possivel enviar os dados %d \n",argv[0],i-1);
      close(sd);
      exit(EXIT_FAILURE);
    }

  }
  
  return 0;
}
