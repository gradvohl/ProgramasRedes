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

#define REMOTE_SERVER_PORT 4321
#define MAX 100


int main(int argc, char *argv[]) 
{
  // Descritor do socket
  int sd; 
  // Variaveis auxiliares
  int rc, i;

  // Estruturas para armazenar os dados do cliente
  // e do servidor
  struct sockaddr_in cliAddr;
  struct sockaddr_in remoteServAddr;

  // Estruturas para armazenar dados do host
  struct hostent *host;

  // Verificar argumentos da linha de comando
  if(argc<3) {
    printf("uso : %s <IP Servidor> <dado1> ... <dadoN> \n", argv[0]);
    exit(EXIT_FAILURE);
  }

  // Obtem o endereco IP ou host enviado como argumento da entrada.
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

  // Criacao e verificacao do socket.
  // Parametros:
  //   AF_INET: o Protocolo IPv4
  //   SOCK_DGRAM: Equivalente ao protocolo UDP
  //   0: Informa um protocolo especifico na familia de protocolos. Pode deixar 0 mesmo
  sd = socket(AF_INET,SOCK_DGRAM,0);
  if(sd<0) {
    printf("%s: nao e possivel abrir o socket \n",argv[0]);
    exit(EXIT_FAILURE);
  }
  
  // Atribui as informacoes especificas na estrutura cliAddr
  //  A familia de protocolos e o AF_INET (IPv4)
  cliAddr.sin_family = AF_INET;
  //  Endereco do qual aceita conexoes.
  //  Nesse caso, INADDR_ANY significa qualquer endereco.
  cliAddr.sin_addr.s_addr = htonl(INADDR_ANY);
  // O htons(0) vai indicar que o sistema operacional escolhera a porta.
  cliAddr.sin_port = htons(0);
  
  // Faz a ligacao do socket ao endereco IP e porta.
  // A primitiva bind utiliza tres parametros:
  //   - o descritor do socket
  //   - a estrutura com os dados do servidor preenchida nos comandos anteriores
  //   - o tamanho da estrutura com os dados do servidor
  rc = bind(sd, (struct sockaddr *) &cliAddr, sizeof(cliAddr));
  if(rc<0) {
    printf("%s: Nao pode se ligar a porta\n", argv[0]);
    exit(EXIT_FAILURE);
  }


  // Enviar os dados
  for(i=2;i<argc;i++) {
    // A primitiva sendto recebe seis parametros:
    //   - descritor do software
    //   - variavel que contem os dados os dados a serem enviados
    //   - tamanho da variavel que contem os dados a serem enviados
    //   - as flags. Nesse caso, mantenha 0 ou de uma olhada no manual do sendto
    //   - estrutura com as informacoes do servidor
    //   - tamanho da estrutura com as informacoes do servidor
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
